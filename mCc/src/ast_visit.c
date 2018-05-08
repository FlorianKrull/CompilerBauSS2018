#include "mCc/ast_visit.h"

#include <assert.h>
#include <stdlib.h>

#define visit(node, callback, visitor) \
	do { \
		if (callback) { \
			(callback)(node, (visitor)->userdata); \
		} \
	} while (0)

#define visit_if(cond, node, callback, visitor) \
	do { \
		if (cond) { \
			visit(node, callback, visitor); \
		} \
	} while (0)

#define visit_if_pre_order(node, callback, visitor) \
	visit_if((visitor)->order == MCC_AST_VISIT_PRE_ORDER, node, callback, \
	         visitor)

#define visit_if_post_order(node, callback, visitor) \
	visit_if((visitor)->order == MCC_AST_VISIT_POST_ORDER, node, callback, \
	         visitor)

void mCc_ast_visit_expression(struct mCc_ast_expression *expression,
                              struct mCc_ast_visitor *visitor)
{
	assert(expression);
	assert(visitor);

	visit_if_pre_order(expression, visitor->expression, visitor);

	switch (expression->type) {
		case MCC_AST_EXPRESSION_TYPE_LITERAL:
			visit_if_pre_order(expression, visitor->expression_literal, visitor);
			mCc_ast_visit_literal(expression->literal, visitor);
			visit_if_post_order(expression, visitor->expression_literal, visitor);
			break;
		case MCC_AST_EXPRESSION_TYPE_CALL_EXPR:
			visit_if_pre_order(expression, visitor->expression_call, visitor);
			mCc_ast_visit_literal(expression->call_expr.identifier, visitor);
                        if(expression->call_expr.arguments != NULL){
                                mCc_ast_visit_arguments(expression->call_expr.arguments, visitor);
                        }
			visit_if_post_order(expression, visitor->expression_call, visitor);
			break;
		case MCC_AST_EXPRESSION_TYPE_BINARY_OP:
			visit_if_pre_order(expression, visitor->expression_binary_op, visitor);
			mCc_ast_visit_expression(expression->lhs, visitor);
			mCc_ast_visit_expression(expression->rhs, visitor);
			visit_if_post_order(expression, visitor->expression_binary_op, visitor);
			break;

		case MCC_AST_EXPRESSION_TYPE_PARENTH:
			visit_if_pre_order(expression, visitor->expression_parenth, visitor);
			mCc_ast_visit_expression(expression->expression, visitor);
			visit_if_post_order(expression, visitor->expression_parenth, visitor);
			break;
		case MCC_AST_EXPRESSION_TYPE_UNARY_OP:
			visit_if_pre_order(expression, visitor->expression_unary_op, visitor);
			mCc_ast_visit_expression(expression->u_rhs, visitor);
			visit_if_post_order(expression, visitor->expression_binary_op, visitor);
			break;
	}

	visit_if_post_order(expression, visitor->expression, visitor);
}

void mCc_ast_visit_literal(struct mCc_ast_literal *literal,
                           struct mCc_ast_visitor *visitor)
{
	assert(literal);
	assert(visitor);

	visit_if_pre_order(literal, visitor->literal, visitor);

	switch (literal->type) {
                case MCC_AST_LITERAL_TYPE_INT:
                        visit(literal, visitor->literal_int, visitor);
                        break;

                case MCC_AST_LITERAL_TYPE_FLOAT:
                        visit(literal, visitor->literal_float, visitor);
                        break;

                case MCC_AST_LITERAL_TYPE_BOOL:
                        visit(literal, visitor->literal_bool, visitor);
                        break;

                case MCC_AST_LITERAL_TYPE_ALPHA:
                        visit(literal, visitor->literal_alpha, visitor);
                        break;

                case MCC_AST_LITERAL_TYPE_ALPHA_NUM:
                        visit(literal, visitor->literal_alpha_num, visitor);
                        break;

                case MCC_AST_LITERAL_TYPE_DIGIT:
                        visit(literal, visitor->literal_digit, visitor);
                        break;

                case MCC_AST_LITERAL_TYPE_IDENTIFIER:
                        visit(literal, visitor->literal_identifier, visitor);
                        break;

                case MCC_AST_LITERAL_TYPE_STRING:
                        visit(literal, visitor->literal_string, visitor);
                        break;
        }

