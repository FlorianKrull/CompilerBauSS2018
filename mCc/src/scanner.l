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

alpha			[a-zA-Z_]
alpha_num		[a-zA-Z0-9_]
digit			[0-9]
identifier		[a-zA-Z_][a-zA-Z0-9_]*
int_literal   -?[0-9]+
float_literal -?[0-9]+\.[0-9]+
bool_literal	"true"|"false"


%%

{int_literal}     { yylval->TK_INT_LITERAL = atol(yytext); return TK_INT_LITERAL; }

{float_literal}   { yylval->TK_FLOAT_LITERAL = atof(yytext); return TK_FLOAT_LITERAL; }

{bool_literal}	  { yylval->TK_BOOL_LITERAL = strcmp(yytext,"false"); return TK_BOOL_LITERAL; }

{identifier}	{yylval->TK_IDENTIFIER = strdup(yytext);	return TK_IDENTIFIER;}

{alpha}			{yylval->TK_ALPHA = strdup(yytext);	return TK_ALPHA;}

{alpha_num}		{yylval->TK_ALPHA_NUM = strdup(yytext);	return TK_ALPHA_NUM;}

{digit}			{yylval->TK_DIGIT = atoi(yytext); return TK_DIGIT;}


"+"							  { return TK_PLUS; }
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

"("               { return TK_LPARENTH; }
")"               { return TK_RPARENTH; }

";"				{return TK_SEMICOLON; }
"{"				{return TK_LBRACKET; }
"}"				{return TK_RBRACKET; }
"if"			{return TK_IF; }
"else"			{return TK_ELSE; }
"while"			{return TK_WHILE; }
"return"		{return TK_RETURN; }

[ \t\r\n]+        { /* ignore */ }

<<EOF>>           { return TK_END; }

.                 { fprintf(stderr, "invalid character '%c'\n", yytext[0]); }
