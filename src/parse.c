#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "object.h"
#include "object_lib.h"
#include "parse.h"

char SPECIAL_ID[] = {'!', '$', '%', '&', '*', '+', '-', '.',
		     '/', '<', '=', '>', '?', '@', '^', '_'};

Object* SafeParseExp(char exp_str[]){
  int begin = 0, end = strlen(exp_str);

  if( !CheckBracket(exp_str, begin, end, 0) )
    return NewCondition("Syntax Error: invalid bracket pattern.");

  Object* parsed = ParseExp(exp_str, begin, end, PMODE_NONE);
  Object* included_condition;
  if( (included_condition = ExtractCondition(parsed)) == NULL )
    return parsed;

  else
    return included_condition;
}

Object* ParseExp(char exp_str[], int begin, int end, ParseMode mode){
  if( begin >= end )
    return NULL;

  if( mode != PMODE_DOUBLE && mode != PMODE_SINGLE_SHARP_BSLASH && isSeparation(exp_str[begin]) )
    return ParseExp(exp_str, begin + 1, end, mode);

  if( mode != PMODE_DOUBLE && mode != PMODE_SINGLE_SHARP_BSLASH && exp_str[begin] == ';' )
    return ParseExp(exp_str, LineEndPos(exp_str, begin, end) + 1, end, mode);

  if( mode == PMODE_NONE && exp_str[begin] == '\'' )
    return ParseExp(exp_str, begin + 1, end, PMODE_SINGLE);

  if( mode == PMODE_NONE && exp_str[begin] == '"' )
    return ParseExp(exp_str, begin + 1, end, PMODE_DOUBLE);

  if( mode == PMODE_NONE && exp_str[begin] == '`' )
    return ParseExp(exp_str, begin + 1, end, PMODE_BACK);

  if( mode == PMODE_NONE && exp_str[begin] == ',' )
    return ParseExp(exp_str, begin + 1, end, PMODE_COMMA);

  if( mode == PMODE_COMMA && exp_str[begin] == '@' )
    return ParseExp(exp_str, begin + 1, end, PMODE_COMMA_AT);

  if( mode == PMODE_SINGLE && exp_str[begin] == '#' )
    return ParseExp(exp_str, begin + 1, end, PMODE_SINGLE_SHARP);

  if( mode == PMODE_COLLECT_SINGLE && exp_str[begin] == '\'' )
    return ParseExp(exp_str, begin + 1, end, PMODE_NONE);

  if( mode == PMODE_SINGLE_SHARP ){
    if( exp_str[begin] == 't' || exp_str[begin] == 'f' )
      return Cons(NewBool(exp_str[begin] == 't'),
		  ParseExp(exp_str, begin + 1, end, PMODE_COLLECT_SINGLE));
    if( exp_str[begin] == '\\' )
      return ParseExp(exp_str, begin + 1, end, PMODE_SINGLE_SHARP_BSLASH);
    else
      return NewCondition("Syntax Error: Invalid character after \"'#\".");
  }

  if( mode == PMODE_SINGLE_SHARP_BSLASH )
    return Cons(NewCharacter(exp_str[begin]), ParseExp(exp_str, begin + 1, end, PMODE_COLLECT_SINGLE));

  if( mode == PMODE_DOUBLE ){
    int str_end_pos = StringEndPos(exp_str, begin, end, false);
    if( str_end_pos < 0 )
      return NewCondition("Syntax Error: Invalid string literal.");
    else
      return Cons(Quoten(StringParse(exp_str, begin, str_end_pos), PMODE_SINGLE),
		  ParseExp(exp_str, str_end_pos + 1, end, PMODE_NONE));
  }

  if( exp_str[begin] == '(' ){
    int position = BracketNestOutPosition(exp_str, 1, begin + 1, end);
    return Cons(Quoten(ParseExp(exp_str, begin + 1, position - 1, PMODE_NONE), mode),
		ParseExp(exp_str, position, end, PMODE_NONE));
  }

  if( isIdent(exp_str[begin]) ){
    int position = WordEndPosition(exp_str, begin, end);
    return Cons(Quoten(ParseIdent(exp_str, begin, position), mode),
		ParseExp(exp_str, position, end, PMODE_NONE));
  }

  else
    return NewCondition("Syntax Error: Invalid token.");
}

