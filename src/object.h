#include <stdbool.h>

#ifndef OBJECT_H
#define OBJECT_H


typedef enum TYPE TYPE;
typedef struct Object Object;
typedef struct ConsCell ConsCell;
typedef struct Symbol Symbol;
typedef struct Integer Integer;
typedef struct Bool Bool;
typedef struct Lambda Lambda;
typedef struct PrimitiveFunction PrimitiveFunction;
typedef struct Continuation Continuation;
typedef struct Environment Environment;
typedef struct SpecialForm SpecialForm;
typedef struct Condition Condition;
typedef struct Character Character;


enum TYPE{
  TYPE_ConsCell,
  TYPE_Symbol,
  TYPE_Integer,
  TYPE_Bool,
  TYPE_Lambda,
  TYPE_Quote,
  TYPE_If,
  TYPE_PrimitiveFunction,
  TYPE_Continuation,
  TYPE_Environment,
  TYPE_Garbage,
  TYPE_SpecialForm,
  TYPE_Condition,
  TYPE_Character
};

struct Object{
  Object* id;
  TYPE type;
  int ref_count;
  int reachable;
  bool garbage_flag;
  void* entity;
};

struct ConsCell{
  Object* car;
  Object* cdr;
};

struct Symbol{
  char name[101];
};

struct Integer{
  int i;
}; 

struct Bool{
  bool val;
}; 

struct Lambda{
  Object* environment;
  Object* params;
  Object* rest_param;
  Object* body;
}; 

struct PrimitiveFunction{
  Object* (*func)(Object*, Object*, Object*);
}; 

struct Continuation{
  Object* exp;
  Object* args;
  Object* environment;
  Object* next;
};

struct Environment{
  Object* parent;
  Object* name_list;
};

struct SpecialForm{
  char name[21];
};

struct Condition{
  char message[1024];
};

struct Character{
  int c;
}; 

Object* objects[1024];
int OBJ;


/*
  Object
*/
Object* NewObject(enum TYPE type, void* entity);

/*
  Nil
*/
bool isNil(Object* cell);

/*
  Cell
*/
Object* NewConsCell(Object* car, Object* cdr);
void ApplyToConsCellRef(Object* cell, void (*func)(Object*));
ConsCell* TakeConsCell(Object* cell);
bool isConsCell(Object* object);

/*
  Symbol
*/
Object* NewSymbol(char name[]);
void ApplyToSymbolRef(Object* symbol, void (*func)(Object*));
Symbol* TakeSymbol(Object* symbol);
bool isSymbol(Object* object);

/*
  Integer
*/
Object* NewInteger(int i);
void ApplyToIntegerRef(Object* integer, void (*func)(Object*));
Integer* TakeInteger(Object* integer);
bool isInteger(Object* object);

/*
  Lambda
*/
Object* NewLambda(Object* environment, Object* params, Object* rest_param, Object* body);
void ApplyToLambdaRef(Object* lambda, void (*func)(Object*));
Lambda* TakeLambda(Object* lambda);
bool isLambda(Object* object);

/*
  Primitive Func
*/
Object* NewPrimitiveFunc(Object* (*func)(Object*, Object*, Object*));
void ApplyToPrimitiveFuncRef(Object* primitive_func, void (*func)(Object*));
PrimitiveFunction* TakePrimitiveFunc(Object* primitive_func);
bool isPrimitiveFunc(Object* object);

/*
  Continuation
*/
Object* NewContinuation(Object* exp, Object* args, Object* environment, Object* next);
void ApplyToContinuationRef(Object* continuation, void (*func)(Object*));
Continuation* TakeContinuation(Object* continuation);
bool isContinuation(Object* object);

/*
  Environment
*/
Object* NewEnvironment(Object* parent, Object* name_list);
void ApplyToEnvironmentRef(Object* environment, void (*func)(Object*));
Environment* TakeEnvironment(Object* environment);
bool isEnvironment(Object* object);

/*
  Bool
*/
Object* NewBool(bool val);
void ApplyToBoolRef(Object* bool_object, void (*func)(Object*));
Bool* TakeBool(Object* bool_object);
bool isBool(Object* object);

/*
  Special Form
*/

Object* NewSpecialForm(char name[]);
void ApplyToSpecialFormRef(Object* sp, void (*func)(Object*));
SpecialForm* TakeSpecialForm(Object* sp);
bool isSpecialForm(Object* object);

/*
  Condition
*/

Object* NewCondition(char message[]);
void ApplyToConditionRef(Object* Condition, void (*func)(Object*));
Condition* TakeCondition(Object* Condition);
bool isCondition(Object* object);

/*
  Character
*/
Object* NewCharacter(int c);
void ApplyToCharacterRef(Object* character, void (*func)(Object*));
Character* TakeCharacter(Object* character);
bool isCharacter(Object* object);


#endif
