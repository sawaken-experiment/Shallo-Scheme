#include "object.h"

#ifndef PRIMITIVE_FUNCTION_H
#define PRIMITIVE_FUNCTION_H

Object* Prim_putc(Object* env, Object* args, Object* cont);
Object* Prim_naive_set_e(Object* env, Object* args, Object* cont);
Object* Prim_display(Object* env, Object* args, Object* cont);

Object* Prim_cons(Object* env, Object* args, Object* cont);
Object* Prim_car(Object* env, Object* args, Object* cont);
Object* Prim_cdr(Object* env, Object* args, Object* cont);
Object* Prim_list(Object* env, Object* args, Object* cont);

/*
  Type predicate
*/
Object* Prim_symbol_q(Object* env, Object* args, Object* cont);
Object* Prim_null_q(Object* env, Object* args, Object* cont);
Object* Prim_pair_q(Object* env, Object* args, Object* cont);
Object* Prim_list_q(Object* env, Object* args, Object* cont);
Object* Prim_boolean_q(Object* env, Object* args, Object* cont);
Object* Prim_string_q(Object* env, Object* args, Object* cont);
Object* Prim_procedure_q(Object* env, Object* args, Object* cont);
Object* Prim_symbol_q(Object* env, Object* args, Object* cont);
Object* Prim_null_q(Object* env, Object* args, Object* cont);


Object* Prim_eq_q(Object* env, Object* args, Object* cont);
Object* Prim_raise(Object* env, Object* args, Object* cont);
Object* Prim_load(Object* env, Object* args, Object* cont);

Object* Prim_get_nc(Object*env, Object* args, Object* cont);
Object* Prim_naive_define(Object* env, Object* args, Object* cont);
Object* Prim_naive_lambda(Object* env, Object* args, Object* cont);
Object* Prim_eval(Object* env, Object* args, Object* cont);
Object* Prim_interaction_environment(Object* env, Object* args, Object* cont);
Object* Prim_read(Object* env, Object* args, Object* cont);

Object* Prim_number_eq(Object* env, Object* args, Object* cont);
Object* Prim_add(Object* env, Object* args, Object* cont);
Object* Prim_sub(Object* env, Object* args, Object* cont);
Object* Prim_mul(Object* env, Object* args, Object* cont);
Object* Prim_div(Object* env, Object* args, Object* cont);

void EnterPrimitiveFunc(Object*env, Object* args, Object* cont);
void ExitPrimitiveFunc(Object*env, Object* args, Object* cont);

Object* Exception(char message[], Object* env, Object* args, Object* cont);
Object* PrimReturn(Object* val, Object* env, Object* args, Object* cont);
#endif
