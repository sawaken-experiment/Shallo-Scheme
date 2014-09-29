#ifndef EVAL_H
#define EVAL_H


Object* Continue(Object* now_cont);
Object* Raise(Object* cont, Object* raised_object);
Object* Return(Object* target_cont, Object* return_object);
Object* Eval(Object* object, Object* env, Object* cont);
Object* Apply(Object* exp, Object* env, Object* cont);
Object* EvalFromInput(Object* input_exp, Object* env, Object* cont);

bool isBodyForm(Object* env, Object* object);
bool isQuoteForm(Object* env, Object* object);
bool isDefMacroForm(Object* env, Object* object);

bool isIfContinuation(Object* cont);
bool isTopLevelForm(Object* cont);
bool isLastExpOfBody(Object* cont);
bool isCatchForm(Object* cont);
bool ValidDefinePosition(Object* body, Object* env, bool top_flag);

void EnterContinue(Object* now_cont);
void ExitContinue(Object* now_cont);
void EnterRaise(Object* cont, Object* raised_object);
void ExitRaise(Object* cont, Object* raised_object);
void EnterReturn(Object* target_cont, Object* return_object);
void ExitReturn(Object* target_cont, Object* return_object);
void EnterEval(Object* object, Object* env, Object* target_cont);
void ExitEval(Object* object, Object* env, Object* target_cont);
void EnterApply(Object* exp, Object* env, Object* cont);
void ExitApply(Object* exp, Object* env, Object* cont);

#endif
