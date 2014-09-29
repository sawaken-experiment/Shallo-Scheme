#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include "object.h"
#include "object_lib.h"
#include "eval.h"
#include "primitive_function.h"
#include "string_representation.h"
#include "garbage_collector.h"
#include "global.h"
#include "cl_macro.h"

Object* Continue(Object* cont){
  MARK_AND_SWEEP_try(cont);

  Continuation* entity = TakeContinuation(cont);
  
  if( isNil(entity->exp) ){
    Object* exp = Reverse(entity->args, NULL);
    Object *env = entity->environment, *next = entity->next;

    EnterApply(exp, env, next);
    ExitContinue(cont);
    return Apply(exp, env, next);
  }

  else{
    Object* new_cont = NewContinuation(Cdr(entity->exp), entity->args, entity->environment, entity->next);
    Object *exp = Car(entity->exp), *env = entity->environment;

    EnterEval(exp, env, new_cont);
    ExitContinue(cont);
    return Eval(exp, env, new_cont);
  }
}

Object* Raise(Object* cont, Object* raised_object){
  if( isNil(cont) ){
    printf("Runtime Error: raised ");
    PrintDesc(raised_object);
    exit(0);
  }

  if( isCatchForm(cont) ){
    Continuation* entity = TakeContinuation(cont);
    Object* catch_proc  = Nth(Length(entity->args) - 2, entity->args);
    Object* env = entity->environment;
    Object* target_cont = entity->next;

    if( !isLambda(catch_proc) ){
      Object* raised_cond = NewCondition("format of 'catch' form is invalid.");

      EnterRaise(target_cont, raised_cond);
      ExitRaise(cont, raised_object);
      return Raise(target_cont, raised_cond);
    }

    Object* eval_exp = Cons(catch_proc, Cons(raised_object, NULL));

    EnterApply(eval_exp, env, target_cont);
    ExitRaise(cont, raised_object);
    return Apply(eval_exp, env, target_cont);
  }

  else{
    Object* target_cont = TakeContinuation(cont)->next;
    
    EnterRaise(target_cont, raised_object);
    ExitRaise(cont, raised_object);
    return Raise(target_cont, raised_object);
  }
}

Object* Return(Object* target_cont, Object* return_value){
  if( isNil(target_cont) )
    return return_value;

  else{
    Continuation* entity = TakeContinuation(target_cont);
    Object* new_cont = NewContinuation(entity->exp, Cons(return_value, entity->args),
				       entity->environment, entity->next);

    EnterContinue(new_cont);
    ExitReturn(target_cont, return_value);
    return Continue(new_cont);
  }
}


Object* Eval(Object* object, Object* env, Object* cont){
  if( isIfContinuation(cont) ){
    Continuation* entity = TakeContinuation(cont);
    Object* altered_cont = entity->next;
    Object* altered_object;

    if( isTrue(Car(entity->args)) )
      altered_object = object;
    else
      altered_object = (isNil(entity->exp) ? NULL : Car(entity->exp));

    EnterEval(altered_object, env, altered_cont);
    ExitEval(object, env, cont);
    return Eval(altered_object, env, altered_cont);
  }

  if( isLastExpOfBody(cont) ){
    Object* altered_cont = TakeContinuation(cont)->next;

    EnterEval(object, env, altered_cont);
    ExitEval(object, env, cont);
    return Eval(object, env, altered_cont);
  }
  
  if( isTopLevelForm(cont)  && matchMacro(object, env) ){
    Object* expanded = MacroExpand(object, env);

    EnterEval(expanded, env, cont);
    ExitEval(object, env, cont);
    return Eval(expanded, env, cont);
  }

  if( isDefMacroForm(env, object) ){
    Object* name_sym   = Car(Car(NthCdr(1, object)));
    Object* params_def = Cdr(Car(NthCdr(1, object)));
    Object* body       = Cons(GetSymbolAsSingleton("body-form"), NthCdr(2, object));
    
    AttachToEnv(getMacroEnv(env),
		name_sym,
		NewLambdaWithParamAnalysis(env, params_def, body));

    Object* return_value = NewBool(true);
    EnterReturn(cont, return_value);
    ExitEval(object, env, cont);
    return Return(cont, return_value);
  }

  if( isQuoteForm(env, object) ){
    Object* quoted = Cadr(object);

    EnterReturn(cont, quoted);
    ExitEval(object, env, cont);
    return Return(cont, quoted);
  }

  if( isBodyForm(env, object) && !ValidDefinePosition(Cdr(object), env, true) ){
    Object* raised_cond = NewCondition("'naive-define' is only allowed in top of body.");
    
    EnterRaise(cont, raised_cond);
    ExitEval(object, env, cont);
    return Raise(cont, raised_cond);
  }

  if( isConsCell(object) ){
    Object* new_cont = NewContinuation(object, NULL, env, cont);

    EnterContinue(new_cont);
    ExitEval(object, env, cont);
    return Continue(new_cont);
  }

  if( isSymbol(object) ){

    Object* solution = EnvFind(env, object);
    if( isNil(solution) ){
      Object* raised_cond = NewCondition("Runtime Error: undefined reference.");
    
      EnterRaise(cont, raised_cond);
      ExitEval(object, env, cont);
      return Raise(cont, raised_cond);

    }else{
      Object* return_value = Cdr(solution);

      EnterReturn(cont, return_value);
      ExitEval(object, env, cont);
      return Return(cont, return_value);
    }
  }

  else{
    EnterReturn(cont, object);
    ExitEval(object, env, cont);
    return Return(cont, object);
  }
}

