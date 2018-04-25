#include "mCc/ast_visit.h"

#include <assert.h>

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
			//TODO here
//			visit_if_pre_order(expression, visitor->expression_binary_op, visitor);
//						mCc_ast_visit_expression(expression->lhs, visitor);
//						mCc_ast_visit_expression(expression->rhs, visitor);
//						visit_if_post_order(expression, visitor->expression_binary_op, visitor);
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
                        switch(statement->declaration->type){
                                case MCC_AST_DECLARATION_TYPE_NORMAL:
                                        visit_if_pre_order(statement, visitor->statement_declaration, visitor);
                                        mCc_ast_visit_literal(statement->declaration->normal_decl.identifier, visitor);
                                        visit_if_post_order(statement, visitor->statement_declaration, visitor);
                                        break;
                                case MCC_AST_DECLARATION_TYPE_ARRAY:
                                        visit_if_pre_order(statement, visitor->statement_declaration, visitor);
                                        mCc_ast_visit_literal(statement->declaration->array_decl.identifier, visitor);
                                        mCc_ast_visit_literal(statement->declaration->array_decl.size, visitor);
                                        visit_if_post_order(statement, visitor->statement_declaration, visitor);
                                        break;
                        }
                        break;
                case MCC_AST_STATEMENT_TYPE_ASSIGNMENT:
			switch(statement->assignment->type){
				case MCC_AST_ASSIGNMENT_TYPE_NORMAL:
					visit_if_pre_order(statement, visitor->statement_assignment, visitor);
					mCc_ast_visit_literal(statement->assignment->identifier, visitor);
					mCc_ast_visit_expression(statement->assignment->normal_asmt.rhs, visitor);
					visit_if_post_order(statement, visitor->statement_declaration, visitor);
					break;
				case MCC_AST_ASSIGNMENT_TYPE_ARRAY:
					visit_if_pre_order(statement, visitor->statement_assignment, visitor);
					mCc_ast_visit_literal(statement->assignment->identifier, visitor);
                                        mCc_ast_visit_expression(statement->assignment->array_asmt.index, visitor);
					mCc_ast_visit_expression(statement->assignment->array_asmt.rhs, visitor);
					visit_if_post_order(statement, visitor->statement_declaration, visitor);
					break;
			}
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