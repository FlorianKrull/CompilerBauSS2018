%define api.prefix {mCc_parser_}

%define api.pure full
%lex-param   {void *scanner}
%parse-param {void *scanner} {struct mCc_ast_expression** expr_result}
			     {struct mCc_ast_statement** stmt_result}
			     {struct mCc_ast_parameter** par_result}
			     {struct mCc_ast_declaration** decl_result}
			     {struct mCc_ast_program** result}
			     {struct mCc_parse_error* result_error}

%define parse.trace
%define parse.error verbose

%code requires {
#include "mCc/parser.h"
}

%{
#include <string.h>
#include <assert.h>

int mCc_parser_lex();
void mCc_parser_error();
%}

%define api.value.type union
%define api.token.prefix {TK_}

%locations

/* Precedence */
%right PREC_IF ELSE

%token START_PROGRAM START_TEST
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

%type <enum mCc_ast_type>var_type function_type

%type <struct mCc_ast_statement *>statement compound_stmt if_stmt while_stmt ret_stmt
%type <struct mCc_ast_expression *> expression term term_2 single_expr call_expr
%type <struct mCc_ast_literal *> literal

%type <struct mCc_ast_declaration *> declaration
%type <struct mCc_ast_assignment *> assignment

%type <struct mCc_ast_function_def *> function_def
%type <struct mCc_ast_parameter *>parameters
%type <struct mCc_ast_argument_list *> arguments
%type <struct mCc_ast_function_def_list *> function_def_list
%type <struct mCc_ast_program *> program

%start toplevel

/* Destructor - to avoid memory leaks */
/* Ideas from team 21 */
%destructor {mCc_ast_delete_literal($$);} literal
%destructor {mCc_ast_delete_expression($$);} expression term term_2 single_expr call_expr
%destructor {mCc_ast_delete_statement($$);} statement compound_stmt if_stmt while_stmt ret_stmt
%destructor {mCc_ast_delete_declaration($$);} declaration
%destructor {mCc_ast_delete_argument_list($$);} arguments
%destructor {mCc_ast_delete_parameter($$);} parameters
%destructor {mCc_ast_delete_assignment($$);} assignment
%destructor {mCc_ast_delete_function_def($$);} function_def
%destructor {mCc_ast_delete_function_def_list($$);}  function_def_list
%destructor {mCc_ast_delete_program($$);} program

%%
         
toplevel : program { *result = $1; }
		 | expression { *expr_result = $1; }
	 	 | statement  { *stmt_result = $1; }
	 	 | parameters { *par_result = $1; }
	 	 | declaration { *decl_result = $1; }
         ;

/*
toplevel : START_PROGRAM program { *result = $2; }
	 | START_TEST program    { *result = $2; }
	 | START_TEST statement  { *stmt_result = $2; }
	 | START_TEST expression { *expr_result = $2; }
	 | START_TEST parameters { *par_result = $2; }	
	 ;
 */
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
var_type : INT_TYPE 	{ $$ = MCC_AST_TYPE_INT; }
		 | FLOAT_TYPE 	{ $$ = MCC_AST_TYPE_FLOAT; }
		 | BOOL_TYPE 	{ $$ = MCC_AST_TYPE_BOOL; }
		 | STRING_TYPE 	{ $$ = MCC_AST_TYPE_STRING; }
		 ;
		 
function_type : var_type {$$ = $1; }
	     | VOID_TYPE { $$ = MCC_AST_TYPE_VOID; }
	     ;
	 
/* Expressions */
single_expr : literal                         { $$ = mCc_ast_new_expression_literal($1); }
			| call_expr						  { $$ = $1; }
			| MINUS expression 			  	  { $$ = mCc_ast_new_expression_unary_op(MCC_AST_UNARY_OP_MINUS, ($2));}
			| EXCLAM INT_LITERAL			  { $$ = mCc_ast_new_expression_unary_op(MCC_AST_UNARY_OP_EXCLAM, 
												mCc_ast_new_expression_literal(mCc_ast_new_literal_int(($2))));}
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
		  | assignment SEMICOLON 	{ $$ = mCc_ast_new_statement_assignment($1); }
		  | compound_stmt			{ $$ = $1; }
		  | if_stmt					{ $$ = $1; }
		  | while_stmt				{ $$ = $1; }
		  | ret_stmt SEMICOLON		{ $$ = $1; }	  
		  ;
		  
compound_stmt : LBRACKET RBRACKET			{ $$ = mCc_ast_new_statement_compound(NULL); }
			  | LBRACKET statement RBRACKET	{ $$ = mCc_ast_new_statement_compound($2); }
			  ;

if_stmt : IF LPARENTH expression RPARENTH statement %prec PREC_IF	{$$ = mCc_ast_new_statement_if($3, $5, NULL); }
		| IF LPARENTH expression RPARENTH statement ELSE statement 	{$$ = mCc_ast_new_statement_if($3, $5, $7); }
		;

