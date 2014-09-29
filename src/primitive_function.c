#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "primitive_function.h"
#include "object.h"
#include "object_lib.h"
#include "string_representation.h"
#include "eval.h"
#include "garbage_collector.h"
#include "file_loader.h"
#include "parse.h"
#include "cl_macro.h"

Object* Prim_putc(Object* env, Object* args, Object* cont){
  if( Length(args) != 1 && isCharacter(Car(args)) )
    return Exception("Runtime Error: invalid application for putc.", env, args, cont);

  printf("%c", TakeCharacter(Car(args))->c);
  return PrimReturn(Car(args), env, args, cont);
}

Object* Prim_naive_set_e(Object* env, Object* args, Object* cont){
  Object *symbol = Car(args), *value = Cadr(args);
  Object* bound_env = BoundEnv(env, symbol);

  if( isNil(bound_env) )
    return Exception("Runtime Error: symbol given to naive-set! is not bound on environment.", env, args, cont);

  AttachToEnv(bound_env, symbol, value);
  return PrimReturn(value, env, args, cont);
}

Object* Prim_display(Object* env, Object* args, Object* cont){
  if( Length(args) != 1 )
    return Exception("Runtime Error: invalid application for display.", env, args, cont);

  PrintDesc(Car(args));
  return PrimReturn(NewBool(true), env, args, cont);
}

Object* Prim_cons(Object* env, Object* args, Object* cont){
  if( Length(args) != 2 )
    return Exception("Runtime Error: invalid application for cons.", env, args, cont);

  return PrimReturn(NewConsCell(Car(args), Cadr(args)), env, args, cont);
}

Object* Prim_car(Object* env, Object* args, Object* cont){
  if( Length(args) != 1 || !isConsCell(Car(args)) )
    return Exception("Runtime Error: invalid application for car.", env, args, cont);

  return PrimReturn(Car(Car(args)), env, args, cont);
}

Object* Prim_cdr(Object* env, Object* args, Object* cont){
  if( Length(args) != 1 || !isConsCell(Car(args)) )
    return Exception("Runtime Error: invalid application for cdr.", env, args, cont);

  return PrimReturn(Cdr(Car(args)), env, args, cont);
}

Object* Prim_list(Object* env, Object* args, Object* cont){
  return PrimReturn(args, env, args, cont);
}

/*
  Predicate
*/

Object* Prim_symbol_q(Object* env, Object* args, Object* cont){
  if( Length(args) != 1 )
    return Exception("Runtime Error: invalid application for symbol?.", env, args, cont);

  return PrimReturn(NewBool(isSymbol(Car(args))), env, args, cont);
}

Object* Prim_null_q(Object* env, Object* args, Object* cont){
  if( Length(args) != 1 )
    return Exception("Runtime Error: invalid application for null?.", env, args, cont);

  return PrimReturn(NewBool(isNil(Car(args))), env, args, cont);
}

Object* Prim_pair_q(Object* env, Object* args, Object* cont){
  if( Length(args) != 1 )
    return Exception("Runtime Error: invalid application for pair?.", env, args, cont);

  return PrimReturn(NewBool(isConsCell(Car(args))), env, args, cont);
}

Object* Prim_list_q(Object* env, Object* args, Object* cont){
  if( Length(args) != 1 )
    return Exception("Runtime Error: invalid application for list?.", env, args, cont);

  return PrimReturn(NewBool(isList(Car(args))), env, args, cont);
}

Object* Prim_boolean_q(Object* env, Object* args, Object* cont){
  if( Length(args) != 1 )
    return Exception("Runtime Error: invalid application for boolean?.", env, args, cont);

  return PrimReturn(NewBool(isBool(Car(args))), env, args, cont);
}

Object* Prim_string_q(Object* env, Object* args, Object* cont){
  if( Length(args) != 1 )
    return Exception("Runtime Error: invalid application for string?.", env, args, cont);

  return PrimReturn(NewBool(isString(Car(args))), env, args, cont);
}

Object* Prim_procedure_q(Object* env, Object* args, Object* cont){
  if( Length(args) != 1 )
    return Exception("Runtime Error: invalid apply for procedure?.", env, args, cont);

  return PrimReturn(NewBool(isProcedure(Car(args))), env, args, cont);
}

Object* Prim_eq_q(Object* env, Object* args, Object* cont){
  if( Length(args) != 2 )
    return Exception("Runtime Error: invalid apply for eq?.", env, args, cont);

  return PrimReturn(NewBool(EQ(Car(args), Cadr(args))), env, args, cont);
}



Object* Prim_raise(Object* env, Object* args, Object* cont){
  Object* raised_object = Car(args);

  EnterRaise(cont, raised_object);
  ExitPrimitiveFunc(env, args, cont);
  return Raise(cont, raised_object);
}

