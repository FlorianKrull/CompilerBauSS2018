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

enum mCc_ast_binary_op {
	MCC_AST_BINARY_OP_SMT,
	MCC_AST_BINARY_OP_GRT,
	MCC_AST_BINARY_OP_GRE,
	MCC_AST_BINARY_OP_SME,
	MCC_AST_BINARY_OP_AND,
	MCC_AST_BINARY_OP_OR,
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
	MCC_AST_EXPRESSION_TYPE_BINARY_OP,
	MCC_AST_EXPRESSION_TYPE_PARENTH,
};

struct mCc_ast_expression {
	struct mCc_ast_node node;

	enum mCc_ast_expression_type type;
	union {
		/* MCC_AST_EXPRESSION_TYPE_LITERAL */
		struct mCc_ast_literal *literal;


		struct {
			/* For precedence cascade technique */
			union {
				/* MCC_AST_EXPRESSION_TYPE_BINARY_OP */
				enum mCc_ast_binary_op op;

				/* MCC_AST_EXPRESSION_TYPE_ADD_OP */
				enum mCc_ast_binary_add_op add_op;

				/* MCC_AST_EXPRESSION_TYPE_MUL_OP */
				enum mCc_ast_binary_mul_op mul_op;
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
	};
};

struct mCc_ast_literal *mCc_ast_new_literal_alpha(char value);

struct mCc_ast_literal *mCc_ast_new_literal_alpha_num(char value);

struct mCc_ast_literal *mCc_ast_new_literal_digit(int value);

struct mCc_ast_literal *mCc_ast_new_literal_identifier(char* value);

struct mCc_ast_literal *mCc_ast_new_literal_int(long value);

struct mCc_ast_literal *mCc_ast_new_literal_float(double value);

struct mCc_ast_literal *mCc_ast_new_literal_bool(bool value);

void mCc_ast_delete_literal(struct mCc_ast_literal *literal);

#ifdef __cplusplus
}
#endif

#endif
