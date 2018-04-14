%define api.prefix {mCc_parser_}

%define api.pure full
%lex-param   {void *scanner}
%parse-param {void *scanner} {struct mCc_ast_expression** expr_result}
			     {struct mCc_ast_literal** lit_result}
			     {struct mCc_ast_statement** stmt_result}
			     {struct mCc_ast_declaration** dec_result}
			     {struct mCc_ast_program** result}
/*TODO: combine it to only one struct mCc_ast_program*/
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


%token <const char*>	IDENTIFIER	"identifier"
%token <long>   INT_LITERAL   "integer literal"
%token <double> FLOAT_LITERAL "float literal"
%token <bool>	BOOL_LITERAL	"boolean literal"
%token <const char*> STRING_LITERAL "string literal"

%token INT_TYPE "int"
%token FLOAT_TYPE "float"
%token BOOL_TYPE "bool"
%token STRING_TYPE "string"
%token VOID_TYPE "void"

%token IF	"if"
%token ELSE	"else"
%token WHILE	"while"
%token RETURN	"return"

%token LPARENTH "("
%token RPARENTH ")"
%token LSQUARE_BRACKET "["
%token RSQUARE_BRACKET "]"
%token COMMA ","
%token SEMICOLON ";"
%token ASSIGN "="
%token LBRACKET "{"
%token RBRACKET "}"

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
%token EXCLAM "!"


/* To handle the precedence of operations, we grouped binary operators
   into groups of equal precedence. This technique is called "prececence cascade".
   We also rewrite the grammar rules so operators in the same group 
   will be left assosiative.
   Reference: Kenneth C. Louden, "Compiler Construction: Principles and Practice",
   Chapter 3, 3.4.2. Precedence and Associativity
*/
%type <enum mCc_ast_unary_op>  unary_op
%type <enum mCc_ast_binary_op> binary_op
%type <enum mCc_ast_binary_mul_op> mul_op
%type <enum mCc_ast_binary_add_op> add_op
%type <enum mCc_ast_binary_compare_op> compare_op

%type <enum mCc_ast_var_type>var_type
%type <enum mCc_ast_function_type>function_type

%type <struct mCc_ast_statement *>statement compound_stmt
%type <struct mCc_ast_expression *> expression term term_2 single_expr
%type <struct mCc_ast_literal *> literal

%type <struct mCc_ast_declaration *> declaration
%type <struct mCc_ast_assignment *> assignment

%start toplevel

%%
         
toplevel : expression { *expr_result = $1; }
	 | statement  { *stmt_result = $1; }
         ;
		 
/* unary operators */

unary_op  : MINUS { $$ = MCC_AST_UNARY_OP_MINUS; }
		  | PLUS { $$ = MCC_AST_UNARY_OP_PLUS;}
		  | EXCLAM { $$ = MCC_AST_UNARY_OP_EXCLAM; }

/* binary operators */
binary_op : AND { $$ = MCC_AST_BINARY_OP_AND; }
	  | OR { $$ = MCC_AST_BINARY_OP_OR; }
          ;

compare_op : GREATER { $$ = MCC_AST_BINARY_OP_GRT; }
          | SMALLER { $$ = MCC_AST_BINARY_OP_SMT; }
          | GREATER_EQUAL { $$ = MCC_AST_BINARY_OP_GRE; }
          | SMALLER_EQUAL { $$ = MCC_AST_BINARY_OP_SME; }
          | EQUAL { $$ = MCC_AST_BINARY_OP_EQ; }
          | UNEQUAL { $$ = MCC_AST_BINARY_OP_UEQ; }
          ;

add_op : PLUS  { $$ = MCC_AST_BINARY_OP_ADD; }
       | MINUS { $$ = MCC_AST_BINARY_OP_SUB; }
       ;
       
mul_op : ASTER { $$ = MCC_AST_BINARY_OP_MUL; }
       | SLASH { $$ = MCC_AST_BINARY_OP_DIV; }
       ;
       
/* Type */
var_type : INT_TYPE 	{ $$ = MCC_AST_VARIABLES_TYPE_INT; }
		 | FLOAT_TYPE 	{ $$ = MCC_AST_VARIABLES_TYPE_FLOAT; }
		 | BOOL_TYPE 	{ $$ = MCC_AST_VARIABLES_TYPE_BOOL; }
		 | STRING_TYPE 	{ $$ = MCC_AST_VARIABLES_TYPE_STRING; }
		 ;
		 
function_type : BOOL_TYPE { $$ = MCC_AST_FUNCTION_TYPE_BOOL; }
	     | INT_TYPE  { $$ = MCC_AST_FUNCTION_TYPE_INT; }
	     | FLOAT_TYPE{ $$ = MCC_AST_FUNCTION_TYPE_FLOAT; }
	     | STRING_TYPE  { $$ = MCC_AST_FUNCTION_TYPE_STRING; }
	     | VOID_TYPE { $$ = MCC_AST_FUNCTION_TYPE_VOID; }
	     ;
	 
