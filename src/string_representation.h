#include "object.h"

#ifndef STRING_REPRESENTATION_H
#define STRING_REPRESENTATION_H

void PrintDesc(Object* object);
void PrintInspect(Object* object);

int Describe(char* str, int len, Object* object);
int DescribeNil(char* str, int len, Object* null);
int DescribeConsCell(char* str, int len, Object* cell);
int DescribeSymbol(char* str, int len, Object* symbol);
int DescribeInteger(char* str, int len, Object* integer);
int DescribeLambda(char* str, int len, Object* lambda);
int DescribePrimitiveFunc(char* str, int len, Object* primitive_func);
int DescribeContinuation(char* str, int len, Object* continuation);
int DescribeEnvironment(char* str, int len, Object* environment);
int DescribeBool(char* str, int len, Object* bool_object);
int DescribeSpecialForm(char* str, int len, Object* sp);
int DescribeCondition(char* str, int len, Object* condition);
int DescribeCharacter(char* str, int len, Object* character);
int DescribeUnknown(char* str, int len, Object* object);
int DescribeString(char* str, int len, Object* str_list);

int Inspect(char* str, int len, Object* object);
int InspectNil(char* str, int len, Object* null);
int InspectConsCell(char* str, int len, Object* cell);
int InspectSymbol(char* str, int len, Object* symbol);
int InspectInteger(char* str, int len, Object* integer);
int InspectLambda(char* str, int len, Object* lambda);
int InspectPrimitiveFunc(char* str, int len, Object* primitive_func);
int InspectContinuation(char* str, int len, Object* continuation);
int InspectEnvironment(char* str, int len, Object* environment);
int InspectBool(char* str, int len, Object* bool_object);
int InspectSpecialForm(char* str, int len, Object* sp);
int InspectCondition(char* str, int len, Object* condition);
int InspectCharacter(char* str, int len, Object* character);
int InspectUnknown(char* str, int len, Object* object);

#endif
