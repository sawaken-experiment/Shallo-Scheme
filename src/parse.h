#ifndef PARSE_H
#define PARSE_H

typedef enum ParseMode ParseMode;

enum ParseMode{
  PMODE_NONE,
  PMODE_SINGLE,
  PMODE_BACK,
  PMODE_COMMA,
  PMODE_COMMA_AT,
  PMODE_DOUBLE,
  PMODE_SINGLE_SHARP,
  PMODE_COLLECT_SINGLE,
  PMODE_SINGLE_SHARP_BSLASH
};



Object* SafeParseExp(char exp_str[]);
Object* ParseExp(char exp_str[], int begin, int end, ParseMode mode);
int StringEndPos(char str[], int begin, int end, bool escaped);
Object* StringParse(char str[], int begin, int end);
Object* ParseIdent(char exp_str[], int begin, int end);
Object* ExtractCondition(Object* exp);
Object* Quoten(Object* exp, ParseMode mode);
int LineEndPos(char str[], int begin, int end);
bool hasOnlyDigit(char str[]);
bool isIdent(char c);
bool isSeparation(char c);
bool between(char c, char s, char t);
bool array_member(char c, char array[], int len);

bool CheckBracket(char str[], int begin_pos, int end_pos, int count);
int BracketNestOutPosition(char str[], int nest_level, int begin_pos, int end_pos);
int WordEndPosition(char str[], int begin_pos, int end_pos);

#endif
