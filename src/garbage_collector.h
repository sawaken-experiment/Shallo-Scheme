#include <stdio.h> //for dubug
#include <stdlib.h>
#include "object.h"
#include "string_representation.h" //for debug

#ifndef GARBAGE_COLLECTOR_H
#define GARBAGE_COLLECTOR_H

void ApplyToRef(Object* object, void (*func)(Object*));
void FreeEntity(Object* object);
void Release(Object* object);
void DecRefCount(Object* object);
void IncRefCount(Object* object);
void ReserveAsGarbage(Object* object);
bool isReservedAsGarbage(Object* object);
void Die_for_debug(Object* object);
void MarkAndSweepFrom(Object* object);
void MarkFrom(Object* object);

#endif
