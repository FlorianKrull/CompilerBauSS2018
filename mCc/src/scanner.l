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

identifier		[a-zA-Z_][a-zA-Z0-9_]*
int_literal     [0-9]+
float_literal   [0-9]+\.[0-9]+
bool_literal	"true"|"false"
string_literal \"(\\.|[^\\])*\"
whitespace [\r\t\v\f]
newline \n

%%

"bool"            { return TK_BOOL_TYPE; }
"int"             { return TK_INT_TYPE; }
"float"           { return TK_FLOAT_TYPE; }
"string"          { return TK_STRING_TYPE; }
"void"          { return TK_VOID_TYPE; }

"if"              { return TK_IF; }
"else"            { return TK_ELSE; }
"while"            { return TK_WHILE; }
"return"            { return TK_RETURN; }

{int_literal}     { yylval->TK_INT_LITERAL = atol(yytext); return TK_INT_LITERAL; }

{float_literal}   { yylval->TK_FLOAT_LITERAL = atof(yytext); return TK_FLOAT_LITERAL; }

{bool_literal}	  { yylval->TK_BOOL_LITERAL = strcmp(yytext,"false"); return TK_BOOL_LITERAL; }

{string_literal}    {yylval->TK_STRING_LITERAL = strdup(yytext) ;return TK_STRING_LITERAL;}

{identifier}	{yylval->TK_IDENTIFIER = strdup(yytext);	return TK_IDENTIFIER;}

"+"		{ return TK_PLUS; }
"-"		{ return TK_MINUS; }
"*"		{ return TK_ASTER; }
"/"		{ return TK_SLASH; }
">"		{ return TK_GREATER; }
"<"		{ return TK_SMALLER; }
">="		{ return TK_GREATER_EQUAL; }
"<="		{ return TK_SMALLER_EQUAL; }
"&&"		{ return TK_AND; }
"||"		{ return TK_OR; }
"=="		{ return TK_EQUAL; }
"!="		{ return TK_UNEQUAL; }
"!"             { return TK_EXCLAM; }
"="               { return TK_ASSIGN; }

"("     { return TK_LPARENTH; }
")"     { return TK_RPARENTH; }
","		{ return TK_COMMA; }
";"		{return TK_SEMICOLON; }
"{"		{return TK_LBRACKET; }
"}"		{return TK_RBRACKET; }
"["		{ return TK_LSQUARE_BRACKET; }
"]"		{ return TK_RSQUARE_BRACKET; }

[ \t\r\n]+        { /* ignore */ }

<<EOF>>           { return TK_END; }

.                 { fprintf(stderr, "invalid character '%c'\n", yytext[0]); }