while_stmt : WHILE LPARENTH expression RPARENTH statement {$$ = mCc_ast_new_statement_while($3, $5); }
		   ;
  
ret_stmt : RETURN 				{ $$ = mCc_ast_new_statement_return(NULL); }
		 | RETURN expression   { $$ = mCc_ast_new_statement_return($2); }
		 ;

/* Declaration/Assignment */
/* Solution for array declaration and assignment credits to team 21 */
declaration : var_type IDENTIFIER {$$ = mCc_ast_new_declaration($1, $2);}
			| var_type LSQUARE_BRACKET INT_LITERAL RSQUARE_BRACKET IDENTIFIER {$$ = 
			mCc_ast_new_array_declaration($1, $3, $5);}
			;
			
assignment : IDENTIFIER ASSIGN expression 	{$$ = mCc_ast_new_assignment($1,
											$3);}
		   | IDENTIFIER LSQUARE_BRACKET expression RSQUARE_BRACKET ASSIGN expression	{$$ = 
		   									mCc_ast_new_array_assignment($1, $3,
											$6);}
		   ;
		   
/* Function definition/call */
function_def : function_type IDENTIFIER LPARENTH parameters RPARENTH compound_stmt { $$ = 
							mCc_ast_new_function_def($1, $2, $4, $6); }
			 | function_type IDENTIFIER LPARENTH RPARENTH compound_stmt { $$ = 
			 				mCc_ast_new_function_def($1, $2, NULL, $5); }
			 ;

/* Idea from team 21 */
parameters  : declaration COMMA parameters { $$ = mCc_ast_new_parameter($1, $3); }
			| declaration                  { $$ = mCc_ast_new_parameter($1, NULL);                }
			;

call_expr : IDENTIFIER LPARENTH arguments RPARENTH     { $$ = mCc_ast_new_expression_call($1, $3); }
          | IDENTIFIER LPARENTH RPARENTH               { $$ = mCc_ast_new_expression_call($1, NULL); }
		  ;

arguments : expression COMMA arguments         { $$ = mCc_ast_new_argument_list($1); $$->next = $3; }
		  | expression                         { $$ = mCc_ast_new_argument_list($1);                }
		  ;
		  
function_def_list : function_def function_def_list { $$ = mCc_ast_new_function_def_list($1, $2); }
				  | function_def                   { $$ = mCc_ast_new_function_def_list($1, NULL);                }
				  ;

program : function_def_list { $$ = mCc_ast_new_program($1); }
		;
%%

#include <assert.h>

#include "scanner.h"

//void yyerror(yyscan_t *scanner, const char *msg) {}

/* Error handling, idea from team 21 */
void mCc_parser_error(
	struct MCC_PARSER_LTYPE *yylloc,
	yyscan_t *scanner,
	struct mCc_ast_expression** result_e,
	struct mCc_ast_literal** result_l,
	struct mCc_ast_statement** result_s,
	struct mCc_ast_function_def** result_f,
	struct mCc_ast_declaration** result_d,
	struct mCc_ast_program** result,
	struct mCc_parse_error* result_error,
	const char *msg)
{
	 // suppress the warning unused parameter
	(void)scanner;
	(void)result_e;
	(void)result_l;
	(void)result_s;
	(void)result_f;
	(void)result_d;
	(void)result;
	result_error->is_error = true;
	result_error->location.start_line = yylloc->first_line;
	result_error->location.end_line = yylloc->last_line;
	result_error->location.start_col = yylloc->first_column;
	result_error->location.end_col = yylloc->last_column;
	//result_error->msg = strdup(msg); //ERROR HERE
	result_error->msg = NULL;
}

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

void mCc_parser_delete_result(struct mCc_parser_result* result) {
	assert(result);
	
	if (NULL != result->expression) {
		mCc_ast_delete_expression(result->expression);
	}

	if (NULL != result->statement) {
		mCc_ast_delete_statement(result->statement);
	}
	
	if (NULL != result->parameter) {
		mCc_ast_delete_parameter(result->parameter);
	}
	
	if (NULL != result->declaration) {
		mCc_ast_delete_declaration(result->declaration);
	}

	if (NULL != result->program) {
		mCc_ast_delete_program(result->program);
	}
	
	if (NULL != result->parse_error.msg) {
		free(result->parse_error.msg);
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

	// reset is_error
	result.parse_error.is_error = false;

	if (yyparse(scanner, &result.expression, &result.statement,
	&result.parameter, &result.declaration, &result.program, &result.parse_error) != 0) {
		result.status = MCC_PARSER_STATUS_UNKNOWN_ERROR;
	}

	if (result.parse_error.is_error)
		result.status = MCC_PARSER_STATUS_SYNTAX_ERROR;

	mCc_parser_lex_destroy(scanner);
	
	return result;
}
