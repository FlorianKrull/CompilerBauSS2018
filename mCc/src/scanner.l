%option prefix="mCc_parser_"

%option batch
%option bison-bridge
%option noinput
%option nounput
%option noyywrap
%option reentrant

%{
#include "parser.tab.h"

#define YYSTYPE MCC_PARSER_STYPE
%}

alpha [a-zA-Z]
alpha_num {alpha} ([0-9]*{alpha}*)
int_literal   -?[0-9]+
float_literal -?[0-9]+\.[0-9]+
bool_literal	"true"|"false"
string_literal \"(\\.|[^\\])*\"
identfier [a-zA-Z]+[0-9a-zA-Z]*
whitespace [\r\t\v\f]
newline \n

int_type int
float_type float
bool_type bool
string_type string
void_type void

if_identifier if
else_identifier else
return_identifier return
while_identifier while


%%

{int_type}          {return TK_INT_TYPE;}

{float_type}        {return TK_FLOAT_TYPE;}

{bool_type}         {return TK_BOOL_TYPE;}

{string_type}       {return TK_STRING_TYPE;}

{void_type}         {return TK_VOID_TYPE;}

{if_identifier}     {return TK_IF_IDENTIFIER;}

{else_identifier}   {return TK_ELSE_IDENTIFIER;}

{return_identifier} {return TK_RETURN_IDENTIFIER;}

{while_identifier}  {return TK_WHILE_IDENTIFIER;}  

{int_literal}       { yylval->TK_INT_LITERAL = atol(yytext); return TK_INT_LITERAL; }

{float_literal}     { yylval->TK_FLOAT_LITERAL = atof(yytext); return TK_FLOAT_LITERAL; }

{bool_literal}	    { yylval->TK_BOOL_LITERAL = strcmp(yytext,"false"); return TK_BOOL_LITERAL; }

{string_literal}    {yylval->TK_STRING = strncpy(yytext[1],strlen(yytext)-2);return TK_STRING_LITERAL;)}





"+"							    { return TK_PLUS; }
"-"								{ return TK_MINUS; }
"*"								{ return TK_ASTER; }
"/"								{ return TK_SLASH; }
">"								{ return TK_GREATER; }
"<"								{ return TK_SMALLER; }
">="							{ return TK_GREATER_EQUAL; }
"<="							{ return TK_SMALLER_EQUAL; }
"&&"							{ return TK_AND; }
"||"							{ return TK_OR; }
"=="							{ return TK_EQUAL; }
"!="							{ return TK_UNEQUAL; }

"("                             { return TK_LPARENTH; }
")"                             { return TK_RPARENTH; }

"!"                             { return TK_EXCLAM; }


[ \t\r\n]+        { /* ignore */ }

<<EOF>>           { return TK_END; }

.                 { fprintf(stderr, "invalid character '%c'\n", yytext[0]); }
