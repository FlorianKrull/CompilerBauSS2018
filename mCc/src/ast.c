#include "mCc/ast.h"

#include <assert.h>
#include <stdlib.h>

struct mCc_ast_program *mCc_ast_new_program_1 (struct mCc_ast_expression *expression)
{
	assert(expression);

		struct mCc_ast_program *pro = malloc(sizeof(*pro));
		if (!pro) {
			return NULL;
		}

		pro->expression = expression;
		return pro;
}
/*
struct mCc_ast_program *mCc_ast_new_program_2 (struct mCc_ast_var_action *var_action)
{
	assert(var_action);

		struct mCc_ast_program *pro = malloc(sizeof(*pro));
		if (!pro) {
			return NULL;
		}

		pro->var_action = var_action;
		return pro;
}*/

void mCc_ast_delete_program(struct mCc_ast_program *program)
{
	assert(program);
//	mCc_ast_delete_function_def_list(program->function_def_list);
	free(program);
}

/* ------------------------------------------------------------- Expressions */

struct mCc_ast_expression *
mCc_ast_new_expression_literal(struct mCc_ast_literal *literal)
{
	assert(literal);

	struct mCc_ast_expression *expr = malloc(sizeof(*expr));
	if (!expr) {
		return NULL;
	}

	expr->type = MCC_AST_EXPRESSION_TYPE_LITERAL;
	expr->literal = literal;
	return expr;
}

struct mCc_ast_expression *
mCc_ast_new_expression_unary_op(enum mCc_ast_unary_op op,
                                 struct mCc_ast_expression *rhs)
{
	
	assert(rhs);

	struct mCc_ast_expression *expr = malloc(sizeof(*expr));
	if (!expr) {
		return NULL;
	}

	expr->type = MCC_AST_EXPRESSION_TYPE_UNARY_OP;
	expr->unary_op = op;
	expr->u_rhs = rhs;
	return expr;
}

struct mCc_ast_expression *
mCc_ast_new_expression_binary_op(enum mCc_ast_binary_op op,
                                 struct mCc_ast_expression *lhs,
                                 struct mCc_ast_expression *rhs)
{
	assert(lhs);
	assert(rhs);

	struct mCc_ast_expression *expr = malloc(sizeof(*expr));
	if (!expr) {
		return NULL;
	}

	expr->type = MCC_AST_EXPRESSION_TYPE_BINARY_OP;
	expr->op = op;
	expr->lhs = lhs;
	expr->rhs = rhs;
	return expr;
}

struct mCc_ast_expression *
mCc_ast_new_expression_add_op(enum mCc_ast_binary_add_op add_op,
                                 struct mCc_ast_expression *lhs,
                                 struct mCc_ast_expression *rhs)
{
	assert(lhs);
	assert(rhs);

	struct mCc_ast_expression *expr = malloc(sizeof(*expr));
	if (!expr) {
		return NULL;
	}

	expr->type = MCC_AST_EXPRESSION_TYPE_BINARY_OP;
	expr->add_op = add_op;
	expr->lhs = lhs;
	expr->rhs = rhs;
	return expr;
}

struct mCc_ast_expression *
mCc_ast_new_expression_mul_op(enum mCc_ast_binary_mul_op mul_op,
                                 struct mCc_ast_expression *lhs,
                                 struct mCc_ast_expression *rhs)
{
	assert(lhs);
	assert(rhs);

	struct mCc_ast_expression *expr = malloc(sizeof(*expr));
	if (!expr) {
		return NULL;
	}

	expr->type = MCC_AST_EXPRESSION_TYPE_BINARY_OP;
	expr->mul_op = mul_op;
	expr->lhs = lhs;
	expr->rhs = rhs;
	return expr;
}

struct mCc_ast_expression *
mCc_ast_new_expression_compare_op(enum mCc_ast_binary_compare_op compare_op,
                                 struct mCc_ast_expression *lhs,
                                 struct mCc_ast_expression *rhs)
{
	assert(lhs);
	assert(rhs);

