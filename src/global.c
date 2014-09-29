#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "object.h"

#include "garbage_collector.h"

void GLOBAL_init(){
  OBJECT_COUNT = 0;
  SYMBOL_COUNT = 0;
  MARK_AND_SWEEP_COUNT = 0;
  MARK_AND_SWEEP_ON = true;
}

/*
  Objects list (to mark-and-sweep GC)
*/

void OBJECTS_refresh(){
  int count = 0;
  for(int i = 0, j = 0; i < OBJECT_COUNT; i++){
    if(OBJECTS[i].exist)
      OBJECTS[j++] = OBJECTS[i];
    else
      count++;
  }
  OBJECT_COUNT -= count;
  //printf("*** %d objects rewinded ***\n", count);
}

void OBJECTS_remove(Object* object){
  for(int i = 0; i < OBJECT_COUNT; i++){
    if(OBJECTS[i].exist && OBJECTS[i].object == object){
      OBJECTS[i].exist = false;
      return;
    }
  }
  printf("ERROR, OBJECT NOT FOUND in remove\n");
  exit(0);
}

void OBJECTS_add(Object* object){
  if(OBJECT_COUNT == MAX_OBJECTS){
    OBJECTS_refresh();
    if(OBJECT_COUNT == MAX_OBJECTS){
      printf("Internal Error: object amount exceeded.\n");
      exit(0);
    }
  }
  OBJECTS[OBJECT_COUNT++] = (ObjectTag){true, object};
}

void OBJECTS_mark_reset(){
  for(int i = 0; i < OBJECT_COUNT; i++)
    if(OBJECTS[i].exist)
      OBJECTS[i].object->reachable = 0;
}

void OBJECTS_apply_to_unmarked(void (*func)(Object*)){
  for(int i = 0; i < OBJECT_COUNT; i++)
    if(OBJECTS[i].exist && OBJECTS[i].object->reachable == 0){
      //printf("  *** APPLY TO UNMARKED");
      //PrintInspect(OBJECTS[i].object);
      (*func)(OBJECTS[i].object);
    }
}

/*
  Symbol list (for symbol to be singleton)
*/

void SYMBOLS_add(Object* symbol){
  if(SYMBOL_COUNT == MAX_SYMBOLS){
    printf("Internal Error: symbol amount exceeded.\n");
    exit(0);
  }
  SYMBOLS[SYMBOL_COUNT++] = (ObjectTag){true, symbol};
}

void SYMBOLS_remove(Object* symbol){
  for(int i = 0; i < SYMBOL_COUNT; i++){
    if(SYMBOLS[i].exist && SYMBOLS[i].object == symbol){
      SYMBOLS[i].exist = false;
      return;
    }
  }
  printf("ERROR, SYMBOL NOT FOUND in remove\n");
  exit(0);
}

Object* SYMBOLS_find(char name[]){
  for(int i = 0; i < SYMBOL_COUNT; i++){
    if(SYMBOLS[i].exist && !strcmp(TakeSymbol(SYMBOLS[i].object)->name, name))
      return SYMBOLS[i].object;
  }
  return NULL;
}

/*
  Mark and sweep GC on/off control
*/

void MARK_AND_SWEEP_try(Object* object){
  if(!MARK_AND_SWEEP_ON)
    return;

  if(MARK_AND_SWEEP_COUNT++ == MARK_AND_SWEEP_INTERVAL){
    MARK_AND_SWEEP_COUNT = 0;
    MarkAndSweepFrom(object);
  }
}

void MARK_AND_SWEEP_on(){
  MARK_AND_SWEEP_ON = true;
}

void MARK_AND_SWEEP_off(){
  MARK_AND_SWEEP_ON = false;
}