/* Expressions */
single_expr : literal                         { $$ = mCc_ast_new_expression_literal($1); }
			| unary_op INT_LITERAL 			  { $$ = mCc_ast_new_expression_unary_op($1, mCc_ast_new_expression_literal(mCc_ast_new_literal_int($2)));}
            | LPARENTH expression RPARENTH    { $$ = mCc_ast_new_expression_parenth($2); }
            ;
            
term : single_expr				{ $$ = $1; }
	 | term mul_op single_expr	{ $$ = mCc_ast_new_expression_mul_op($2, $1, $3); }
	 ;

term_2 : term                    { $$ = $1;                                           }
           | term_2 add_op term  { $$ = mCc_ast_new_expression_add_op($2, $1, $3); }
           | term_2 compare_op term_2 { $$ = mCc_ast_new_expression_compare_op($2, $1, $3); }
           ;
           
expression : term_2				{ $$ = $1; }
		   | expression binary_op term_2  { $$ = mCc_ast_new_expression_binary_op($2, $1, $3); }

		   ;

/* Literal */
literal : INT_LITERAL   { $$ = mCc_ast_new_literal_int($1);   }
        | FLOAT_LITERAL { $$ = mCc_ast_new_literal_float($1); }
		| BOOL_LITERAL	{ $$ = mCc_ast_new_literal_bool($1); }
		| STRING_LITERAL {$$ = mCc_ast_new_literal_string($1);}
		| IDENTIFIER	{ $$ = mCc_ast_new_literal_identifier($1); }
		;
		
/* Statements */

statement : expression SEMICOLON	{ $$ = mCc_ast_new_statement_expression($1); }
		  | declaration SEMICOLON   { $$ = mCc_ast_new_statement_declaration($1); }
		  | compound_stmt			{ $$ = $1; }
/*		  | if_stmt					{ $$ = $1; }
		  | while_stmt				{ $$ = $1; }
		  | ret_stmt				{ $$ = $1; }
*/		  
		  ;
		  
compound_stmt : LBRACKET RBRACKET			{ $$ = mCc_ast_new_statement_compound_1(); }
			  | LBRACKET statement RBRACKET	{ $$ = mCc_ast_new_statement_compound_2($2); }
			  ;
/*
if_stmt : IF LPARENTH expression RPARENTH statement 						{$$ = mCc_ast_new_statement_if($3, $5); }
		| IF LPARENTH expression RPARENTH compound_stmt ELSE compound_stmt 	{$$ = mCc_ast_new_statement_if_else($3, $5, $7); }
		;
		
while_stmt : WHILE LPARENTH expression RPARENTH statement {$$ = mCc_ast_new_statement_while($3, $5); }
		   ;
		   
ret_stmt : RETURN SEMICOLON				{ $$ = mCc_ast_new_statement_return(); }
		 | RETURN expression SEMICOLON  { $$ = mCc_ast_new_statement_return_2($2); }
		 ;
*/
/* Declaration/Assignment */
/* Idea for array declaration and assignment credits to team 21 */
declaration : var_type IDENTIFIER {$$ = mCc_ast_new_declaration($1, mCc_ast_new_literal_identifier($2));}
			| var_type LSQUARE_BRACKET INT_LITERAL RSQUARE_BRACKET IDENTIFIER {$$ = 
			mCc_ast_new_array_declaration($1, $3, mCc_ast_new_literal_identifier($5));}
			;
			
assignment : IDENTIFIER EQUAL expression 	{$$ = mCc_ast_new_statement_ass_1(mCc_ast_new_literal_identifier($1),
											$3);}
		   | IDENTIFIER LSQUARE_BRACKET expression RSQUARE_BRACKET ASSIGN expression	{$$ = $1;}
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

/* Idea from team 21 */
void mCc_parser_delete_result(struct mCc_parser_result* result) {
	if (result->expression != NULL) {
		mCc_ast_delete_expression(result->expression);
	}

	if (result->literal != NULL) {
		mCc_ast_delete_literal(result->literal);
	}

	if (result->statement != NULL) {
		mCc_ast_delete_statement(result->statement);
	}

	if (result->declaration != NULL) {
		mCc_ast_delete_declaration(result->declaration);
	}

	if (result->program != NULL) {
		mCc_ast_delete_program(result->program);
	}
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

	if (yyparse(scanner, &result.expression, &result.literal, &result.statement,
&result.declaration, &result.program) != 0) {
		result.status = MCC_PARSER_STATUS_UNKNOWN_ERROR;
	}

	mCc_parser_lex_destroy(scanner);

	return result;
}
