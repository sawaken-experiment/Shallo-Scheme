#include <stdio.h>
#include <stdlib.h>
#include "garbage_collector.h"
#include "object.h"
#include "global.h"
#include "string_representation.h"

void ApplyToRef(Object* object, void (*func)(Object*)){
  if( isConsCell(object) )
    ApplyToConsCellRef(object, func);

  else if( isSymbol(object) )
    ApplyToSymbolRef(object, func);

  else if( isInteger(object) )
    ApplyToIntegerRef(object, func);

  else if( isLambda(object) )
    ApplyToLambdaRef(object, func);

  else if( isPrimitiveFunc(object) )
    ApplyToPrimitiveFuncRef(object, func);

  else if( isContinuation(object) )
    ApplyToContinuationRef(object, func);

  else if( isEnvironment(object) )
    ApplyToEnvironmentRef(object, func);

  else if( isBool(object) )
    ApplyToBoolRef(object, func);

  else if( isSpecialForm(object) )
    ApplyToSpecialFormRef(object, func);

  else if( isCondition(object) )
    ApplyToConditionRef(object, func);

  else if( isCharacter(object) )
    ApplyToCharacterRef(object, func);

  else{
    printf("Internal Error: cannnot apply to reference of ");
    PrintInspect(object);
    exit(0);
  }
}

void FreeEntity(Object* object){
  if( isConsCell(object) )
    free(TakeConsCell(object));

  else if( isSymbol(object) )
    free(TakeSymbol(object));

  else if( isInteger(object) )
    free(TakeInteger(object));

  else if( isLambda(object) )
    free(TakeLambda(object));

  else if( isPrimitiveFunc(object) )
    free(TakePrimitiveFunc(object));

  else if( isContinuation(object) )
    free(TakeContinuation(object));

  else if( isEnvironment(object) )
    free(TakeEnvironment(object));

  else if( isBool(object) )
    free(TakeBool(object));

  else if( isSpecialForm(object) )
    free(TakeSpecialForm(object));

  else if( isCondition(object) )
    free(TakeCondition(object));

  else if( isCharacter(object) )
    free(TakeCharacter(object));

  else{
    printf("Internal Error: cannnot free entity of ");
    PrintInspect(object);
    exit(0);
  }
}
    
void Release(Object* object){
  if( isNil(object) )
    return;

  ApplyToRef(object, DecRefCount);

  OBJECTS_remove(object);

  if( isSymbol(object) )
    SYMBOLS_remove(object);

  FreeEntity(object);
  free(object);
}

void DecRefCount(Object* object){
  if(isNil(object))
    return;
    
  if(--object->ref_count == 0)
    Release(object);
}

void IncRefCount(Object* object){
  if(isNil(object))
    return;

  object->ref_count += 1;
}

void MarkAndSweepFrom(Object* object){
  OBJECTS_mark_reset();
  MarkFrom(object);
  OBJECTS_apply_to_unmarked(Release);
}

void MarkFrom(Object* object){
  if(isNil(object) || object->reachable == 1)
    return;

  object->reachable = 1;
  ApplyToRef(object, MarkFrom);
}