	struct mCc_ast_expression *expr = malloc(sizeof(*expr));
	if (!expr) {
		return NULL;
	}

	expr->type = MCC_AST_EXPRESSION_TYPE_BINARY_OP;
	expr->compare_op = compare_op;
	expr->lhs = lhs;
	expr->rhs = rhs;
	return expr;
}

struct mCc_ast_expression *
mCc_ast_new_expression_parenth(struct mCc_ast_expression *expression)
{
	assert(expression);

	struct mCc_ast_expression *expr = malloc(sizeof(*expr));
	if (!expr) {
		return NULL;
	}

	expr->type = MCC_AST_EXPRESSION_TYPE_PARENTH;
	expr->expression = expression;
	return expr;
}

void mCc_ast_delete_expression(struct mCc_ast_expression *expression)
{
	assert(expression);

	switch (expression->type) {
	case MCC_AST_EXPRESSION_TYPE_LITERAL:
		mCc_ast_delete_literal(expression->literal);
		break;

	case MCC_AST_EXPRESSION_TYPE_BINARY_OP:
		mCc_ast_delete_expression(expression->lhs);
		mCc_ast_delete_expression(expression->rhs);
		break;

	case MCC_AST_EXPRESSION_TYPE_PARENTH:
		mCc_ast_delete_expression(expression->expression);
		break;
	}

	free(expression);
}

/* ---------------------------------------------------------------- Literals */

struct mCc_ast_literal *mCc_ast_new_literal_alpha(char value)
{
	struct mCc_ast_literal *lit = malloc(sizeof(*lit));
	if (!lit) {
		return NULL;
	}

	lit->type = MCC_AST_LITERAL_TYPE_ALPHA;
	lit->a_value = value;
	return lit;
}

struct mCc_ast_literal *mCc_ast_new_literal_alpha_num(char value)
{
	struct mCc_ast_literal *lit = malloc(sizeof(*lit));
	if (!lit) {
		return NULL;
	}

	lit->type = MCC_AST_LITERAL_TYPE_ALPHA_NUM;
	lit->an_value = value;
	return lit;
}

struct mCc_ast_literal *mCc_ast_new_literal_digit(int value)
{
	struct mCc_ast_literal *lit = malloc(sizeof(*lit));
	if (!lit) {
		return NULL;
	}

	lit->type = MCC_AST_LITERAL_TYPE_DIGIT;
	lit->d_value = value;
	return lit;
}

struct mCc_ast_literal *mCc_ast_new_literal_identifier(const char* value)
{
	struct mCc_ast_literal *lit = malloc(sizeof(*lit));
	if (!lit) {
		return NULL;
	}

	lit->type = MCC_AST_LITERAL_TYPE_IDENTIFIER;
	lit->id_value = value;
	return lit;
}

struct mCc_ast_literal *mCc_ast_new_literal_int(long value)
{
	struct mCc_ast_literal *lit = malloc(sizeof(*lit));
	if (!lit) {
		return NULL;
	}

	lit->type = MCC_AST_LITERAL_TYPE_INT;
	lit->i_value = value;
	return lit;
}

struct mCc_ast_literal *mCc_ast_new_literal_float(double value)
{
	struct mCc_ast_literal *lit = malloc(sizeof(*lit));
	if (!lit) {
		return NULL;
	}

	lit->type = MCC_AST_LITERAL_TYPE_FLOAT;
	lit->f_value = value;
	return lit;
}

struct mCc_ast_literal *mCc_ast_new_literal_bool(bool value)
{
	struct mCc_ast_literal *lit = malloc(sizeof(*lit));
	if (!lit) {
		return NULL;
	}

	lit->type = MCC_AST_LITERAL_TYPE_BOOL;
	lit->b_value = value;
	return lit;
}

struct mCc_ast_literal *mCc_ast_new_literal_string(const char* value)
{
	struct mCc_ast_literal *lit = malloc(sizeof(*lit));
	if (!lit) {
		return NULL;
	}

