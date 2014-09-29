#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "object.h"
#include "garbage_collector.h"
#include "global.h"

/*
  Object
*/

Object* NewObject(TYPE type, void* entity){
  Object* object = (Object*)malloc(sizeof(Object));
  object->id = object;
  object->type = type;
  object->ref_count = 0;
  //object->garbage_flag = false;
  object->reachable = 0;
  object->entity = entity;
  OBJECTS_add(object);
  ApplyToRef(object, IncRefCount);
  return object;
}

/*
  Nil
*/

bool isNil(Object* object){
  return object == NULL;
}

/*
  Cell
*/

Object* NewConsCell(Object* car, Object* cdr){
  ConsCell* new_cell = malloc(sizeof(ConsCell));
  new_cell->car = car;
  new_cell->cdr = cdr;
  return NewObject(TYPE_ConsCell, new_cell);
}

void ApplyToConsCellRef(Object* cell, void (*func)(Object*)){
  assert(isConsCell(cell));
  ConsCell* entity = TakeConsCell(cell);
  (*func)(entity->car);
  (*func)(entity->cdr);
}

ConsCell* TakeConsCell(Object* cell){
  assert(isConsCell(cell));
  return (ConsCell*)(cell->entity);
}

bool isConsCell(Object* object){
  return !isNil(object) && object->type == TYPE_ConsCell;
}

/*
  Symbol
*/

Object* NewSymbol(char name[]){
  Symbol* new_symbol = malloc(sizeof(Symbol));
  strcpy(new_symbol->name, name);
  return NewObject(TYPE_Symbol, new_symbol);
}

void ApplyToSymbolRef(Object* symbol, void (*func)(Object*)){
  assert(isSymbol(symbol));
}

Symbol* TakeSymbol(Object* symbol){
  assert(isSymbol(symbol));
  return (Symbol*)(symbol->entity);
}

bool isSymbol(Object* object){
  return !isNil(object) && object->type == TYPE_Symbol;
}

/*
  Integer
*/

Object* NewInteger(int i){
  Integer* new_integer = malloc(sizeof(Integer));
  new_integer->i = i;
  return NewObject(TYPE_Integer, new_integer);
}

void ApplyToIntegerRef(Object* integer, void (*func)(Object*)){
  assert(isInteger(integer));
}

Integer* TakeInteger(Object* integer){
  assert(isInteger(integer));
  return (Integer*)(integer->entity);
}

bool isInteger(Object* object){
  return !isNil(object) && object->type == TYPE_Integer;
}

/*
  Lambda
*/

Object* NewLambda(Object* environment, Object* params, Object* rest_param, Object* body){
  Lambda* new_lambda = malloc(sizeof(Lambda));
  new_lambda->environment = environment;
  new_lambda->params      = params;
  new_lambda->rest_param  = rest_param;
  new_lambda->body        = body;
  return NewObject(TYPE_Lambda, new_lambda);
}

void ApplyToLambdaRef(Object* lambda, void (*func)(Object*)){
  assert(isLambda(lambda));
  Lambda* entity = TakeLambda(lambda);
  (*func)(entity->environment);
  (*func)(entity->params);
  (*func)(entity->rest_param);
  (*func)(entity->body);
}

Lambda* TakeLambda(Object* lambda){
  assert(isLambda(lambda));
  return (Lambda*)(lambda->entity);
}

bool isLambda(Object* object){
  return !isNil(object) && object->type == TYPE_Lambda;
}

/*
  Primitive Func
*/

Object* NewPrimitiveFunc(Object* (*func)(Object*, Object*, Object*)){
  PrimitiveFunction* new_pfunc = malloc(sizeof(PrimitiveFunction));
  new_pfunc->func = func;
  return NewObject(TYPE_PrimitiveFunction, new_pfunc);
}

void ApplyToPrimitiveFuncRef(Object* primitive_func, void (*func)(Object*)){
  assert(isPrimitiveFunc(primitive_func));
}

PrimitiveFunction* TakePrimitiveFunc(Object* primitive_func){
  assert(isPrimitiveFunc(primitive_func));
  return (PrimitiveFunction*)(primitive_func->entity);
}

