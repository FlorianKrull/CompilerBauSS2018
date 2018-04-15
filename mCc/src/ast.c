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

	case MCC_AST_EXPRESSION_TYPE_UNARY_OP:
		mCc_ast_delete_expression(expression->u_rhs);
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

/* ------------------------------------------------------------- Statements/Declaration/Assignment */

struct mCc_ast_declaration *
mCc_ast_new_declaration(enum mCc_ast_var_type var_type,
                        struct mCc_ast_literal *identifier)
{
	assert(identifier);
	struct mCc_ast_declaration *decl = malloc(sizeof(*decl));
	if (!decl) {
		return NULL;
	}

	decl->type = MCC_AST_DECLARATION_TYPE_NORMAL;
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
	if (!decl) {
		return NULL;
	}

	decl->type = MCC_AST_DECLARATION_TYPE_ARRAY;
	decl->var_type = var_type;
	decl->array_decl.identifier = identifier;
	decl->array_decl.size = mCc_ast_new_literal_int(size);
	return decl;
}

void mCc_ast_delete_declaration(struct mCc_ast_declaration *decl)
{
	assert(decl);
	if (decl->type ==
	    MCC_AST_DECLARATION_TYPE_ARRAY) {
		mCc_ast_delete_literal(decl->array_decl.size);
		mCc_ast_delete_literal(decl->array_decl.identifier);
	} else {
		mCc_ast_delete_literal(decl->normal_decl.identifier);
	}
	free(decl);
}

struct mCc_ast_assignment *
mCc_ast_new_assignment(struct mCc_ast_literal *identifier,
                       struct mCc_ast_expression *rhs)
{
	assert(identifier);
	assert(rhs);

	struct mCc_ast_assignment *asmt = malloc(sizeof(*asmt));
	if (!asmt) {
		return NULL;
	}

	asmt->type = MCC_AST_ASSIGNMENT_TYPE_NORMAL;
	asmt->identifier = identifier;
	asmt->normal_asmt.rhs = rhs;
	return asmt;
}

struct mCc_ast_assignment *
mCc_ast_new_array_assignment(struct mCc_ast_literal *identifier,
                             struct mCc_ast_expression *index,
                             struct mCc_ast_expression *rhs)
{
	assert(identifier);
	assert(index);
	assert(rhs);

	struct mCc_ast_assignment *asmt = malloc(sizeof(*asmt));
	if (!asmt) {
			return NULL;
		}

	asmt->type = MCC_AST_ASSIGNMENT_TYPE_ARRAY;
	asmt->identifier = identifier;
	asmt->array_asmt.index = index;
	asmt->array_asmt.rhs = rhs;
	return asmt;
}

void mCc_ast_delete_assignment(struct mCc_ast_assignment *assignment)
{
	assert(assignment);
	mCc_ast_delete_literal(assignment->identifier);
	if (assignment->type == MCC_AST_ASSIGNMENT_TYPE_NORMAL) {
		mCc_ast_delete_expression(assignment->normal_asmt.rhs);
	} else {
		mCc_ast_delete_expression(assignment->array_asmt.index);
		mCc_ast_delete_expression(assignment->array_asmt.rhs);
	}
	free(assignment);
}

struct mCc_ast_statement *
mCc_ast_new_statement_declaration(struct mCc_ast_declaration *declaration)
{
	assert(declaration);

	struct mCc_ast_statement *stmt = malloc(sizeof(*stmt));
	if (!stmt) {
		return NULL;
	}

	stmt->type = MCC_AST_STATEMENT_TYPE_DECLARATION;
	stmt->declaration = declaration;
	return stmt;
}

struct mCc_ast_statement *
mCc_ast_new_statement_assignment(struct mCc_ast_assignment *assignment)
{
	assert(assignment);

	struct mCc_ast_statement *stmt = malloc(sizeof(*stmt));
	if (!stmt) {
		return NULL;
	}

	stmt->type = MCC_AST_STATEMENT_TYPE_ASSIGNMENT;
	stmt->assignment = assignment;
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
mCc_ast_new_statement_compound(struct mCc_ast_statement *statement)
{
//	assert(statement);

	struct mCc_ast_statement *stmt = malloc(sizeof(*stmt));
	if (!stmt) {
		return NULL;
	}

	if (statement != NULL) {
		stmt->type = MCC_AST_STATEMENT_TYPE_COMPOUND;
		stmt->statement = statement;
	} else {
		stmt->type = MCC_AST_STATEMENT_TYPE_COMPOUND_EMPTY;
	}
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
*/
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
mCc_ast_new_statement_return(struct mCc_ast_expression *expression)
{
//	assert(expression);

	struct mCc_ast_statement *stmt = malloc(sizeof(*stmt));
	if (!stmt) {
		return NULL;
	}


	if (expression != NULL) {
		stmt->type = MCC_AST_STATEMENT_TYPE_RETURN;
		stmt->expression = expression;
	} else {
		stmt->type = MCC_AST_STATEMENT_TYPE_RETURN_EMPTY;
	}
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
			mCc_ast_delete_assignment(statement->assignment);
			break;
		case MCC_AST_STATEMENT_TYPE_EXPRESSION:
		case MCC_AST_STATEMENT_TYPE_RETURN:
			mCc_ast_delete_expression(statement->expression);
			break;
		case MCC_AST_STATEMENT_TYPE_COMPOUND:
			mCc_ast_delete_statement(statement->statement);
			break;
		case MCC_AST_STATEMENT_TYPE_IF:
		case MCC_AST_STATEMENT_TYPE_IF_ELSE:

			break;
		case MCC_AST_STATEMENT_TYPE_WHILE:
			mCc_ast_delete_expression(statement->expr);
			mCc_ast_delete_statement(statement->stmt);
			break;
		default: break;
		}

	free(statement);
}
