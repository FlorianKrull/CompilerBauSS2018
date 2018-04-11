#ifndef MCC_AST_H
#define MCC_AST_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __cplusplus
typedef int bool;
#define true 1
#define false 0
#endif

/* Forward Declarations */
struct mCc_ast_expression;
struct mCc_ast_literal;

/* ---------------------------------------------------------------- AST Node */

struct mCc_ast_source_location {
	int start_line;
	int start_col;
	int end_line;
	int end_col;
};

/* Data contained by every AST node. */
struct mCc_ast_node {
	struct mCc_ast_source_location sloc;
};

/* --------------------------------------------------------------- Operators */

enum mCc_ast_unary_op {
	MCC_AST_UNARY_OP_MINUS,
	MCC_AST_UNARY_OP_PLUS,
	MCC_AST_UNARY_OP_EXCLAM,
};

enum mCc_ast_binary_op {
	MCC_AST_BINARY_OP_AND,
	MCC_AST_BINARY_OP_OR,
};

enum mCc_ast_binary_compare_op {
  MCC_AST_BINARY_OP_SMT,
	MCC_AST_BINARY_OP_GRT,
	MCC_AST_BINARY_OP_GRE,
	MCC_AST_BINARY_OP_SME,
  MCC_AST_BINARY_OP_EQ,
	MCC_AST_BINARY_OP_UEQ,
};

/* For precedence cascade technique */
enum mCc_ast_binary_add_op {
	MCC_AST_BINARY_OP_ADD,
	MCC_AST_BINARY_OP_SUB,
};

/* For precedence cascade technique */
enum mCc_ast_binary_mul_op {
	MCC_AST_BINARY_OP_MUL,
	MCC_AST_BINARY_OP_DIV,
};

/* ------------------------------------------------------------- Expressions */

enum mCc_ast_expression_type {
	MCC_AST_EXPRESSION_TYPE_LITERAL,
	MCC_AST_EXPRESSION_TYPE_UNARY_OP,
	MCC_AST_EXPRESSION_TYPE_BINARY_OP,
	MCC_AST_EXPRESSION_TYPE_PARENTH,
};

struct mCc_ast_expression {
	struct mCc_ast_node node;

	enum mCc_ast_expression_type type;
	union {
		/* MCC_AST_EXPRESSION_TYPE_LITERAL */
		struct mCc_ast_literal *literal;

		/* MCC_AST_EXPRESSION_TYPE_UNARY_OP */
		struct {
			enum mCc_ast_unary_op unary_op;
			struct mCc_ast_expression *u_rhs;
		};

		struct {
			/* For precedence cascade technique */
			union {
				/* MCC_AST_EXPRESSION_TYPE_BINARY_OP */
				enum mCc_ast_binary_op op;

				/* MCC_AST_EXPRESSION_TYPE_ADD_OP */
				enum mCc_ast_binary_add_op add_op;

				/* MCC_AST_EXPRESSION_TYPE_MUL_OP */
				enum mCc_ast_binary_mul_op mul_op;

        /* MCC_AST_EXPRESSION_TYPE_COMPARE_OP */
				enum mCc_ast_binary_compare_op compare_op;
			};
			struct mCc_ast_expression *lhs;
			struct mCc_ast_expression *rhs;
		};

		/* MCC_AST_EXPRESSION_TYPE_PARENTH */
		struct mCc_ast_expression *expression;
	};
};

struct mCc_ast_expression *
mCc_ast_new_expression_literal(struct mCc_ast_literal *literal);

struct mCc_ast_expression *
mCc_ast_new_expression_unary_op(enum mCc_ast_unary_op op,
								struct mCc_ast_expression *rhs);

struct mCc_ast_expression *
mCc_ast_new_expression_binary_op(enum mCc_ast_binary_op op,
                                 struct mCc_ast_expression *lhs,
                                 struct mCc_ast_expression *rhs);

struct mCc_ast_expression *
mCc_ast_new_expression_add_op(enum mCc_ast_binary_add_op add_op,
                                 struct mCc_ast_expression *lhs,
                                 struct mCc_ast_expression *rhs);

struct mCc_ast_expression *
mCc_ast_new_expression_mul_op(enum mCc_ast_binary_mul_op mul_op,
                                 struct mCc_ast_expression *lhs,
                                 struct mCc_ast_expression *rhs);

struct mCc_ast_expression *
mCc_ast_new_expression_compare_op(enum mCc_ast_binary_compare_op compare_op,
                                 struct mCc_ast_expression *lhs,
                                 struct mCc_ast_expression *rhs);


struct mCc_ast_expression *
mCc_ast_new_expression_parenth(struct mCc_ast_expression *expression);

void mCc_ast_delete_expression(struct mCc_ast_expression *expression);

