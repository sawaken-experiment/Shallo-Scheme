#include <stdio.h>
#include "string_representation.h"
#include "object.h"
#include "object_lib.h"


void PrintDesc(Object* object){
  char buf[1024] = {'\0'};
  Describe(buf, 1024, object);
  printf("%s\n", buf);
}

void PrintInspect(Object* object){
  char buf[1024] = {'\0'};
  Inspect(buf, 1024, object);
  printf("%s\n", buf);
}

/*
  Describe
*/

int Describe(char* str, int len, Object* object){
  if( isNil(object) )
    return DescribeNil(str, len, object);

  if( isConsCell(object) )
    return DescribeConsCell(str, len, object);

  if( isSymbol(object) )
    return DescribeSymbol(str, len, object);

  if( isInteger(object) )
    return DescribeInteger(str, len, object);

  if( isLambda(object) )
    return DescribeLambda(str, len, object);

  if( isPrimitiveFunc(object) )
    return DescribePrimitiveFunc(str, len, object);

  if( isContinuation(object) )
    return DescribeContinuation(str, len, object);

  if( isEnvironment(object) )
    return DescribeEnvironment(str, len, object);

  if( isBool(object) )
    return DescribeBool(str, len, object);

  if( isSpecialForm(object) )
    return DescribeSpecialForm(str, len, object);

  if( isCondition(object) )
    return DescribeCondition(str, len, object);

  if( isCharacter(object) )
    return DescribeCharacter(str, len, object);

  else
    return DescribeUnknown(str, len, object);
}

int DescribeNil(char* str, int len, Object* null){
  return snprintf(str, len, "()");
}

int DescribeConsCell(char* str, int len, Object* cell){
  char car_buf[512] = {'\0'};
  char cdr_buf[512] = {'\0'};
  Describe(car_buf, 512, Car(cell));
  Describe(cdr_buf, 512, Cdr(cell));

  if( !isNil(cell) && isString(cell) )
    return DescribeString(str, len, cell);

  if(isConsCell(Cdr(cell)))
    return snprintf(str, len, "(%s %s", car_buf, &cdr_buf[1]);
  
  if(isNil(Cdr(cell)))
    return snprintf(str, len, "(%s)", car_buf);
  
  else
    return snprintf(str, len, "(%s . %s)", car_buf, cdr_buf);
}

int DescribeSymbol(char* str, int len, Object* symbol){
  return snprintf(str, len, "%s", TakeSymbol(symbol)->name);
}

int DescribeInteger(char* str, int len, Object* integer){
  return snprintf(str, len, "%d", TakeInteger(integer)->i);
}

int DescribeLambda(char* str, int len, Object* lambda){
  //return snprintf(str, len, "#<closure>");
  char params[128]; Describe(params, 128, TakeLambda(lambda)->params);
  char body[128];   Describe(body, 128, TakeLambda(lambda)->body);
  return snprintf(str, len, "#<closure:%lld params=%s body=%s>", IdToInt(lambda->id), params, body);
}

int DescribePrimitiveFunc(char* str, int len, Object* primitive_func){
  return snprintf(str, len, "#<primitive-func>");
}

int DescribeContinuation(char* str, int len, Object* continuation){
  return snprintf(str, len, "#<continuation>");
}

int DescribeEnvironment(char* str, int len, Object* environment){
  return snprintf(str, len, "#<environment>");
}

int DescribeBool(char* str, int len, Object* bool_object){
  return snprintf(str, len, "#%c", TakeBool(bool_object)->val ? 't' : 'f');
}

int DescribeSpecialForm(char* str, int len, Object* sp){
  return snprintf(str, len, "#<%s>", TakeSpecialForm(sp)->name);
}

int DescribeCondition(char* str, int len, Object* condition){
  return snprintf(str, len, "#<condition \"%s\">", TakeCondition(condition)->message);
}

int DescribeCharacter(char* str, int len, Object* character){
  return snprintf(str, len, "#\\%c", TakeCharacter(character)->c);
}

int DescribeUnknown(char* str, int len, Object* object){
  return snprintf(str, len, "#<unknown>");
}

// utility
int DescribeString(char* str, int len, Object* str_list){
  char tail[1024] = {'\0'};
  if( !isNil(Cdr(str_list)) )
    DescribeString(tail, 1024, Cdr(str_list));
  return snprintf(str, len, "%c%s", TakeCharacter(Car(str_list))->c, tail);
}