	visit_if_post_order(literal, visitor->literal, visitor);
}

void mCc_ast_visit_statement(struct mCc_ast_statement *statement,
			     struct mCc_ast_visitor *visitor)
{
	assert(statement);
	assert(visitor);

	visit_if_pre_order(statement, visitor->statement, visitor);

	switch (statement->type) {
		case MCC_AST_STATEMENT_TYPE_DECLARATION:
			visit_if_pre_order(statement, visitor->statement_declaration, visitor);
                        mCc_ast_visit_declaration(statement->declaration, visitor);
			visit_if_post_order(statement, visitor->statement_declaration, visitor);
                        break;
                case MCC_AST_STATEMENT_TYPE_ASSIGNMENT:
                        visit_if_pre_order(statement, visitor->statement_assignment, visitor);
                        mCc_ast_visit_assignment(statement->assignment, visitor);
                        visit_if_post_order(statement, visitor->statement_assignment, visitor);
                        break;
                case MCC_AST_STATEMENT_TYPE_EXPRESSION:
                        visit_if_pre_order(statement, visitor->statement_expression, visitor);
                        mCc_ast_visit_expression(statement->expression, visitor);
                        visit_if_post_order(statement, visitor->statement_expression, visitor);
                        break;
                case MCC_AST_STATEMENT_TYPE_COMPOUND:
                        visit_if_pre_order(statement, visitor->statement_compound, visitor);
                        mCc_ast_visit_statement(statement->statement, visitor);
                        visit_if_post_order(statement, visitor->statement_compound, visitor);
                        break;
                case MCC_AST_STATEMENT_TYPE_COMPOUND_EMPTY:
                        visit(statement, visitor->statement_compound_empty, visitor);
                        break;
                case MCC_AST_STATEMENT_TYPE_RETURN:
                        visit_if_pre_order(statement, visitor->statement_return, visitor);
                        mCc_ast_visit_expression(statement->expression, visitor);
                        visit_if_post_order(statement, visitor->statement_return, visitor);
                        break;
                case MCC_AST_STATEMENT_TYPE_RETURN_EMPTY:
                        visit(statement, visitor->statement_return_empty, visitor);
                        break;
                case MCC_AST_STATEMENT_TYPE_WHILE:
                        visit_if_pre_order(statement, visitor->statement_while, visitor);
                        mCc_ast_visit_expression(statement->expr, visitor);
                        mCc_ast_visit_statement(statement->stmt, visitor);
                        visit_if_post_order(statement, visitor->statement_while, visitor);
                        break;
                case MCC_AST_STATEMENT_TYPE_IF:
                        visit_if_pre_order(statement, visitor->statement_if, visitor);
                        mCc_ast_visit_expression(statement->expr, visitor);
                        mCc_ast_visit_statement(statement->stmt, visitor);
                        visit_if_post_order(statement, visitor->statement_if, visitor);
                        break;
                case MCC_AST_STATEMENT_TYPE_IF_ELSE:
                        visit_if_pre_order(statement, visitor->statement_if_else, visitor);
                        mCc_ast_visit_expression(statement->expr, visitor);
                        mCc_ast_visit_statement(statement->if_else_stmt.stmt_1, visitor);
                        mCc_ast_visit_statement(statement->if_else_stmt.stmt_2, visitor);
                        visit_if_post_order(statement, visitor->statement_if_else, visitor);
                        break;

	}

	visit_if_post_order(statement, visitor->statement, visitor);
}

