#include <gtest/gtest.h>

#include "mCc/ast.h"
#include "mCc/parser.h"

TEST(Parser, BinaryOp_1)
{
	const char input[] = "192 + 3.14";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto expr = result.expression;

	// root
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->type);
	ASSERT_EQ(MCC_AST_BINARY_OP_ADD, expr->op);

	// root -> lhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, expr->lhs->type);

	// root -> lhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, expr->lhs->literal->type);
	ASSERT_EQ(192, expr->lhs->literal->i_value);

	// root -> rhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, expr->rhs->type);

	// root -> rhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_FLOAT, expr->rhs->literal->type);
	ASSERT_EQ(3.14, expr->rhs->literal->f_value);

	mCc_ast_delete_expression(expr);
}

TEST(Parser, NestedExpression_1)
{
	const char input[] = "42 * (-192 + 3.14)";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto expr = result.expression;

	// root
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->type);
	ASSERT_EQ(MCC_AST_BINARY_OP_MUL, expr->op);

	// root -> lhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, expr->lhs->type);

	// root -> lhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, expr->lhs->literal->type);
	ASSERT_EQ(42, expr->lhs->literal->i_value);

	// root -> rhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_PARENTH, expr->rhs->type);

	auto subexpr = expr->rhs->expression;

	// subexpr
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_BINARY_OP, subexpr->type);
	ASSERT_EQ(MCC_AST_BINARY_OP_ADD, subexpr->op);

	// subexpr -> lhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, subexpr->lhs->type);

	// subexpr -> lhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, subexpr->lhs->literal->type);
	ASSERT_EQ(-192, subexpr->lhs->literal->i_value);

	// subexpr -> rhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, subexpr->rhs->type);

	// subexpr -> rhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_FLOAT, subexpr->rhs->literal->type);
	ASSERT_EQ(3.14, subexpr->rhs->literal->f_value);

	mCc_ast_delete_expression(expr);
}

TEST(Parser, MissingClosingParenthesis_1)
{
	const char input[] = "(42";
	auto result = mCc_parser_parse_string(input);

	ASSERT_NE(MCC_PARSER_STATUS_OK, result.status);
}

TEST(Parser, GreaterThan_1)
{
	const char input[] = "2 > 1";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto expr = result.expression;

	// root
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->type);
	ASSERT_EQ(MCC_AST_BINARY_OP_GRT, expr->op);

	// root -> lhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, expr->lhs->type);

	// root -> lhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, expr->lhs->literal->type);
	ASSERT_EQ(2, expr->lhs->literal->i_value);

	// root -> rhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, expr->rhs->type);

	// root -> rhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, expr->rhs->literal->type);
	ASSERT_EQ(1, expr->rhs->literal->i_value);

	mCc_ast_delete_expression(expr);
}

TEST(Parser, SmallerThan_1)
{
	const char input[] = "2 < 1";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto expr = result.expression;

	// root
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->type);
	ASSERT_EQ(MCC_AST_BINARY_OP_SMT, expr->op);

	// root -> lhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, expr->lhs->type);

	// root -> lhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, expr->lhs->literal->type);
	ASSERT_EQ(2, expr->lhs->literal->i_value);

	// root -> rhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, expr->rhs->type);

	// root -> rhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, expr->rhs->literal->type);
	ASSERT_EQ(1, expr->rhs->literal->i_value);

	mCc_ast_delete_expression(expr);
}

TEST(Parser, GreaterEqual_1)
{
	const char input[] = "2 >= 1";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto expr = result.expression;

	//root
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->type);
	ASSERT_EQ(MCC_AST_BINARY_OP_GRE, expr->op);

		// root -> lhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, expr->lhs->type);

	// root -> lhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, expr->lhs->literal->type);
	ASSERT_EQ(2, expr->lhs->literal->i_value);

	// root -> rhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, expr->rhs->type);

	// root -> rhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, expr->rhs->literal->type);
	ASSERT_EQ(1, expr->rhs->literal->i_value);

	mCc_ast_delete_expression(expr);
}

TEST(Parser, SmallerEqual_1)
{
	const char input[] = "2 <= 1";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto expr = result.expression;

	//root
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->type);
	ASSERT_EQ(MCC_AST_BINARY_OP_SME, expr->op);

		// root -> lhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, expr->lhs->type);

	// root -> lhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, expr->lhs->literal->type);
	ASSERT_EQ(2, expr->lhs->literal->i_value);

	// root -> rhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, expr->rhs->type);

	// root -> rhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, expr->rhs->literal->type);
	ASSERT_EQ(1, expr->rhs->literal->i_value);

	mCc_ast_delete_expression(expr);
}

TEST(Parser, And_1)
{
	const char input[] = "1 && 2";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto expr = result.expression;

	//root
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->type);
	ASSERT_EQ(MCC_AST_BINARY_OP_AND, expr->op);

		// root -> lhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, expr->lhs->type);

	// root -> lhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, expr->lhs->literal->type);
	ASSERT_EQ(1, expr->lhs->literal->i_value);

	// root -> rhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, expr->rhs->type);

	// root -> rhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, expr->rhs->literal->type);
	ASSERT_EQ(2, expr->rhs->literal->i_value);

	mCc_ast_delete_expression(expr);
}

TEST(Parser, Or_1)
{
	const char input[] = "1 || 2";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto expr = result.expression;

	//root
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->type);
	ASSERT_EQ(MCC_AST_BINARY_OP_OR, expr->op);

		// root -> lhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, expr->lhs->type);

	// root -> lhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, expr->lhs->literal->type);
	ASSERT_EQ(1, expr->lhs->literal->i_value);

	// root -> rhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, expr->rhs->type);

	// root -> rhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, expr->rhs->literal->type);
	ASSERT_EQ(2, expr->rhs->literal->i_value);

	mCc_ast_delete_expression(expr);
}

TEST(Parser, Equal_1)
{
	const char input[] = "1 == 2";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto expr = result.expression;

	//root
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->type);
	ASSERT_EQ(MCC_AST_BINARY_OP_EQ, expr->op);

		// root -> lhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, expr->lhs->type);

	// root -> lhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, expr->lhs->literal->type);
	ASSERT_EQ(1, expr->lhs->literal->i_value);

	// root -> rhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, expr->rhs->type);

	// root -> rhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, expr->rhs->literal->type);
	ASSERT_EQ(2, expr->rhs->literal->i_value);

	mCc_ast_delete_expression(expr);
}

TEST(Parser, Unequal_1)
{
	const char input[] = "1 != 2";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto expr = result.expression;

	//root
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->type);
	ASSERT_EQ(MCC_AST_BINARY_OP_UEQ, expr->op);

		// root -> lhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, expr->lhs->type);

	// root -> lhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, expr->lhs->literal->type);
	ASSERT_EQ(1, expr->lhs->literal->i_value);

	// root -> rhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, expr->rhs->type);

	// root -> rhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, expr->rhs->literal->type);
	ASSERT_EQ(2, expr->rhs->literal->i_value);

	mCc_ast_delete_expression(expr);
}