Object* EvalFromInput(Object* input_exp, Object* env, Object* cont){
  Object* top_level_exp = Cons(NewSpecialForm("top-level"), input_exp);

  EnterEval(top_level_exp, env, cont);
  ExitEval(input_exp, env, cont);
  return Eval(top_level_exp, env, cont);
}

Object* Apply(Object* exp, Object* env, Object* cont){
  Object *proc = Car(exp), *args = Cdr(exp);
  
  if( isPrimitiveFunc(proc) ){
    EnterPrimitiveFunc(env, args, cont);
    ExitApply(exp, env, cont);
    return CallPrimitiveFunc(proc, args, env, cont);
  }

  if( isFormOf("top-level", proc) || isFormOf("catch", proc) ){
    Object* return_value = Last(args);

    EnterReturn(cont, return_value);
    ExitApply(exp, env, cont);
    return Return(cont, return_value);
  }

  if( isLambda(proc) ){
    if( !ValidNumberOfArgument(Length(args), proc) ){
      Object* raised_cond = NewCondition("wrong number of arguments for give procedure.");
      
      EnterRaise(cont, raised_cond);
      ExitApply(exp, env, cont);
      return Raise(cont, raised_cond);
    }

    Object* new_lambda_env = NewLambdaEnv(proc, args);
    Object* body = TakeLambda(proc)->body;

    EnterEval(body, new_lambda_env, cont);
    ExitApply(exp, env, cont);
    return Eval(body, new_lambda_env, cont);
  }

  if( isFormOf("splicing", proc)){
    Continuation* cc_entity = TakeContinuation(cont);
    Object* altered_cont = NewContinuation(cc_entity->exp, PushList(cc_entity->args, Car(args)),
					   cc_entity->environment, cc_entity->next);

    EnterContinue(altered_cont);
    ExitApply(exp, env, cont);
    return Continue(altered_cont);
  }

  if( isContinuation(proc) ){
    Object* return_value = Car(args);
    Object* target_cont  = proc;

    EnterReturn(target_cont, return_value);
    ExitApply(exp, env, cont);
    return Return(target_cont, return_value);
  }

  else {
    Object* raised_cond = NewCondition("command of form is not procedure.");
    
    EnterRaise(cont, raised_cond);
    ExitApply(exp, env, cont);
    return Raise(cont, raised_cond);
  }
}

bool isForm(Object* env, Object* object){
  return isConsCell(object) && isSymbol(Car(object));
}

bool isQuoteForm(Object* env, Object* object){
  return isForm(env, object) && !isNil(EnvFind(env, Car(object)))
    && isFormOf("quote", Cdr(EnvFind(env, Car(object))));
}

bool isBodyForm(Object* env, Object* object){
  return isForm(env, object) && !isNil(EnvFind(env, Car(object)))
    && isFormOf("body-form", Cdr(EnvFind(env, Car(object))));
}

bool isDefMacroForm(Object* env, Object* object){
  return isForm(env, object) && !isNil(EnvFind(env, Car(object)))
    && isFormOf("define-macro", Cdr(EnvFind(env, Car(object))));
}

bool isIfContinuation(Object* cont){
  return !isNil(cont)
    && Length(TakeContinuation(cont)->args) == 2
    && isFormOf("if", Cadr(TakeContinuation(cont)->args));
}

bool isTopLevelForm(Object* cont){
  return !isNil(cont) && isFormOf("top-level", Last(TakeContinuation(cont)->args));
}
 
bool isLastExpOfBody(Object* cont){
  return !isNil(cont)
    && isFormOf("body-form", Last(TakeContinuation(cont)->args))
    && Length(TakeContinuation(cont)->exp) == 0;
}

bool isCatchForm(Object* cont){
  return !isNil(cont) && isFormOf("catch", Last(TakeContinuation(cont)->args));
}

bool ValidDefinePosition(Object* body, Object* env, bool top_flag){
  if( isNil(body) )
    return true;

  if( isConsCell(Car(body)) && isSymbol(Caar(body)) && SymbolMatchString(Caar(body), "naive-define") ){
    if( top_flag )
      return ValidDefinePosition(Cdr(body), env, true);

    else
      return false;
  }

  else
    return ValidDefinePosition(Cdr(body), env, false);
}

/*
  Enter and Exit
*/

void EnterContinue(Object* now_cont){
  IncRefCount(now_cont);
}

void ExitContinue(Object* now_cont){
  DecRefCount(now_cont);
}

void EnterRaise(Object* cont, Object* raised_object){
  IncRefCount(cont);
  IncRefCount(raised_object);
}

void ExitRaise(Object* cont, Object* raised_object){
  DecRefCount(cont);
  DecRefCount(raised_object);
}

void EnterReturn(Object* target_cont, Object* return_object){
  IncRefCount(target_cont);
  IncRefCount(return_object);
}

void ExitReturn(Object* target_cont, Object* return_object){
  DecRefCount(target_cont);
  DecRefCount(return_object);
}

void ExitEval(Object* object, Object* env, Object* target_cont){
  DecRefCount(object);
  DecRefCount(env);
  DecRefCount(target_cont);
}

void EnterEval(Object* object, Object* env, Object* target_cont){
  IncRefCount(object);
  IncRefCount(env);
  IncRefCount(target_cont);
}

void ExitApply(Object* exp, Object* env, Object* cont){
  DecRefCount(exp);
  DecRefCount(env);
  DecRefCount(cont);
}

void EnterApply(Object* exp, Object* env, Object* cont){
  IncRefCount(exp);
  IncRefCount(env);
  IncRefCount(cont);
}

