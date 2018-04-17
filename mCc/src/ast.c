#include "mCc/ast.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
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
	lit->id_value = strdup(value);
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
	lit->s_value = strdup(value);
	return lit;
}

void mCc_ast_delete_literal(struct mCc_ast_literal *literal)
{
	assert(literal);
	free(literal);
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
mCc_ast_new_expression_call(const char* value,
                            struct mCc_ast_argument_list *arguments)
{
	assert(value);

	struct mCc_ast_expression *expr = malloc(sizeof(*expr));
	if (!expr) {
		return NULL;
	}

	expr->type = MCC_AST_EXPRESSION_TYPE_CALL_EXPR;
	expr->call_expr.identifier = mCc_ast_new_literal_identifier(value);
	expr->call_expr.arguments = arguments;
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
    	expr->binary_op_type = MCC_AST_BINARY_OP_TYPE_BINARY;
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
    	expr->binary_op_type = MCC_AST_BINARY_OP_TYPE_ADD;
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
    	expr->binary_op_type = MCC_AST_BINARY_OP_TYPE_MUL;
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
    	expr->binary_op_type = MCC_AST_BINARY_OP_TYPE_COMPARE;
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

	case MCC_AST_EXPRESSION_TYPE_CALL_EXPR:
		mCc_ast_delete_literal(expression->call_expr.identifier);
		if (expression->call_expr.arguments != NULL) {
			mCc_ast_delete_argument_list(expression->call_expr.arguments);
		}
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

struct mCc_ast_statement *
mCc_ast_new_statement_if(struct mCc_ast_expression *expression,
		struct mCc_ast_statement *stmt_1, struct mCc_ast_statement *stmt_2)
{
	assert(expression);
	assert(stmt_1);

	struct mCc_ast_statement *stmt = malloc(sizeof(*stmt));
	if (!stmt) {
		return NULL;
	}

	if (stmt_2 != NULL) {
		stmt->type = MCC_AST_STATEMENT_TYPE_IF_ELSE;
		stmt->if_else_stmt.expr = mCc_ast_new_expression_parenth(expression);
		stmt->if_else_stmt.stmt_1 = stmt_1;
		stmt->if_else_stmt.stmt_2 = stmt_2;
	} else {
		stmt->type = MCC_AST_STATEMENT_TYPE_IF;
		stmt->expr = mCc_ast_new_expression_parenth(expression);
		stmt->stmt = stmt_1;
	}
	return stmt;
}
/*
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
		case MCC_AST_STATEMENT_TYPE_WHILE:
			mCc_ast_delete_expression(statement->expr);
			mCc_ast_delete_statement(statement->stmt);
			break;
		case MCC_AST_STATEMENT_TYPE_IF_ELSE:
			mCc_ast_delete_expression(statement->if_else_stmt.expr);
			mCc_ast_delete_statement(statement->if_else_stmt.stmt_1);
			mCc_ast_delete_statement(statement->if_else_stmt.stmt_2);
			break;

		default: break;
		}

	free(statement);
}

/* ------------------------------------------------------------- Function Definition/Call */
struct mCc_ast_parameter *
mCc_ast_new_parameter(struct mCc_ast_declaration *declaration,
		struct mCc_ast_parameter *next)
{
	assert(declaration);

	struct mCc_ast_parameter *param = malloc(sizeof(*param));
	if (!param) {
		return NULL;
	}

	param->declaration = declaration;
	param->next = next;
	return param;
}

void mCc_ast_delete_parameter(struct mCc_ast_parameter *param)
{
	assert(param);
	mCc_ast_delete_declaration(param->declaration);
	if (param->next != NULL) {
		mCc_ast_delete_parameter(param->next);
	}
	free(param);
}

struct mCc_ast_function_def *
mCc_ast_new_function_def(enum mCc_ast_function_type type,
                         const char *id_value,
                         struct mCc_ast_parameter *params,
                         struct mCc_ast_statement *compound_stmt)
{
	assert(type);
	assert(id_value);
	assert(compound_stmt);

	struct mCc_ast_function_def *function_def = malloc(sizeof(*function_def));
	if(!function_def) {
		return NULL;
	}

	function_def->type = type;
	function_def->identifier = mCc_ast_new_literal_identifier(id_value);
	function_def->compound_stmt = compound_stmt;
	function_def->parameters = params;

	return function_def;
}

void mCc_ast_delete_function_def(struct mCc_ast_function_def *function_def)
{
	assert(function_def);
	mCc_ast_delete_literal(function_def->identifier);
//	mCc_ast_delete_parameter(function_def->parameters);
	if (NULL != function_def->parameters) {
		mCc_ast_delete_parameter(function_def->parameters);
	}
	mCc_ast_delete_statement(function_def->compound_stmt);
	free(function_def);
}

struct mCc_ast_argument_list *
mCc_ast_new_argument_list(struct mCc_ast_expression *expression)
{
	assert(expression);

	struct mCc_ast_argument_list *list = malloc(sizeof(*list));
	if(!list) {
		return NULL;
	}

	list->expression = expression;
	list->next = NULL;
	return list;
}

void mCc_ast_delete_argument_list(struct mCc_ast_argument_list *argument_list)
{
	assert(argument_list);
	mCc_ast_delete_expression(argument_list->expression);
	if (argument_list->next != NULL) {
		mCc_ast_delete_argument_list(argument_list->next);
	}
	free(argument_list);
}

struct mCc_ast_function_def_list *
mCc_ast_new_function_def_list(struct mCc_ast_function_def *function_def)
{
	assert(function_def);
	struct mCc_ast_function_def_list *list = malloc(sizeof(*list));
	if(!list) {
		return NULL;
	}

	list->function_def = function_def;
	list->next = NULL;
	return list;
}

void mCc_ast_delete_function_def_list(
    struct mCc_ast_function_def_list *function_def_list)
{
	assert(function_def_list);
	if (function_def_list->next != NULL) {
		mCc_ast_delete_function_def_list(function_def_list->next);
	}
	mCc_ast_delete_function_def(function_def_list->function_def);
	free(function_def_list);
}

/* ---------------------------------------------------------------- Program */
struct mCc_ast_program *
mCc_ast_new_program(struct mCc_ast_function_def_list *function_def_list)
{
	assert(function_def_list);
	struct mCc_ast_program *program = malloc(sizeof(*program));
	if(!program) {
		return NULL;
	}

	program->function_def_list = function_def_list;
	return program;
}

void mCc_ast_delete_program(struct mCc_ast_program *program)
{
	assert(program);
	mCc_ast_delete_function_def_list(program->function_def_list);
	free(program);
}
