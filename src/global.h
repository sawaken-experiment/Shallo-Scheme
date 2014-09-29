#include "object.h"

#ifndef GLOBAL_H
#define GLOBAL_H


#define MAX_OBJECTS 1000
#define MAX_SYMBOLS 1000
#define MARK_AND_SWEEP_INTERVAL 1

typedef struct ObjectTag ObjectTag;
struct ObjectTag{
  bool exist;
  Object* object;
};

int OBJECT_COUNT;
ObjectTag OBJECTS[MAX_OBJECTS];

int SYMBOL_COUNT;
ObjectTag SYMBOLS[MAX_SYMBOLS];

int MARK_AND_SWEEP_COUNT;
bool MARK_AND_SWEEP_ON;

void GLOBAL_init();

void OBJECTS_remove(Object* object);
void OBJECTS_add(Object* object);
void OBJECTS_mark_reset();
void OBJECTS_apply_to_unmarked(void (*func)(Object*));

void SYMBOLS_add(Object* symbol);
void SYMBOLS_remove(Object* symbol);
Object* SYMBOLS_find(char name[]);

void MARK_AND_SWEEP_try(Object* object);
void MARK_AND_SWEEP_on();
void MARK_AND_SWEEP_off();

#endif
