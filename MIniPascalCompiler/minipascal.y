%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "symbol_table.h"
#include "semantic_analyzer.h"

extern int yylex();
extern int yyparse();
extern FILE *yyin;
extern char* yytext;
extern int yylineno;

SymbolTable symbolTable;
void yyerror(const char *s);
%}

%union {
    int int_val;
    double real_val;
    char* string_val;
    ASTNode* node;
}

%token PROGRAM VAR INTEGER REAL BOOLEAN FUNCTION PROCEDURE
%token BEGIN END IF THEN ELSE WHILE DO ARRAY OF
%token DIV NOT OR AND TRUE FALSE
%token PLUS MINUS MULT DIVIDE
%token EQ NEQ LT LE GT GE ASSIGN
%token COLON SEMICOLON COMMA LPAREN RPAREN LBRACKET RBRACKET DOT DOTDOT
%token <int_val> INT_NUM
%token <real_val> REAL_NUM
%token <string_val> ID

%type <node> program declarations subprogram_declarations subprogram_declaration
%type <node> compound_statement statement expression variable
%type <node> type standard_type subprogram_head arguments parameter_list
%type <node> identifier_list expression_list optional_statements statement_list
%type <node> procedure_statement unary_operator

%left OR
%left AND
%left NOT
%left EQ NEQ
%left LT LE GT GE
%left PLUS MINUS
%left MULT DIVIDE DIV
%left UMINUS

%%

program: PROGRAM ID SEMICOLON declarations subprogram_declarations compound_statement DOT
        { $$ = createProgramNode($2, $4, $5, $6); }
        ;

declarations: /* empty */
            | declarations VAR identifier_list COLON type SEMICOLON
            { $$ = createDeclarationsNode($1, $3, $5); }
            ;

type: standard_type
    | ARRAY LBRACKET INT_NUM DOTDOT INT_NUM RBRACKET OF standard_type
    { $$ = createArrayTypeNode($3, $5, $8); }
    ;

standard_type: INTEGER { $$ = createTypeNode("integer"); }
             | REAL { $$ = createTypeNode("real"); }
             | BOOLEAN { $$ = createTypeNode("boolean"); }
             ;

subprogram_declarations: /* empty */
                      | subprogram_declarations subprogram_declaration SEMICOLON
                      { $$ = createSubprogramDeclarationsNode($1, $2); }
                      ;

subprogram_declaration: subprogram_head compound_statement
                      { $$ = createSubprogramNode($1, $2); }
                      ;

subprogram_head: FUNCTION ID arguments COLON standard_type SEMICOLON
               { $$ = createFunctionHeadNode($2, $3, $5); }
               | PROCEDURE ID arguments SEMICOLON
               { $$ = createProcedureHeadNode($2, $3); }
               ;

arguments: /* empty */
         | LPAREN parameter_list RPAREN
         { $$ = $2; }
         ;

parameter_list: identifier_list COLON type
              { $$ = createParameterListNode($1, $3); }
              | parameter_list SEMICOLON identifier_list COLON type
              { $$ = appendParameterListNode($1, $3, $5); }
              ;

identifier_list: ID { $$ = createIdentifierListNode($1); }
               | identifier_list COMMA ID
               { $$ = appendIdentifierListNode($1, $3); }
               ;

compound_statement: BEGIN optional_statements END
                  { $$ = createCompoundStatementNode($2); }
                  ;

optional_statements: /* empty */
                  | statement_list
                  { $$ = $1; }
                  ;

statement_list: statement
              | statement_list SEMICOLON statement
              { $$ = appendStatementNode($1, $3); }
              ;

statement: variable ASSIGN expression
         { $$ = createAssignmentNode($1, $3); }
         | procedure_statement
         | compound_statement
         | IF expression THEN statement
         { $$ = createIfNode($2, $4, NULL); }
         | IF expression THEN statement ELSE statement
         { $$ = createIfNode($2, $4, $6); }
         | WHILE expression DO statement
         { $$ = createWhileNode($2, $4); }
         ;

variable: ID { $$ = createVariableNode($1, NULL); }
        | ID LBRACKET expression RBRACKET
        { $$ = createArrayAccessNode($1, $3); }
        ;

procedure_statement: ID
                   { $$ = createProcedureCallNode($1, NULL); }
                   | ID LPAREN expression_list RPAREN
                   { $$ = createProcedureCallNode($1, $3); }
                   ;

expression_list: expression
               | expression_list COMMA expression
               { $$ = appendExpressionListNode($1, $3); }
               ;

expression: INT_NUM { $$ = createIntNumNode($1); }
          | REAL_NUM { $$ = createRealNumNode($1); }
          | TRUE { $$ = createBooleanNode(1); }
          | FALSE { $$ = createBooleanNode(0); }
          | ID { $$ = createVariableNode($1, NULL); }
          | ID LPAREN expression_list RPAREN
          { $$ = createFunctionCallNode($1, $3); }
          | LPAREN expression RPAREN { $$ = $2; }
          | expression PLUS expression { $$ = createBinaryOpNode($1, $3, "+"); }
          | expression MINUS expression { $$ = createBinaryOpNode($1, $3, "-"); }
          | expression MULT expression { $$ = createBinaryOpNode($1, $3, "*"); }
          | expression DIVIDE expression { $$ = createBinaryOpNode($1, $3, "/"); }
          | expression DIV expression { $$ = createBinaryOpNode($1, $3, "div"); }
          | expression EQ expression { $$ = createBinaryOpNode($1, $3, "="); }
          | expression NEQ expression { $$ = createBinaryOpNode($1, $3, "<>"); }
          | expression LT expression { $$ = createBinaryOpNode($1, $3, "<"); }
          | expression LE expression { $$ = createBinaryOpNode($1, $3, "<="); }
          | expression GT expression { $$ = createBinaryOpNode($1, $3, ">"); }
          | expression GE expression { $$ = createBinaryOpNode($1, $3, ">="); }
          | expression AND expression { $$ = createBinaryOpNode($1, $3, "and"); }
          | expression OR expression { $$ = createBinaryOpNode($1, $3, "or"); }
          | MINUS expression %prec UMINUS { $$ = createUnaryOpNode($2, "-"); }
          | NOT expression { $$ = createUnaryOpNode($2, "not"); }
          ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error at line %d: %s\n", yylineno, s);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return 1;
    }
    
    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("Error opening file");
        return 1;
    }
    
    yyparse();
    fclose(yyin);
    
    return 0;
}