	lit->type = MCC_AST_LITERAL_TYPE_STRING;
	lit->s_value = value;
	return lit;
}

void mCc_ast_delete_literal(struct mCc_ast_literal *literal)
{
	assert(literal);
	free(literal);
}

/* ------------------------------------------------------------- Declaration/Assignment */

struct mCc_ast_declaration *
mCc_ast_new_declaration(enum mCc_ast_var_type var_type,
                        struct mCc_ast_literal *identifier)
{
	assert(identifier);
	struct mCc_ast_declaration *decl = malloc(sizeof(*decl));
	//TODO: return this line to code base
	assert(decl);

	decl->type = MCC_AST_DECLARATION_TYPE_DECLARATION;
	decl->var_type = var_type;
	decl->normal_decl.identifier = identifier;
	return decl;
}

struct mCc_ast_declaration *
mCc_ast_new_array_declaration(enum mCc_ast_var_type var_type,
                              long size, struct mCc_ast_literal *identifier)
{
	assert(identifier);

	struct mCc_ast_declaration *decl = malloc(sizeof(*decl));
	assert(decl);

	decl->type = MCC_AST_DECLARATION_TYPE_ARRAY_DECLARATION;
	decl->var_type = var_type;
	decl->array_decl.identifier = identifier;
	decl->array_decl.size = mCc_ast_new_literal_int(size);
	return decl;
}

void mCc_ast_delete_declaration(struct mCc_ast_declaration *decl)
{
	assert(decl);
	if (decl->type ==
	    MCC_AST_DECLARATION_TYPE_ARRAY_DECLARATION) {
		mCc_ast_delete_literal(decl->array_decl.size);
		mCc_ast_delete_literal(decl->array_decl.identifier);
	} else {
		mCc_ast_delete_literal(decl->normal_decl.identifier);
	}
	free(decl);
}


/* ---------------------------------------------------------------- Statements */
struct mCc_ast_statement *
mCc_ast_new_statement_declaration(struct mCc_ast_declaration *declaration)
{
	assert(declaration);

	struct mCc_ast_statement *stmt = malloc(sizeof(*stmt));
	assert(stmt);

	stmt->type = MCC_AST_STATEMENT_TYPE_DECLARATION;
	stmt->declaration = declaration;
	return stmt;
}

struct mCc_ast_statement *
mCc_ast_new_statement_expression(struct mCc_ast_expression *expression)
{
	assert(expression);

	struct mCc_ast_statement *stmt = malloc(sizeof(*stmt));
	if (!stmt) {
		return NULL;
	}

	stmt->type = MCC_AST_STATEMENT_TYPE_EXPRESSION;
	stmt->expression = expression;
	return stmt;
}

struct mCc_ast_statement *
mCc_ast_new_statement_compound_1()
{
	struct mCc_ast_statement *stmt = malloc(sizeof(*stmt));
	if (!stmt) {
		return NULL;
	}

	stmt->type = MCC_AST_STATEMENT_TYPE_COMPOUND;

	return stmt;
}