/*
  Inspect
*/

int Inspect(char* str, int len, Object* object){
  if( isNil(object) )
    return InspectNil(str, len, object);

  if( isConsCell(object) )
    return InspectConsCell(str, len, object);

  if( isSymbol(object) )
    return InspectSymbol(str, len, object);

  if( isInteger(object) )
    return InspectInteger(str, len, object);

  if( isLambda(object) )
    return InspectLambda(str, len, object);

  if( isPrimitiveFunc(object) )
    return InspectPrimitiveFunc(str, len, object);

  if( isContinuation(object) )
    return InspectContinuation(str, len, object);

  if( isEnvironment(object) )
    return InspectEnvironment(str, len, object);

  if( isBool(object) )
    return InspectBool(str, len, object);

  if( isSpecialForm(object) )
    return InspectSpecialForm(str, len, object);

  if( isCondition(object) )
    return InspectCondition(str, len, object);

  if( isCharacter(object) )
    return InspectCharacter(str, len, object);

  else
    return InspectUnknown(str, len, object);
}

int InspectNil(char* str, int len, Object* null){
  return snprintf(str, len, "#<nil>");
}

int InspectConsCell(char* str, int len, Object* cell){
  char car_buf[512] = {'\0'}, cdr_buf[512] = {'\0'};
  Inspect(car_buf, 512, Car(cell));
  Inspect(cdr_buf, 512, Cdr(cell));
  return snprintf(str, len, "#<cell:%lld, car=%s, cdr=%s>", IdToInt(cell->id), car_buf, cdr_buf);

}

int InspectSymbol(char* str, int len, Object* symbol){
  return snprintf(str, len, "#<symbol:%lld, %s>", IdToInt(symbol->id), TakeSymbol(symbol)->name);
}

int InspectInteger(char* str, int len, Object* integer){
  return snprintf(str, len, "#<integer:%lld, %d>", IdToInt(integer->id), TakeInteger(integer)->i);
}

int InspectLambda(char* str, int len, Object* lambda){
  char params[128]; Describe(params, 128, TakeLambda(lambda)->params);
  char body[128];   Describe(body, 128, TakeLambda(lambda)->body);
  return snprintf(str, len, "#<closure:%lld params=%s body=%s>", IdToInt(lambda->id), params, body);
}

int InspectPrimitiveFunc(char* str, int len, Object* primitive_func){
  return snprintf(str, len, "#<primitive-func:%lld>", IdToInt(primitive_func->id));
}

int InspectContinuation(char* str, int len, Object* continuation){
  Continuation* cont = TakeContinuation(continuation);
  char exp[128]  = {'\0'}; Describe(exp,  128, cont->exp);
  char args[128] = {'\0'}; Describe(args, 128, cont->args);
  char env[128]  = {'\0'}; Describe(env,  128, cont->environment);
  return snprintf(str, len, "#<continuation:%lld exp=%s, args=%s, env=%s, next_id=%lld>",
		  IdToInt(continuation->id), exp, args, env, isNil(cont->next) ? 0 : IdToInt(cont->next->id));
}

int InspectEnvironment(char* str, int len, Object* environment){
  char alist[512] = {'\0'}; Describe(alist, 512, TakeEnvironment(environment)->name_list);
  return snprintf(str, len, "#<environment:%lld, bound=%s>",
		  IdToInt(environment->id), alist);
}

int InspectBool(char* str, int len, Object* bool_object){
  return snprintf(str, len, "#<bool:%lld, %c>", IdToInt(bool_object->id), TakeBool(bool_object)->val ? 't' : 'f');
}

int InspectSpecialForm(char* str, int len, Object* sp){
  return snprintf(str, len, "#<%s:%lld>", TakeSpecialForm(sp)->name, IdToInt(sp->id));
}

int InspectCondition(char* str, int len, Object* condition){
  return snprintf(str, len, "#<condition:%lld, %s>", IdToInt(condition->id),
		  TakeCondition(condition)->message);
}

int InspectCharacter(char* str, int len, Object* character){
  return snprintf(str, len, "#<character:%lld, '%c'>", IdToInt(character->id),
		  TakeCharacter(character)->c);
}

int InspectUnknown(char* str, int len, Object* object){
  return snprintf(str, len, "#<unknown:%lld>", IdToInt(object->id));
}
