%define api.prefix {mCc_parser_}

%define api.pure full
%lex-param   {void *scanner}
%parse-param {void *scanner} {struct mCc_ast_expression** result}

%define parse.trace
%define parse.error verbose

%code requires {
#include "mCc/parser.h"
}

%{
#include <string.h>

int mCc_parser_lex();
void mCc_parser_error();
%}

%define api.value.type union
%define api.token.prefix {TK_}




%token END 0 "EOF"

%token <char>	ALPHA		"alpha"
%token <char>	ALPHA_NUM	"alpha or number"
%token <int>	DIGIT		"digit number"
%token <char*> IDENTIFIER "identifier"
%token <long>   INT_LITERAL   "integer literal"
%token <double> FLOAT_LITERAL "float literal"
%token <bool>		BOOL_LITERAL	"boolean literal"

/* Rules section delimited by the markers %% */

%token LPARENTH "("
%token RPARENTH ")"

%token PLUS  "+"
%token MINUS "-"
%token ASTER "*"
%token SLASH "/"
%token GREATER ">"
%token SMALLER "<"
%token GREATER_EQUAL ">="
%token SMALLER_EQUAL "<="
%token AND "&&"
%token OR "||"
%token EQUAL "=="
%token UNEQUAL "!="
%token MINUSU "-"
%token PLUSU "+"
%token EXCLAM "!"

%type <enum mCc_ast_unary_op>  unary_op
%type <enum mCc_ast_binary_op> binary_op
%type <enum mCc_ast_binary_add_op> add_op
%type <enum mCc_ast_binary_mul_op> mul_op


%type <struct mCc_ast_expression *> expression single_expr
%type <struct mCc_ast_literal *> literal

/* Precendence assignment. Priority is sorted from low to high*/


%start toplevel

%%

/* Rules section delimited by the markers %% */

toplevel : expression { *result = $1; }
         ;

/* unary operators */

unary_op  : MINUSU { $$ = MCC_AST_UNARY_OP_MINUS; }
		  | PLUSU { $$ = MCC_AST_UN}
		  | EXCLAM { $$ = MCC_AST_UNARY_OP_EXCLAM; }
		;
binary_op : PLUS  { $$ = MCC_AST_BINARY_OP_ADD; }
          | MINUS { $$ = MCC_AST_BINARY_OP_SUB; }
          | ASTER { $$ = MCC_AST_BINARY_OP_MUL; }
          | SLASH { $$ = MCC_AST_BINARY_OP_DIV; }
		  | GREATER { $$ = MCC_AST_BINARY_OP_GRT; }
		  | SMALLER { $$ = MCC_AST_BINARY_OP_SMT; }
		  | GREATER_EQUAL { $$ = MCC_AST_BINARY_OP_GRE; }
		  | SMALLER_EQUAL { $$ = MCC_AST_BINARY_OP_SME; }
		  | AND { $$ = MCC_AST_BINARY_OP_AND; }
		  | OR { $$ = MCC_AST_BINARY_OP_OR; }
		  | EQUAL { $$ = MCC_AST_BINARY_OP_EQ; }
		  | UNEQUAL { $$ = MCC_AST_BINARY_OP_UEQ; }
          ;

add_op : PLUS  { $$ = MCC_AST_BINARY_OP_ADD; }
       | MINUS { $$ = MCC_AST_BINARY_OP_SUB; }
       ;
       
mul_op : ASTER { $$ = MCC_AST_BINARY_OP_MUL; }
       | SLASH { $$ = MCC_AST_BINARY_OP_DIV; }
;

single_expr : literal                         { $$ = mCc_ast_new_expression_literal($1); }
            | LPARENTH expression RPARENTH    { $$ = mCc_ast_new_expression_parenth($2); }
            ;

expression : single_expr                      { $$ = $1;                                           }
           | single_expr binary_op expression { $$ = mCc_ast_new_expression_binary_op($2, $1, $3); }
		   | single_expr unary_op expression { $$ = mCc_ast_new_expression_unary_op($2,$1);}
           ;

literal : INT_LITERAL   { $$ = mCc_ast_new_literal_int($1);   }
        | FLOAT_LITERAL { $$ = mCc_ast_new_literal_float($1); }
				| BOOL_LITERAL	{ $$ = mCc_ast_new_literal_bool($1); }
        ;

%%

#include <assert.h>

#include "scanner.h"

void yyerror(yyscan_t *scanner, const char *msg) {}

struct mCc_parser_result mCc_parser_parse_string(const char *input)
{
	assert(input);

	struct mCc_parser_result result = { 0 };

	FILE *in = fmemopen((void *)input, strlen(input), "r");
	if (!in) {
		result.status = MCC_PARSER_STATUS_UNABLE_TO_OPEN_STREAM;
		return result;
	}

	result = mCc_parser_parse_file(in);

	fclose(in);

	return result;
}

struct mCc_parser_result mCc_parser_parse_file(FILE *input)
{
	assert(input);

	yyscan_t scanner;
	mCc_parser_lex_init(&scanner);
	mCc_parser_set_in(input, scanner);

	struct mCc_parser_result result = {
		.status = MCC_PARSER_STATUS_OK,
	};

	if (yyparse(scanner, &result.expression) != 0) {
		result.status = MCC_PARSER_STATUS_UNKNOWN_ERROR;
	}

	mCc_parser_lex_destroy(scanner);

	return result;
}