bool isPrimitiveFunc(Object* object){
  return !isNil(object) && object->type == TYPE_PrimitiveFunction;
}

/*
  Continuation
*/

Object* NewContinuation(Object* exp, Object* args, Object* environment, Object* next){
  Continuation* new_continuation = (Continuation*)malloc(sizeof(Continuation));
  new_continuation->exp         = exp;
  new_continuation->args        = args;
  new_continuation->environment = environment;
  new_continuation->next        = next;
  return NewObject(TYPE_Continuation, new_continuation);
}

void ApplyToContinuationRef(Object* continuation, void (*func)(Object*)){
  assert(isContinuation(continuation));
  Continuation* entity = TakeContinuation(continuation);
  (*func)(entity->exp);
  (*func)(entity->args);
  (*func)(entity->environment);
  (*func)(entity->next);
}

Continuation* TakeContinuation(Object* continuation){
  assert(isContinuation(continuation));
  return (Continuation*)(continuation->entity);
}

bool isContinuation(Object* object){
  return !isNil(object) && object->type == TYPE_Continuation;
}

/*
  Environment
*/

Object* NewEnvironment(Object* parent, Object* name_list){
  Environment* new_environment = malloc(sizeof(Environment));
  new_environment->parent = parent;
  new_environment->name_list = name_list;
  return NewObject(TYPE_Environment, new_environment);
}

void ApplyToEnvironmentRef(Object* environment, void (*func)(Object*)){
  assert(isEnvironment(environment));
  Environment* entity = TakeEnvironment(environment);
  (*func)(entity->parent);
  (*func)(entity->name_list);
}

Environment* TakeEnvironment(Object* environment){
  assert(isEnvironment(environment));
  return (Environment*)(environment->entity);
}

bool isEnvironment(Object* object){
  return !isNil(object) && object->type == TYPE_Environment;
}

/*
  Bool
*/

Object* NewBool(bool val){
  Bool* new_bool = malloc(sizeof(Bool));
  new_bool->val = val;
  return NewObject(TYPE_Bool, new_bool);
}

void ApplyToBoolRef(Object* bool_object, void (*func)(Object*)){
  assert(isBool(bool_object));
}

Bool* TakeBool(Object* bool_object){
  assert(isBool(bool_object));
  return (Bool*)(bool_object->entity);
}

bool isBool(Object* object){
  return !isNil(object) && object->type == TYPE_Bool;
}

/*
  Special Form
*/

Object* NewSpecialForm(char name[]){
  Symbol* new_sp = malloc(sizeof(SpecialForm));
  strcpy(new_sp->name, name);
  return NewObject(TYPE_SpecialForm, new_sp);
}

void ApplyToSpecialFormRef(Object* sp, void (*func)(Object*)){
  assert(isSpecialForm(sp));
}

SpecialForm* TakeSpecialForm(Object* sp){
  assert(isSpecialForm(sp));
  return (SpecialForm*)(sp->entity);
}

bool isSpecialForm(Object* object){
  return !isNil(object) && object->type == TYPE_SpecialForm;
}

/*
  Condition
*/

Object* NewCondition(char message[]){
  Condition* new_condition = malloc(sizeof(Condition));
  strcpy(new_condition->message, message);
  return NewObject(TYPE_Condition, new_condition);
}

void ApplyToConditionRef(Object* condition, void (*func)(Object*)){
  assert(isCondition(condition));
}

Condition* TakeCondition(Object* condition){
  assert(isCondition(condition));
  return (Condition*)(condition->entity);
}

bool isCondition(Object* object){
  return !isNil(object) && object->type == TYPE_Condition;
}

/*
  Character
*/

Object* NewCharacter(int c){
  Character* new_character = malloc(sizeof(Character));
  new_character->c = c;
  return NewObject(TYPE_Character, new_character);
}

void ApplyToCharacterRef(Object* character, void (*func)(Object*)){
  assert(isCharacter(character));
}

Character* TakeCharacter(Object* character){
  assert(isCharacter(character));
  return (Character*)(character->entity);
}

bool isCharacter(Object* object){
  return !isNil(object) && object->type == TYPE_Character;
}
