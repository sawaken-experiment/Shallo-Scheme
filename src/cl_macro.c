#include <stdlib.h>
#include <stdio.h>
#include "cl_macro.h"
#include "object.h"
#include "object_lib.h"
#include "eval.h"
#include "string_representation.h"
#include "global.h"
#include "garbage_collector.h"

Object* MacroConvert(Object* lambda, Object* exp){
  Object* eval_exp = Cons(lambda, Cdr(exp));
  MARK_AND_SWEEP_off();
  EnterApply(eval_exp, NULL, NULL);
  Object* converted = Apply(eval_exp, NULL, NULL);
  converted->ref_count = 0;
  MARK_AND_SWEEP_on();
  return converted;
}

Object* ListMacroExpand(Object* exp_list, Object* env){
  if( isNil(exp_list) )
    return NULL;

  if( !isConsCell(Car(exp_list)) )
    return Cons(Car(exp_list), ListMacroExpand(Cdr(exp_list), env));

  else
    return Cons(MacroExpand(Car(exp_list), env), ListMacroExpand(Cdr(exp_list), env));
}

Object* MacroExpand(Object* exp, Object* env){
  Object* command = Car(exp);
  Object* macro = EnvFind(getMacroEnv(env), command);

  if( isNil(macro) )
    return ListMacroExpand(exp, env);

  else
    return MacroConvert(Cdr(macro), exp);
}

bool matchMacro(Object* exp, Object* env){
  if( !isConsCell(exp) )
    return false;

  if( isNil(EnvFind(getMacroEnv(env), Car(exp))) )
    return matchListMacro(exp, env);

  else
    return true;
}

bool matchListMacro(Object* exp_list, Object* env){
  if( isNil(exp_list) )
    return false;

  if( !isConsCell(Car(exp_list)) )
    return matchListMacro(Cdr(exp_list), env);

  else
    return matchMacro(Car(exp_list), env) || matchListMacro(Cdr(exp_list), env);
}
