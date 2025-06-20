%{
#include "minipascal.tab.h"
#include "ast.h"
#include <string>
void yyerror(const char *s);
%}

%option noyywrap

DIGIT       [0-9]
ALPHA       [a-zA-Z]
ID          ({ALPHA}|_)({ALPHA}|{DIGIT}|_)*
INT_NUM     0|[1-9]{DIGIT}*
REAL_NUM    {DIGIT}+\.{DIGIT}*([eE][-+]?{DIGIT}+)?|\.{DIGIT}+([eE][-+]?{DIGIT}+)?|{DIGIT}+[eE][-+]?{DIGIT}+
WHITESPACE  [ \t\n\r]
COMMENT     \{[^\}]*\}|\/\/[^\n]*

%%

{WHITESPACE}    { /* ignore whitespace */ }
{COMMENT}       { /* ignore comments */ }

"program"       { return PROGRAM; }
"var"           { return VAR; }
"integer"       { return INTEGER; }
"real"          { return REAL; }
"boolean"       { return BOOLEAN; }
"function"      { return FUNCTION; }
"procedure"     { return PROCEDURE; }
"begin"         { return BEGIN; }
"end"           { return END; }
"if"            { return IF; }
"then"          { return THEN; }
"else"          { return ELSE; }
"while"         { return WHILE; }
"do"            { return DO; }
"array"         { return ARRAY; }
"of"            { return OF; }
"div"           { return DIV; }
"not"           { return NOT; }
"or"            { return OR; }
"and"           { return AND; }
"true"          { return TRUE; }
"false"         { return FALSE; }

{INT_NUM}       { yylval.int_val = atoi(yytext); return INT_NUM; }
{REAL_NUM}      { yylval.real_val = atof(yytext); return REAL_NUM; }
{ID}            { yylval.string_val = strdup(yytext); return ID; }

"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return MULT; }
"/"             { return DIVIDE; }
"="             { return EQ; }
"<>"            { return NEQ; }
"<"             { return LT; }
"<="            { return LE; }
">"             { return GT; }
">="            { return GE; }
":="            { return ASSIGN; }
":"             { return COLON; }
";"             { return SEMICOLON; }
","             { return COMMA; }
"("             { return LPAREN; }
")"             { return RPAREN; }
"["             { return LBRACKET; }
"]"             { return RBRACKET; }
"."             { return DOT; }
".."            { return DOTDOT; }

.               { yyerror("Invalid character"); }

%%