Object* Prim_load(Object* env, Object* args, Object* cont){
  char* load_program_text;
  Object* parsed_load_program;
  Object* filename_symbol = Car(args);

  if( !isMacroEnv(TakeEnvironment(env)->parent) )
    return Exception("Runtime Error: load cannot be used in out of top-level.", env, args, cont);
  
  if( !isSymbol(filename_symbol) )
    return Exception("Runtime Error: argument should be symbol of filename.", env, args, cont);

  if( (load_program_text = process_input(TakeSymbol(filename_symbol)->name)) == NULL )
    return Exception("Runtime Error: cannot open load file.", env, args, cont);

  parsed_load_program = SafeParseExp(load_program_text);

  EnterEval(parsed_load_program, env, cont);
  ExitPrimitiveFunc(env, args, cont);
  return EvalFromInput(parsed_load_program, env, cont);
}

Object* Prim_get_nc(Object*env, Object* args, Object* cont){
  if( Length(args) != 0 )
    return Exception("Runtime Error: invalid application of get-nc.", env, args, cont);

  return PrimReturn(TakeContinuation(cont)->next, env, args, cont);
}

Object* Prim_naive_define(Object* env, Object* args, Object* cont){
  if( Length(args) != 2 )
    return Exception("Runtime Error: invalid application of naive-define", env, args, cont);

  Object *symbol = Car(args), *value = Cadr(args);

  AttachToEnv(env, symbol, value);
  return PrimReturn(symbol, env, args, cont);
}
 
Object* Prim_naive_lambda(Object* env, Object* args, Object* cont){
  Object *params_def = Car(args);
  Object *body = Cadr(args);
  Object* new_lambda = NewLambdaWithParamAnalysis(env, params_def, body);

  EnterReturn(cont, new_lambda);
  ExitPrimitiveFunc(env, args, cont);
  return Return(cont, new_lambda);
}

Object* Prim_eval(Object* env, Object* args, Object* cont){
  if( Length(args) != 2 || !isEnvironment(Cadr(args)) )
    return Exception("Runtime Error: invalid application of eval", env, args, cont);

  Object *exp = Car(args), *specified_env = Cadr(args);

  EnterEval(exp, specified_env, cont);
  ExitPrimitiveFunc(env, args, cont);
  return Eval(exp, specified_env, cont);
}

Object* Prim_interaction_environment(Object* env, Object* args, Object* cont){
  if( Length(args) != 0 )
    return Exception("Runtime Error: invalid application of interaction-environment", env, args, cont);

  return PrimReturn(env, env, args, cont);  
}

Object* Prim_read(Object* env, Object* args, Object* cont){
  char buf[1024];
  Object* parsed;

  if( Length(args) != 0 )
    return Exception("Runtime Error: invalid application of read", env, args, cont);

  int i = 0;
  char c;
  while(true){
    c = getchar();
    buf[i++] = c;
    if(c == '\n'){
      buf[i] = '\0';
      break;
    }
  }

  parsed = SafeParseExp(buf);

  if( isCondition(parsed) )
    return Exception("Runtime Error: read error.", env, args, cont);
  
  return PrimReturn(parsed, env, args, cont);  
}

/*
  String
*/


/*
  Number calculation
*/
Object* Prim_number_eq(Object* env, Object* args, Object* cont){
  if( !All(args, isInteger) || Length(args) != 2 )
    return Exception("Runtime Error: invalid application for =.", env, args, cont);

  return PrimReturn(IntegerEqual(Car(args), Cadr(args)), env, args, cont);
}

Object* Prim_add(Object* env, Object* args, Object* cont){
  if( !All(args, isInteger) )
    return Exception("Runtime Error: invalid application for +.", env, args, cont);

  return PrimReturn(Reduce(IntegerAdd, NewInteger(0), args), env, args, cont);
}

Object* Prim_sub(Object* env, Object* args, Object* cont){
  if( !All(args, isInteger) || Length(args) != 2 )
    return Exception("Runtime Error: invalid application for -.", env, args, cont);

  return PrimReturn(IntegerSub(Car(args), Cadr(args)), env, args, cont);
}

Object* Prim_mul(Object* env, Object* args, Object* cont){
  if( !All(args, isInteger) )
    return Exception("Runtime Error: invalid application for *.", env, args, cont);

  return PrimReturn(Reduce(IntegerMul, NewInteger(1), args), env, args, cont);
}

Object* Prim_div(Object* env, Object* args, Object* cont){
  if( !All(args, isInteger) || Length(args) != 2 )
    return Exception("Runtime Error: invalid application for /.", env, args, cont);

  return PrimReturn(IntegerDiv(Car(args), Cadr(args)), env, args, cont);
}

/*
  Enter and Exit
*/

void EnterPrimitiveFunc(Object* env, Object* args, Object* cont){
  IncRefCount(env);
  IncRefCount(args);
  IncRefCount(cont);
}

void ExitPrimitiveFunc(Object* env, Object* args, Object* cont){
  DecRefCount(env);
  DecRefCount(args);
  DecRefCount(cont);
}


/*
  utility
*/

Object* Exception(char message[], Object* env, Object* args, Object* cont){
  Object* raised_cond = NewCondition(message);
    
  EnterRaise(cont, raised_cond);
  ExitPrimitiveFunc(env, args, cont);
  return Raise(cont, raised_cond);
}

Object* PrimReturn(Object* val, Object* env, Object* args, Object* cont){
  EnterReturn(cont, val);
  ExitPrimitiveFunc(env, args, cont);
  return Return(cont, val);
}