/* ---------------------------------------------------------------- Literals */

enum mCc_ast_literal_type {
	MCC_AST_LITERAL_TYPE_ALPHA,
	MCC_AST_LITERAL_TYPE_ALPHA_NUM,
	MCC_AST_LITERAL_TYPE_DIGIT,
	MCC_AST_LITERAL_TYPE_IDENTIFIER,
	MCC_AST_LITERAL_TYPE_INT,
	MCC_AST_LITERAL_TYPE_FLOAT,
	MCC_AST_LITERAL_TYPE_BOOL,
	MCC_AST_LITERAL_TYPE_STRING,
};

struct mCc_ast_literal {
	struct mCc_ast_node node;

	enum mCc_ast_literal_type type;
	union {
		/* MCC_AST_LITERAL_TYPE_ALPHA */
		char a_value;

		/* MCC_AST_LITERAL_TYPE_ALPHA_NUM */
		char an_value;

		/* MCC_AST_LITERAL_TYPE_DIGIT */
		int d_value;

		/* MCC_AST_LITERAL_TYPE_IDENTIFIER */
		char *id_value;
		/* MCC_AST_LITERAL_TYPE_INT */
		long i_value;

		/* MCC_AST_LITERAL_TYPE_FLOAT */
		double f_value;

		/* MCC_AST_LITERAL_TYPE_BOOL */
		bool b_value;

		/* MCC_AST_LITERAL_TYPE_STRING */
		char *s_value;
	};
};

struct mCc_ast_literal *mCc_ast_new_literal_alpha(char value);

struct mCc_ast_literal *mCc_ast_new_literal_alpha_num(char value);

struct mCc_ast_literal *mCc_ast_new_literal_digit(int value);

struct mCc_ast_literal *mCc_ast_new_literal_identifier(char* value);

struct mCc_ast_literal *mCc_ast_new_literal_int(long value);

struct mCc_ast_literal *mCc_ast_new_literal_float(double value);

struct mCc_ast_literal *mCc_ast_new_literal_bool(bool value);

struct mCc_ast_literal *mCc_ast_new_literal_string(char *value);

void mCc_ast_delete_literal(struct mCc_ast_literal *literal);

/* ------------------------------------------------------------- Statements */
enum mCc_ast_statement_type {
	MCC_AST_STATEMENT_TYPE_EXPRESSION,
	MCC_AST_STATEMENT_TYPE_COMPOUND,
	MCC_AST_STATEMENT_TYPE_IF,
	MCC_AST_STATEMENT_TYPE_IF_ELSE,
	MCC_AST_STATEMENT_TYPE_WHILE,
	MCC_AST_STATEMENT_TYPE_RETURN,
};

struct mCc_ast_statement {
	struct mCc_ast_node node;

	enum mCc_ast_statement_type type;
	union {
		/* MCC_AST_STATEMENT_TYPE_EXPRESSION, MCC_AST_STATEMENT_TYPE_RETURN */
		struct mCc_ast_expression *expression;

		/* MCC_AST_STATEMENT_TYPE_COMPOUND */
		struct mCc_ast_statement *statement;

		/* MCC_AST_STATEMENT_TYPE_IF, MCC_AST_STATEMENT_TYPE_WHILE */
		struct {
			struct mCc_ast_expression *expr;
			struct mCc_ast_statement *stmt;
		};

		/* MCC_AST_STATEMENT_TYPE_IF_ELSE */
		struct {
			struct mCc_ast_expression *expr_1;
			struct mCc_ast_statement *compount_stmt_1;
			struct mCc_ast_statement *compount_stmt_2;
		};

		//TODO: if_stmt
	};
};

struct mCc_ast_statement *
mCc_ast_new_statement_expression(struct mCc_ast_expression *expression);

struct mCc_ast_statement *
mCc_ast_new_statement_compound_1();

struct mCc_ast_statement *
mCc_ast_new_statement_compound_2(struct mCc_ast_statement *statement);

/*
struct mCc_ast_statement *
mCc_ast_new_statement_if(struct mCc_ast_expression *expression, struct mCc_ast_statement *statement);

struct mCc_ast_statement *
mCc_ast_new_statement_if_else(struct mCc_ast_expression *expression,
		struct mCc_ast_statement *compound_1, struct mCc_ast_statement *compound_2);

struct mCc_ast_statement *
mCc_ast_new_statement_while(struct mCc_ast_expression *expression, struct mCc_ast_statement *statement);

struct mCc_ast_statement *
mCc_ast_new_statement_return();

struct mCc_ast_statement *
mCc_ast_new_statement_return_2(struct mCc_ast_expression *expression);
*/

void mCc_ast_delete_statement(struct mCc_ast_statement *statement);

#ifdef __cplusplus
}
#endif

#endif
