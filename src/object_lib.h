#include <stdbool.h>
#include "object.h"

#ifndef OBJECT_LIB_H
#define OBJECT_LIB_H

long long int IdToInt(Object* id);
bool EQ(Object* obj1, Object* obj2);
bool isProcedure(Object* object);

Object* Cons(Object* car, Object* cdr);
Object* Car(Object* cell);
Object* Cdr(Object* cell);
Object* Caar(Object* cell);
Object* Cadr(Object* cell);
Object* Cdar(Object* cell);
Object* Cddr(Object* cell);
Object* NthCdr(int n, Object* cell);
int Length(Object* cell);
bool Member(Object* list, Object* object);
bool All(Object* object_list, bool (*p)(Object*));
Object* Reverse(Object* list, Object* reversed);
Object* Append(Object* lst1, Object* lst2);
Object* Last(Object* list);
Object* Nth(int i, Object* list);
Object* PushList(Object* dest_lst, Object* src_lst);
bool isList(Object* object);
bool isString(Object* object);
Object* Reduce(Object* (*func)(Object*, Object*), Object* init, Object* list);

/*
  alist (Cell)
*/

Object* Assoc(Object* key_symbol, Object* alist);
Object* Push(Object* pair, Object* alist);
Object* Remove(Object* key_symbol, Object* alist);
Object* MakeAlist(Object* keys, Object* vals);
Object* MakeReverseAlist(Object* keys, Object* reverse_vals);

/*
  Integer
*/
Object* IntegerAdd(Object* int1, Object* int2);
Object* IntegerSub(Object* int1, Object* int2);
Object* IntegerMul(Object* int1, Object* int2);
Object* IntegerDiv(Object* int1, Object* int2);
Object* IntegerEqual(Object* int1, Object* int2);

/*
  Symbol
*/
Object* GetSymbolAsSingleton(char name[]);
bool SymbolMatchString(Object* symbol, char string[]);

/*
  Lambda
*/
Object* NewLambdaEnv(Object* lambda, Object* args);
bool ValidNumberOfArgument(int num, Object* lambda);
Object* ExtractFixedParams(Object* params);
Object* ExtractNonFixedParams(Object* params);
bool isMacroEnv(Object* env);
Object* getMacroEnv(Object* env);

/*
  PrimitiveFunc
*/
Object* CallPrimitiveFunc(Object* pfunc, Object* args, Object* env, Object* cont);

/*
  Environment
*/
Object* NewLambdaWithParamAnalysis(Object* env, Object* params_def, Object* body);
Object* Resolve(Object* environment, Object* symbol);
Object* EnvFind(Object* environment, Object* symbol);
void AttachToEnv(Object* env, Object* symbol, Object* object);
Object* BoundEnv(Object* environment, Object* symbol);
/*
  Bool
*/
bool isFalse(Object* bool_object);
bool isTrue(Object* bool_object);

/*
  Special Form
*/
bool isFormOf(char name[], Object* sp);

#endif