int StringEndPos(char str[], int begin, int end, bool escaped){
  if( begin >= end )
    return -1;

  if(escaped)
    return StringEndPos(str, begin + 1, end, false);

  if( str[begin] == '"' )
    return begin;

  if( str[begin] == '\\' )
    return StringEndPos(str, begin + 1, end, true);
  
  else
    return StringEndPos(str, begin + 1, end, false);
}

Object* StringParse(char str[], int begin, int end){
  if( begin == end )
    return NULL;

  if( str[begin] == '\\' )
    return Cons(NewCharacter(str[begin + 1]), StringParse(str, begin + 2, end));

  else
    return Cons(NewCharacter(str[begin]), StringParse(str, begin + 1, end));
}

Object* ExtractCondition(Object* exp){
  if( isCondition(exp) )
    return exp;

  if( isConsCell(exp) ){
    Object* car = ExtractCondition(Car(exp));
    return car ? car : ExtractCondition(Cdr(exp));
  }
    
  else
    return NULL;
}

Object* Quoten(Object* exp, ParseMode mode){
  if(mode == PMODE_NONE)
    return exp;
  if(mode == PMODE_SINGLE)
    return Cons(GetSymbolAsSingleton("quote"), Cons(exp, NULL));
  if(mode == PMODE_BACK)
    return Cons(GetSymbolAsSingleton("quasiquote"), Cons(exp, NULL)); 
  if(mode == PMODE_COMMA)
    return Cons(GetSymbolAsSingleton("unquote"), Cons(exp, NULL)); 
  if(mode == PMODE_COMMA_AT)
    return Cons(GetSymbolAsSingleton("unquote-splicing"), Cons(exp, NULL)); 
  else
    return NewCondition("Syntax Error: Invalid quoting.");
}

int LineEndPos(char str[], int begin, int end){
  if( begin >= end )
    return end;

  if( str[begin] == '\n' || str[begin] == '\0' )
    return begin;

  else
    return LineEndPos(str, begin + 1, end);
}

Object* ParseIdent(char exp_str[], int begin, int end){
  char name_buf[101];
  
  for(int i = begin; i < end; i++)
    name_buf[i - begin] = exp_str[i];

  name_buf[end - begin] = '\0';
  
  if( hasOnlyDigit(name_buf) )
    return NewInteger(atoi(name_buf));

  else
    return GetSymbolAsSingleton(name_buf);
}

bool hasOnlyDigit(char str[]){
  for(int i = 0; str[i] != '\0'; i++)
    if(str[i] < '0' || '9' < str[i])
      return false;

  return true;
}

bool isIdent(char c){
  if( between(c, '0', '9') || between(c, 'A', 'Z') || between(c, 'a', 'z') )
    return true;
  
  else if(array_member(c, SPECIAL_ID, 16))
    return true;

  else 
    return false;
}

int BracketNestOutPosition(char str[], int nest_level, int begin_pos, int end_pos){
  if(nest_level == 0){
    return begin_pos;

  }else if(begin_pos == end_pos){
    return -1;

  }else if(str[begin_pos] == '('){
    return BracketNestOutPosition(str, nest_level + 1, begin_pos + 1, end_pos);

  }else if(str[begin_pos] == ')'){
    return BracketNestOutPosition(str, nest_level - 1, begin_pos + 1, end_pos);

  }else{
    return BracketNestOutPosition(str, nest_level, begin_pos + 1, end_pos);
  }
}

bool CheckBracket(char str[], int begin_pos, int end_pos, int count){
  if( begin_pos == end_pos )
    return count == 0;

  if( str[begin_pos] == '(')
    return CheckBracket(str, begin_pos + 1, end_pos, count + 1);

  if( str[begin_pos] == ')')
    return count == 0 ? false : CheckBracket(str, begin_pos + 1, end_pos, count - 1);

  else
    return CheckBracket(str, begin_pos + 1, end_pos, count);
}

int WordEndPosition(char str[], int begin_pos, int end_pos){
  if(begin_pos == end_pos){
    return end_pos;

  }else if( isSeparation(str[begin_pos]) ){
    return begin_pos;

  }else{
    return WordEndPosition(str, begin_pos + 1, end_pos);
  }
}

bool isSeparation(char c){
  return c == ' ' || c == '\n' || c == '\t';
}
	   
bool between(char c, char s, char t){
  return s <= c && c <= t;
}

bool array_member(char c, char array[], int len){
  for(int i = 0; i < len; i++)
    if(array[i] == c)
      return true;
  return false;
}