void mCc_ast_visit_declaration(struct mCc_ast_declaration *declaration,
			       struct mCc_ast_visitor *visitor)
{
	assert(declaration);
	assert(visitor);
	visit_if_pre_order(declaration, visitor->declaration, visitor);
	switch(declaration->type) {
		case MCC_AST_DECLARATION_TYPE_NORMAL:
			mCc_ast_visit_literal(declaration->normal_decl.identifier, visitor);
			break;
		case MCC_AST_DECLARATION_TYPE_ARRAY:
			mCc_ast_visit_literal(declaration->array_decl.identifier, visitor);
			mCc_ast_visit_literal(declaration->array_decl.size, visitor);
			break;
	}
	visit_if_post_order(declaration, visitor->declaration, visitor);
}

void mCc_ast_visit_assignment(struct mCc_ast_assignment *assignment,
                              struct mCc_ast_visitor *visitor)
{
        assert(assignment);
        assert(visitor);
        visit_if_pre_order(assignment, visitor->assignment, visitor);
        switch(assignment->type){
                case MCC_AST_ASSIGNMENT_TYPE_NORMAL:
                        mCc_ast_visit_literal(assignment->identifier, visitor);
                        mCc_ast_visit_expression(assignment->normal_asmt.rhs, visitor);
                        break;
                case MCC_AST_ASSIGNMENT_TYPE_ARRAY:
                        mCc_ast_visit_literal(assignment->identifier, visitor);
                        mCc_ast_visit_expression(assignment->array_asmt.index, visitor);
                        mCc_ast_visit_expression(assignment->array_asmt.rhs, visitor);
                        break;
        }
        visit_if_post_order(assignment, visitor->assignment, visitor);

}

void mCc_ast_visit_parameter(struct mCc_ast_parameter *parameter,
                             struct mCc_ast_visitor *visitor)
{
        assert(parameter);
        assert(visitor);

        visit_if_pre_order(parameter, visitor->parameter, visitor);
        mCc_ast_visit_declaration(parameter->declaration, visitor);
        if(parameter->next != NULL){
                mCc_ast_visit_parameter(parameter->next, visitor);
        }
        visit_if_post_order(parameter, visitor->parameter, visitor);
}

void mCc_ast_visit_arguments(struct mCc_ast_argument_list *arguments,
			     struct mCc_ast_visitor *visitor)
{
	assert(arguments);
	assert(visitor);

	visit_if_pre_order(arguments, visitor->arguments, visitor);
	mCc_ast_visit_expression(arguments->expression, visitor);
	if(arguments->next != NULL){
		mCc_ast_visit_arguments(arguments->next, visitor);
	}
	visit_if_post_order(arguments, visitor->arguments, visitor);
}

void mCc_ast_visit_function_def(struct mCc_ast_function_def *function_def,
                            struct mCc_ast_visitor *visitor)
{
        assert(function_def);
        assert(visitor);

        visit_if_pre_order(function_def, visitor->function_def, visitor);
        mCc_ast_visit_literal(function_def->identifier, visitor);
        if(function_def->parameters != NULL){
                mCc_ast_visit_parameter(function_def->parameters, visitor);
        }
        mCc_ast_visit_statement(function_def->compound_stmt, visitor);
        visit_if_post_order(function_def, visitor->function_def, visitor);
}

void mCc_ast_visit_function_def_list(struct mCc_ast_function_def_list *function_def_list,
                                     struct mCc_ast_visitor *visitor)
{
        assert(function_def_list);
        assert(visitor);

        visit_if_pre_order(function_def_list, visitor->function_def_list, visitor);
        mCc_ast_visit_function_def(function_def_list->function_def, visitor);
        if(function_def_list->next != NULL){
                mCc_ast_visit_function_def_list(function_def_list->next, visitor);
        }
        visit_if_post_order(function_def_list, visitor->function_def_list, visitor);
}

void mCc_ast_visit_program(struct mCc_ast_program *program,
                           struct mCc_ast_visitor *visitor)
{
        assert(program);
        assert(visitor);

        visit_if_pre_order(program, visitor->program, visitor);
        mCc_ast_visit_function_def_list(program->function_def_list, visitor);
        visit_if_post_order(program, visitor->program, visitor);
}