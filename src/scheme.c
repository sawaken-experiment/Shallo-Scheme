#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "object.h"
#include "object_lib.h"
#include "parse.h"
#include "primitive_function.h"
#include "eval.h"
#include "string_representation.h"
#include "global.h"
#include "garbage_collector.h"
#include "cl_macro.h"
#include "file_loader.h"


Object* MakeTopEnvironment(){
  Object* top_env = NewEnvironment(NewEnvironment(NULL, NULL), NULL);

  AttachToEnv(top_env, GetSymbolAsSingleton("if"), NewSpecialForm("if"));
  AttachToEnv(top_env, GetSymbolAsSingleton("quote"), NewSpecialForm("quote"));
  AttachToEnv(top_env, GetSymbolAsSingleton("define-macro"), NewSpecialForm("define-macro"));
  AttachToEnv(top_env, GetSymbolAsSingleton("body-form"), NewSpecialForm("body-form"));
  AttachToEnv(top_env, GetSymbolAsSingleton("top-level"), NewSpecialForm("top-level"));
  AttachToEnv(top_env, GetSymbolAsSingleton("catch"), NewSpecialForm("catch"));
  AttachToEnv(top_env, GetSymbolAsSingleton("splicing"), NewSpecialForm("splicing"));

  AttachToEnv(top_env, GetSymbolAsSingleton("putc"), NewPrimitiveFunc(Prim_putc));
  AttachToEnv(top_env, GetSymbolAsSingleton("raise"), NewPrimitiveFunc(Prim_raise));
  AttachToEnv(top_env, GetSymbolAsSingleton("display"), NewPrimitiveFunc(Prim_display));
  AttachToEnv(top_env, GetSymbolAsSingleton("load"), NewPrimitiveFunc(Prim_load));

  AttachToEnv(top_env, GetSymbolAsSingleton("cons"), NewPrimitiveFunc(Prim_cons));
  AttachToEnv(top_env, GetSymbolAsSingleton("car"), NewPrimitiveFunc(Prim_car));
  AttachToEnv(top_env, GetSymbolAsSingleton("cdr"), NewPrimitiveFunc(Prim_cdr));
  AttachToEnv(top_env, GetSymbolAsSingleton("list"), NewPrimitiveFunc(Prim_list));

  AttachToEnv(top_env, GetSymbolAsSingleton("symbol?"), NewPrimitiveFunc(Prim_symbol_q));
  AttachToEnv(top_env, GetSymbolAsSingleton("null?"), NewPrimitiveFunc(Prim_null_q));
  AttachToEnv(top_env, GetSymbolAsSingleton("pair?"), NewPrimitiveFunc(Prim_pair_q));
  AttachToEnv(top_env, GetSymbolAsSingleton("list?"), NewPrimitiveFunc(Prim_list_q));
  AttachToEnv(top_env, GetSymbolAsSingleton("boolean?"), NewPrimitiveFunc(Prim_boolean_q));
  AttachToEnv(top_env, GetSymbolAsSingleton("string?"), NewPrimitiveFunc(Prim_string_q));
  AttachToEnv(top_env, GetSymbolAsSingleton("procedure?"), NewPrimitiveFunc(Prim_procedure_q));
  AttachToEnv(top_env, GetSymbolAsSingleton("eq?"), NewPrimitiveFunc(Prim_eq_q));

  AttachToEnv(top_env, GetSymbolAsSingleton("+"), NewPrimitiveFunc(Prim_add));
  AttachToEnv(top_env, GetSymbolAsSingleton("-"), NewPrimitiveFunc(Prim_sub));
  AttachToEnv(top_env, GetSymbolAsSingleton("*"), NewPrimitiveFunc(Prim_mul));
  AttachToEnv(top_env, GetSymbolAsSingleton("/"), NewPrimitiveFunc(Prim_div));
  AttachToEnv(top_env, GetSymbolAsSingleton("="), NewPrimitiveFunc(Prim_number_eq));

  AttachToEnv(top_env, GetSymbolAsSingleton("naive-define"), NewPrimitiveFunc(Prim_naive_define));
  AttachToEnv(top_env, GetSymbolAsSingleton("naive-lambda"), NewPrimitiveFunc(Prim_naive_lambda));
  AttachToEnv(top_env, GetSymbolAsSingleton("naive-set!"), NewPrimitiveFunc(Prim_naive_set_e));

  AttachToEnv(top_env, GetSymbolAsSingleton("get-nc"), NewPrimitiveFunc(Prim_get_nc));

  AttachToEnv(top_env, GetSymbolAsSingleton("eval"), NewPrimitiveFunc(Prim_eval));
  AttachToEnv(top_env, GetSymbolAsSingleton("interaction-environment"),
	      NewPrimitiveFunc(Prim_interaction_environment));
  AttachToEnv(top_env, GetSymbolAsSingleton("read"), NewPrimitiveFunc(Prim_read));

  return top_env;
}

int main(int args, char* argv[]){
  char* input;
  Object *top_env, *parsed_exp;

  if(args != 2 || !strcmp(argv[1], "--help")){
    printf("Usage: ./scheme src_file\n");
    exit(EXIT_FAILURE);
  }

  GLOBAL_init();

  input = process_input(argv[1]);

  if( input == NULL ){
    printf("Input file cannot be opened.\n");
    exit(EXIT_FAILURE);
  }

  parsed_exp = SafeParseExp(input);

  if( isCondition(parsed_exp) ){
    printf("%s\n", TakeCondition(parsed_exp)->message);
    exit(EXIT_FAILURE);
  }

  top_env = MakeTopEnvironment();
  EnterEval(parsed_exp, top_env, NULL);
  EvalFromInput(parsed_exp, top_env, NULL);

  return 0;
}
