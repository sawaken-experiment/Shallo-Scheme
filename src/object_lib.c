#include <stdio.h> //for debug
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "object_lib.h"
#include "object.h"
#include "garbage_collector.h"
#include "string_representation.h" //for debug
#include "global.h"

long long int IdToInt(Object* id){
  return (long long int)id;
}

bool EQ(Object* obj1, Object* obj2){
  if( isNil(obj1) && isNil(obj2) )
    return true;
  else if( !isNil(obj1) && !isNil(obj2) && obj1->id == obj2->id)
    return true;
  else
    return false;
}

bool isProcedure(Object* object){
  return isLambda(object) || isPrimitiveFunc(object) || isContinuation(object);
}

/*
  cell
*/

Object* Cons(Object* car, Object* cdr){ return NewConsCell(car, cdr); }
Object* Car(Object* cell){ return TakeConsCell(cell)->car; }
Object* Cdr(Object* cell){ return TakeConsCell(cell)->cdr; }
Object* Caar(Object* cell){ return Car(Car(cell)); }
Object* Cadr(Object* cell){ return Car(Cdr(cell)); }
Object* Cdar(Object* cell){ return Cdr(Car(cell)); }
Object* Cddr(Object* cell){ return Cdr(Cdr(cell)); }

Object* NthCdr(int n, Object* cell){
  if( n <= 0 )
    return cell;
  else
    return NthCdr(n - 1, Cdr(cell));
}

int Length(Object* cell){
  if(isNil(cell))
    return 0;
  else
    return 1 + Length(Cdr(cell));
}

bool Member(Object* list, Object* object){
  if(isNil(list))
    return false;
  else if(Car(list)->id == object->id)
    return true;
  else
    return Member(Cdr(list), object);
}

bool All(Object* object_list, bool (*p)(Object*)){
  if( isNil(object_list) )
    return true;
  else
    return (*p)(Car(object_list)) && All(Cdr(object_list), p);
}

Object* Reverse(Object* list, Object* reversed){
  if(isNil(list))
    return reversed;
  else
    return Reverse(Cdr(list), Cons(Car(list), reversed));
}

Object* Append(Object* lst1, Object* lst2){
  if( isNil(lst1) )
    return lst2;

  else
    return Cons(Car(lst1), Append(Cdr(lst1), lst2));
}

Object* Last(Object* list){
  if( isNil(list) )
    return NULL;

  if( isNil(Cdr(list)) )
    return Car(list);

  else
    return Last(Cdr(list));
}

Object* Nth(int i, Object* list){
  if( i < 0 || isNil(list) )
    return NULL;

  if( i == 0 )
    return Car(list);

  else
    return Nth(i - 1, Cdr(list));
}

// dest_lst = (a b c), src_lst = (1 2 3) => (3 2 1 a b c)
Object* PushList(Object* dest_lst, Object* src_lst){
  if( isNil(src_lst) )
    return dest_lst;

  else
    return PushList(Cons(Car(src_lst), dest_lst), Cdr(src_lst));
}

bool isList(Object* object){
  if( isNil(object) )
    return true;

  if( !isConsCell(object) )
    return false;

  else
    return isList(Cdr(object));
}

bool isString(Object* object){
  return isList(object) && All(object, isCharacter);
}

Object* Reduce(Object* (*func)(Object*, Object*), Object* init, Object* list){
  if( isNil(list) )
    return init;

  else
    return (*func)(Car(list), Reduce(func, init, Cdr(list)));
}


/*
  alist (Cell)
*/

Object* Assoc(Object* key_symbol, Object* alist){
  if( isNil(alist) )
    return NULL;

  if( Caar(alist)->id == key_symbol )
    return Car(alist);

  else
    return Assoc(key_symbol, Cdr(alist));
}

Object* Push(Object* pair, Object* alist){
  return Cons(pair, alist);
}

Object* Remove(Object* key_symbol, Object* alist){
  if( isNil(alist) )
    return NULL;
  else if( Caar(alist)->id == key_symbol )
    return Cdr(alist);
  else
    return Cons(Car(alist), Remove(key_symbol, Cdr(alist)));
}

Object* MakeAlist(Object* keys, Object* vals){
  if( isNil(keys) || isNil(vals) )
    return NULL;
  else
    return Cons(Cons(Car(keys), Car(vals)), MakeAlist(Cdr(keys), Cdr(vals)));
}

/*
  Integer
*/
Object* IntegerAdd(Object* int1, Object* int2){
  return NewInteger(TakeInteger(int1)->i + TakeInteger(int2)->i);
}

Object* IntegerSub(Object* int1, Object* int2){
  return NewInteger(TakeInteger(int1)->i - TakeInteger(int2)->i);
}

Object* IntegerMul(Object* int1, Object* int2){
  return NewInteger(TakeInteger(int1)->i * TakeInteger(int2)->i);
}