struct mCc_ast_statement *
mCc_ast_new_statement_compound_2(struct mCc_ast_statement *statement)
{
	assert(statement);

	struct mCc_ast_statement *stmt = malloc(sizeof(*stmt));
	if (!stmt) {
		return NULL;
	}

	stmt->type = MCC_AST_STATEMENT_TYPE_COMPOUND;
	stmt->statement = statement;
	return stmt;
}
/*

struct mCc_ast_statement *
mCc_ast_new_statement_if(struct mCc_ast_expression *expression,
		struct mCc_ast_statement *statement)
{
	assert(expression);
	assert(statement);

	struct mCc_ast_statement *stmt = malloc(sizeof(*stmt));
	if (!stmt) {
		return NULL;
	}

	stmt->type = MCC_AST_STATEMENT_TYPE_IF;
	stmt->expr = expression;
	stmt->stmt = statement;
	return stmt;
}

struct mCc_ast_statement *
mCc_ast_new_statement_if_else(struct mCc_ast_expression *expression,
		struct mCc_ast_statement *compound_1, struct mCc_ast_statement *compound_2)
{
	assert(expression);
	assert(compound_1);
	assert(compound_2);

	struct mCc_ast_statement *stmt = malloc(sizeof(*stmt));
	if (!stmt) {
		return NULL;
	}

	stmt->type = MCC_AST_STATEMENT_TYPE_IF_ELSE;
	stmt->expr_1 = expression;
	stmt->compount_stmt_1 = compound_1;
	stmt->compount_stmt_2 = compound_2;
	return stmt;
}

struct mCc_ast_statement *
mCc_ast_new_statement_while(struct mCc_ast_expression *expression,
		struct mCc_ast_statement *statement)
{
	assert(expression);
	assert(statement);

	struct mCc_ast_statement *stmt = malloc(sizeof(*stmt));
	if (!stmt) {
		return NULL;
	}

	stmt->type = MCC_AST_STATEMENT_TYPE_WHILE;
	stmt->expr = expression;
	stmt->stmt = statement;
	return stmt;
}

struct mCc_ast_statement *
mCc_ast_new_statement_return()
{
	struct mCc_ast_statement *stmt = malloc(sizeof(*stmt));
	if (!stmt) {
		return NULL;
	}

	stmt->type = MCC_AST_STATEMENT_TYPE_RETURN;
	return stmt;
}

struct mCc_ast_statement *
mCc_ast_new_statement_return_2(struct mCc_ast_expression *expression)
{
	assert(expression);

	struct mCc_ast_statement *stmt = malloc(sizeof(*stmt));
	if (!stmt) {
		return NULL;
	}

	stmt->type = MCC_AST_STATEMENT_TYPE_RETURN;
	stmt->expression = expression;
	return stmt;
}
*/

struct mCc_ast_statement *mCc_ast_new_statement_ass_1(struct mCc_ast_literal *id_literal,
		struct mCc_ast_expression *expression)
{
	assert(id_literal);
	assert(expression);

	struct mCc_ast_statement *stmt = malloc(sizeof(*stmt));
	if (!stmt) {
		return NULL;
	}

	stmt->type = MCC_AST_STATEMENT_TYPE_ASSIGNMENT;
	stmt->expression_1 = expression;
	stmt->id_literal_ass = id_literal;
	return stmt;
}

struct mCc_ast_statement *mCc_ast_new_statement_ass_2(struct mCc_ast_literal *id_literal,
		struct mCc_ast_expression *expression_1, struct mCc_ast_expression *expression_2)
{
	assert(id_literal);
	assert(expression_1);
	assert(expression_2);

	struct mCc_ast_statement *stmt = malloc(sizeof(*stmt));
	if (!stmt) {
		return NULL;
	}

	stmt->type = MCC_AST_STATEMENT_TYPE_ASSIGNMENT;
	stmt->expression_1 = expression_1;
	stmt->expression_2 = expression_2;
	stmt->id_literal_ass = id_literal;
	return stmt;
}

void mCc_ast_delete_statement(struct mCc_ast_statement *statement)
{
	assert(statement);

	switch (statement->type) {
		case MCC_AST_STATEMENT_TYPE_DECLARATION:
			mCc_ast_delete_declaration(statement->declaration);
			break;
		case MCC_AST_STATEMENT_TYPE_ASSIGNMENT:
			//TODO: delete if-stmt
			break;
		case MCC_AST_STATEMENT_TYPE_EXPRESSION:
		case MCC_AST_STATEMENT_TYPE_RETURN:
			mCc_ast_delete_expression(statement->expression);
			break;
		case MCC_AST_STATEMENT_TYPE_COMPOUND:

			break;
		case MCC_AST_STATEMENT_TYPE_IF:
		case MCC_AST_STATEMENT_TYPE_IF_ELSE:

			break;
		case MCC_AST_STATEMENT_TYPE_WHILE:

			break;
		default: break;
		}

	free(statement);
}
