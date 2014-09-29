#include "object.h"

#ifndef CL_MACRO_H
#define CL_MACRO_H

Object* ExtractDefMacro(Object* exp_list, Object* env);
Object* RemoveDefMacro(Object* exp_list, Object* env);
bool isDefmacroForm(Object* env, Object* object);
Object* ToMacroAList(Object* def_list, Object* env);
Object* ToMacroAssocPair(Object* defmacro_form, Object* env);
Object* ApplyMacro(Object* macro_alist, Object* exp_list);
Object* ApplyMacroToExp(Object* macro_alist, Object* exp);
Object* MacroConvert(Object* lambda, Object* exp);

Object* ListMacroExpand(Object* exp_list, Object* env);
Object* MacroExpand(Object* exp, Object* env);

bool matchMacro(Object* exp, Object* env);
bool matchListMacro(Object* exp_list, Object* env);

Object* ProcessInput(Object* exp_list, Object* top_env, Object* cont);
Object* ProcessMacroAndEval(Object* def_macro_list, Object* exp_list, Object* env, Object* cont);
void EnterProcessInput(Object* exp_list, Object* top_env, Object* cont);
void ExitProcessInput(Object* exp_list, Object* top_env, Object* cont);
void EnterProcessMacroAndEval(Object* def_macro_list, Object* exp_list, Object* env, Object* cont);
void ExitProcessMacroAndEval(Object* def_macro_list, Object* exp_list, Object* env, Object* cont);

#endif