Object* IntegerDiv(Object* int1, Object* int2){
  return NewInteger(TakeInteger(int1)->i / TakeInteger(int2)->i);
}

Object* IntegerEqual(Object* int1, Object* int2){
  return NewBool(TakeInteger(int1)->i == TakeInteger(int2)->i);
}

/*
  Symbol
*/
Object* GetSymbolAsSingleton(char name[]){
  Object* stored = SYMBOLS_find(name);

  if( isNil(stored) ){
    Object* symbol = NewSymbol(name);
    SYMBOLS_add(symbol);
    return symbol;
  }

  else
    return stored;
}

bool SymbolMatchString(Object* symbol, char string[]){
  return !strcmp(TakeSymbol(symbol)->name, string);
}

/*
  Lambda
*/

Object* NewLambdaWithParamAnalysis(Object* env, Object* params_def, Object* body){
  Object* params = ExtractFixedParams(params_def);
  Object* rest = ExtractNonFixedParams(params_def);
  return NewLambda(env, params, rest, body);
}

Object* NewLambdaEnv(Object* lambda, Object* args){
  Lambda* entity = TakeLambda(lambda);
  
  if(isNil(entity->rest_param))
    return NewEnvironment(entity->environment, MakeAlist(entity->params, args));

  else{
    Object* alist = Cons(Cons(entity->rest_param, NthCdr(Length(entity->params), args)),
			 MakeAlist(entity->params, args));
    return NewEnvironment(entity->environment, alist);
  }
}

bool ValidNumberOfArgument(int num, Object* lambda){
  Lambda* entity = TakeLambda(lambda);

  return Length(entity->params) <= num
    && (entity->rest_param || Length(entity->params) == num);
} 

Object* ExtractFixedParams(Object* params){
  if( isNil(params) || SymbolMatchString(Car(params), ".") )
    return NULL;

  if( isSymbol(Car(params)) )
    return Cons(Car(params), ExtractFixedParams(Cdr(params)));

  else{
    printf("Runtime Error: Specification of lambda's parameters is invalid.\n");
    exit(0);
  }   
}

Object* ExtractNonFixedParams(Object* params){
  if( isNil(params) )
    return NULL;

  if( SymbolMatchString(Car(params), ".") ){

    if( Length(Cdr(params)) == 0 )
      return NULL;

    if( Length(Cdr(params)) == 1 && isSymbol(Cadr(params)) )
      return Cadr(params);

    else{
      printf("Runtime Error: Specification of lambda's rest parameter is invalid.\n");
      exit(0);
    }
  }

  else
    return ExtractNonFixedParams(Cdr(params));
}

/*
  Primitive Func
*/

Object* CallPrimitiveFunc(Object* pfunc, Object* args, Object* env, Object* cont){
  return TakePrimitiveFunc(pfunc)->func(env, args, cont);
}


/*
  Environment
*/

Object* Resolve(Object* environment, Object* symbol){
  Object* found = EnvFind(environment, symbol);
  if(found == NULL){
    printf("resolve error\n");
    exit(0);
  }

  return Cdr(found);
}

Object* EnvFind(Object* environment, Object* symbol){
  if(environment == NULL)
    return NULL;

  else{
    Object* found = Assoc(symbol, TakeEnvironment(environment)->name_list);
    if(found)
      return found;
    else{
      if( isMacroEnv(environment) || isMacroEnv(TakeEnvironment(environment)->parent) )
	return NULL;
      else
	return EnvFind(TakeEnvironment(environment)->parent, symbol);
    }
  }
}

Object* BoundEnv(Object* environment, Object* symbol){
  if(environment == NULL)
    return NULL;

  else{

    if( Assoc(symbol, TakeEnvironment(environment)->name_list) )
      return environment;
    else{
      if( isMacroEnv(environment) || isMacroEnv(TakeEnvironment(environment)->parent) )
	return NULL;
      else
	return BoundEnv(TakeEnvironment(environment)->parent, symbol);
    }
  }
}

bool isMacroEnv(Object* env){
  return TakeEnvironment(env)->parent == NULL;
}

Object* getMacroEnv(Object* env){
  if( isMacroEnv(env) )
    return env;
  else
    return getMacroEnv(TakeEnvironment(env)->parent);
}

void AttachToEnv(Object* env, Object* symbol, Object* object){
  Object* old_head = TakeEnvironment(env)->name_list;
  Object* new_head = Cons(Cons(symbol, object), TakeEnvironment(env)->name_list);

  DecRefCount(old_head);
  IncRefCount(new_head);
  TakeEnvironment(env)->name_list = new_head;
}


/*
  Bool
*/

bool isFalse(Object* bool_object){
  return isBool(bool_object) && !TakeBool(bool_object)->val;
}

bool isTrue(Object* bool_object){
  return !isFalse(bool_object);
}

/*
  SpecialForm
*/

bool isFormOf(char name[], Object* sp){
  return isSpecialForm(sp) && !strcmp(name, TakeSpecialForm(sp)->name);
}
