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
	ASSERT_EQ(MCC_AST_BINARY_OP_ADD, expr->add_op);

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
/*
TEST(Parser, NestedExpression_1)
{
	const char input[] = "42 * (-192 + 3.14)";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto expr = result.expression;

	// root
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->type);
	ASSERT_EQ(MCC_AST_BINARY_OP_MUL, expr->mul_op);

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
	ASSERT_EQ(MCC_AST_BINARY_OP_ADD, subexpr->add_op);

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

TEST(Parser, NestedExpression_2)
{
	const char input[] = "-192 + 3.14 * 42";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto expr = result.expression;

	// root
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->type);
	ASSERT_EQ(MCC_AST_BINARY_OP_ADD, expr->add_op);

	// root -> lhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, expr->lhs->type);

	// root -> lhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, expr->lhs->literal->type);
	ASSERT_EQ(-192, expr->lhs->literal->i_value);

	// root -> rhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->rhs->type);

	auto subexpr = expr->rhs;

	// subexpr
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_BINARY_OP, subexpr->type);
	ASSERT_EQ(MCC_AST_BINARY_OP_MUL, subexpr->mul_op);

	// subexpr -> lhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, subexpr->lhs->type);

	// subexpr -> lhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_FLOAT, subexpr->lhs->literal->type);
	ASSERT_EQ(3.14, subexpr->lhs->literal->f_value);

	// subexpr -> rhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, subexpr->rhs->type);

	// subexpr -> rhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, subexpr->rhs->literal->type);
	ASSERT_EQ(42, subexpr->rhs->literal->i_value);

	mCc_ast_delete_expression(expr);
}
*/
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
	ASSERT_EQ(MCC_AST_BINARY_OP_GRT, expr->compare_op);

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
	ASSERT_EQ(MCC_AST_BINARY_OP_SMT, expr->compare_op);

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
	ASSERT_EQ(MCC_AST_BINARY_OP_GRE, expr->compare_op);

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
	ASSERT_EQ(MCC_AST_BINARY_OP_SME, expr->compare_op);

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
	ASSERT_EQ(MCC_AST_BINARY_OP_EQ, expr->compare_op);

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

TEST(Parser, Equal_2)
{
	const char input[] = "1 - 2 == 1";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto expr = result.expression;

	//root
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->type);
	ASSERT_EQ(MCC_AST_BINARY_OP_EQ, expr->compare_op);

	// root -> rhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, expr->rhs->type);

	// root -> rhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, expr->rhs->literal->type);
	ASSERT_EQ(1, expr->rhs->literal->i_value);

	// root -> lhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->lhs->type);

	auto subexpr = expr->lhs;

	// subexpr
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_BINARY_OP, subexpr->type);
	ASSERT_EQ(MCC_AST_BINARY_OP_SUB, subexpr->add_op);

	// subexpr -> lhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, subexpr->lhs->type);

	// subexpr -> lhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, subexpr->lhs->literal->type);
	ASSERT_EQ(1, subexpr->lhs->literal->i_value);

	// subexpr -> rhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, subexpr->rhs->type);

	// subexpr -> rhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, subexpr->rhs->literal->type);
	ASSERT_EQ(2, subexpr->rhs->literal->i_value);

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
	ASSERT_EQ(MCC_AST_BINARY_OP_UEQ, expr->compare_op);

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

TEST(Parser, Bool_1)
{
	const char input[] = "1 == true";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto expr = result.expression;

	//root
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->type);
	ASSERT_EQ(MCC_AST_BINARY_OP_EQ, expr->compare_op);

	// root -> lhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, expr->lhs->type);

	// root -> lhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, expr->lhs->literal->type);
	ASSERT_EQ(1, expr->lhs->literal->i_value);

	// root -> rhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, expr->rhs->type);

	// root -> rhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_BOOL, expr->rhs->literal->type);
	ASSERT_EQ(true, expr->rhs->literal->b_value);

	mCc_ast_delete_expression(expr);
}

TEST(Parser, Identifier_1)
{
	const char input[] = "my_var + 1";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto expr = result.expression;

	//root
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->type);
	ASSERT_EQ(MCC_AST_BINARY_OP_ADD, expr->add_op);

	// root -> lhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, expr->lhs->type);

	// root -> lhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_IDENTIFIER, expr->lhs->literal->type);
	ASSERT_STREQ("my_var", expr->lhs->literal->id_value);

	// root -> rhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, expr->rhs->type);

	// root -> rhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, expr->rhs->literal->type);
	ASSERT_EQ(1, expr->rhs->literal->i_value);

	mCc_ast_delete_expression(expr);
}

TEST(Parser, String_1)
{
	const char input[] = "x == \"This is a string\"";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto expr = result.expression;

	//root
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->type);

	// root -> rhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, expr->rhs->type);

	// root -> rhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_STRING, expr->rhs->literal->type);
	ASSERT_STREQ("\"This is a string\"", expr->rhs->literal->s_value);

	mCc_ast_delete_expression(expr);
}

TEST(Parser, Unary_1)
{
	const char input[] = " -5";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto expr = result.expression;

	// root

	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_UNARY_OP, expr->type);
	ASSERT_EQ(MCC_AST_UNARY_OP_MINUS, expr->unary_op);

	// root -> rhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, expr->u_rhs->type);

	//root -> rhs -> literal 
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, expr->u_rhs->literal->type);
	ASSERT_EQ(5,expr->u_rhs->literal->i_value);

	mCc_ast_delete_expression(expr);
}

/* ------------------------Declaration/Assignment */

TEST(Parser, Declaration_1)
{
	const char input[] = "int x;";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto stmt = result.statement;

	// root
	ASSERT_EQ(MCC_AST_STATEMENT_TYPE_DECLARATION, stmt->type);

	// root->declaration
	auto decl = stmt->declaration;
	ASSERT_EQ(MCC_AST_DECLARATION_TYPE_NORMAL, decl->type);
	ASSERT_EQ(MCC_AST_VARIABLES_TYPE_INT, decl->var_type);

	// decl->normal_decl->identifier;
	auto normal_decl = decl->normal_decl;
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_IDENTIFIER, normal_decl.identifier->type);
	ASSERT_STREQ("x", normal_decl.identifier->id_value);

	mCc_ast_delete_statement(stmt);
}

TEST(Parser, Declaration_2)
{
	const char input[] = "float[10] x;";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto stmt = result.statement;

	// root
	ASSERT_EQ(MCC_AST_STATEMENT_TYPE_DECLARATION, stmt->type);

	// root->declaration
	auto decl = stmt->declaration;
	ASSERT_EQ(MCC_AST_DECLARATION_TYPE_ARRAY, decl->type);
	ASSERT_EQ(MCC_AST_VARIABLES_TYPE_FLOAT, decl->var_type);

	// decl->array_decl->identifier;
	auto array_decl = decl->array_decl;
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_IDENTIFIER, array_decl.identifier->type);
	ASSERT_STREQ("x", array_decl.identifier->id_value);

	// decl->array_decl->size
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, array_decl.size->type);
	ASSERT_EQ(10, array_decl.size->i_value);

//	mCc_ast_delete_declaration(decl);
	mCc_ast_delete_statement(stmt);
}

TEST(Parser, Assignment_1)
{
	const char input[] = "x = 1+2;";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto stmt = result.statement;

	// root
	ASSERT_EQ(MCC_AST_STATEMENT_TYPE_ASSIGNMENT, stmt->type);

	// root->assignment
	auto asmt = stmt->assignment;
	ASSERT_EQ(MCC_AST_ASSIGNMENT_TYPE_NORMAL, asmt->type);

	// asmt->identifier;
	auto id = asmt->identifier;

	ASSERT_EQ(MCC_AST_LITERAL_TYPE_IDENTIFIER, id->type);
	ASSERT_STREQ("x", id->id_value);

	// asmt->normal_asmt->rhs
	auto expr = asmt->normal_asmt.rhs;

	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_BINARY_OP, expr->type);
	ASSERT_EQ(MCC_AST_BINARY_OP_ADD, expr->add_op);

	//expr->lhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, expr->lhs->type);

	// expr -> lhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, expr->lhs->literal->type);
	ASSERT_EQ(1, expr->lhs->literal->i_value);

	// expr -> rhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, expr->rhs->type);

	// expr -> rhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, expr->rhs->literal->type);
	ASSERT_EQ(2, expr->rhs->literal->i_value);

	mCc_ast_delete_statement(stmt);
}

TEST(Parser, Assignment_2)
{
	const char input[] = "arr[1] = 0;";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto stmt = result.statement;

	// root
	ASSERT_EQ(MCC_AST_STATEMENT_TYPE_ASSIGNMENT, stmt->type);

	// root->assignment
	auto asmt = stmt->assignment;
	ASSERT_EQ(MCC_AST_ASSIGNMENT_TYPE_ARRAY, asmt->type);

	// asmt->identifier;
	auto id = asmt->identifier;

	ASSERT_EQ(MCC_AST_LITERAL_TYPE_IDENTIFIER, id->type);
	ASSERT_STREQ("arr", id->id_value);

	// asmt->normal_asmt->rhs
	auto expr = asmt->array_asmt.rhs;

	// expr -> lhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, expr->literal->type);
	ASSERT_EQ(0, expr->literal->i_value);

	mCc_ast_delete_statement(stmt);
}
/* ------------------------Statements */

TEST(Parser, Stmt_Expression_1)
{
	const char input[] = "2 + 1;";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto stmt = result.statement;

	//root (expression SEMICOLON)
	ASSERT_EQ(MCC_AST_STATEMENT_TYPE_EXPRESSION, stmt->type);

	//root->expression
	auto sub_expr = stmt->expression;
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_BINARY_OP, sub_expr->type);
	ASSERT_EQ(MCC_AST_BINARY_OP_ADD, sub_expr->add_op);

	// sub_expr -> lhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, sub_expr->lhs->type);

	// sub_expr -> lhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, sub_expr->lhs->literal->type);
	ASSERT_EQ(2, sub_expr->lhs->literal->i_value);

	// sub_expr -> rhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, sub_expr->rhs->type);

	// sub_expr -> rhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, sub_expr->rhs->literal->type);
	ASSERT_EQ(1, sub_expr->rhs->literal->i_value);

	mCc_ast_delete_statement(stmt);
}

TEST(Parser, Stmt_Compound_1)
{
	const char input[] = "{var = 1;}";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto stmt = result.statement;

	//root
	ASSERT_EQ(MCC_AST_STATEMENT_TYPE_COMPOUND, stmt->type);

	//root->statement (assignment SEMICOLON)
	auto sub_stmt = stmt->statement;
	ASSERT_EQ(MCC_AST_STATEMENT_TYPE_ASSIGNMENT, sub_stmt->type);

	//root->statement->assignment
	auto asmt = sub_stmt->assignment;
	ASSERT_EQ(MCC_AST_ASSIGNMENT_TYPE_NORMAL, asmt->type);

	// asmt -> lhs
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_IDENTIFIER, asmt->identifier->type);
	ASSERT_STREQ("var", asmt->identifier->id_value);

	// asmt -> rhs
	auto expr = asmt->normal_asmt.rhs;

	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, expr->type);
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, expr->literal->type);
	ASSERT_EQ(1, expr->literal->i_value);

	mCc_ast_delete_statement(stmt);
}

TEST(Parser, Stmt_Compound_2)
{
	const char input[] = "{}";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto stmt = result.statement;

	//root
	ASSERT_EQ(MCC_AST_STATEMENT_TYPE_COMPOUND_EMPTY, stmt->type);

	// nothing more
	mCc_ast_delete_statement(stmt);
}

TEST(Parser, Return_1)
{
	const char input[] = "return;";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto stmt = result.statement;

	//root
	ASSERT_EQ(MCC_AST_STATEMENT_TYPE_RETURN_EMPTY, stmt->type);

	// nothing more
	mCc_ast_delete_statement(stmt);
}

TEST(Parser, Return_2)
{
	const char input[] = "return 0;";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto stmt = result.statement;

	//root
	ASSERT_EQ(MCC_AST_STATEMENT_TYPE_RETURN, stmt->type);

	//root->expression
	auto expr = stmt->expression;

	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, expr->type);
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, expr->literal->type);
	ASSERT_EQ(0, expr->literal->i_value);

	// nothing more
	mCc_ast_delete_statement(stmt);
}

TEST(Parser, While_1)
{
	const char input[] = "while (flag) {}";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto stmt = result.statement;

	//root
	ASSERT_EQ(MCC_AST_STATEMENT_TYPE_WHILE, stmt->type);

	//root->expression
	auto expr = stmt->expr;

	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, expr->type);
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_IDENTIFIER, expr->literal->type);
	ASSERT_STREQ("flag", expr->literal->id_value);

	//root->statement
	auto substmt = stmt->stmt;

	ASSERT_EQ(MCC_AST_STATEMENT_TYPE_COMPOUND_EMPTY, substmt->type);

	// nothing more
	mCc_ast_delete_statement(stmt);
}

TEST(Parser, If_1)
{
	const char input[] = "if (c3 == 0) c3 = 2;";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto stmt = result.statement;

	//root
	ASSERT_EQ(MCC_AST_STATEMENT_TYPE_IF, stmt->type);

	//root->expression
	auto expr = stmt->expr;

	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_PARENTH, expr->type);

	//expression inside parentheses
	auto subexpr = expr->expression;

	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_BINARY_OP, subexpr->type);
	ASSERT_EQ(MCC_AST_BINARY_OP_EQ, subexpr->compare_op);

	// subexpr -> lhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, subexpr->lhs->type);

	// subexpr -> lhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_IDENTIFIER, subexpr->lhs->literal->type);
	ASSERT_STREQ("c3", subexpr->lhs->literal->id_value);

	// subexpr -> rhs
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, subexpr->rhs->type);

	// subexpr -> rhs -> literal
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, subexpr->rhs->literal->type);
	ASSERT_EQ(0, subexpr->rhs->literal->i_value);

	// root->if statement after parentheses
	auto if_stmt = stmt->stmt;
	ASSERT_EQ(MCC_AST_STATEMENT_TYPE_ASSIGNMENT, if_stmt->type);

	// if_stmt->assignment
	auto asmt = if_stmt->assignment;
	ASSERT_EQ(MCC_AST_ASSIGNMENT_TYPE_NORMAL, asmt->type);

	// asmt->identifier;
	auto id = asmt->identifier;

	ASSERT_EQ(MCC_AST_LITERAL_TYPE_IDENTIFIER, id->type);
	ASSERT_STREQ("c3", id->id_value);

	// asmt->normal_asmt->rhs
	auto subexpr_asmt = asmt->normal_asmt.rhs;
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, subexpr_asmt->type);
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_INT, subexpr_asmt->literal->type);
	ASSERT_EQ(2, subexpr_asmt->literal->i_value);

	mCc_ast_delete_statement(stmt);
}

/* ------------------------Function Definition/Call */
/*
TEST(Parser, Parameter_2)
{
	const char input[] = "int x, int y";
	auto result = mCc_parser_parse_string(input);

//	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto stmt = result.parameter;

	//root

	// nothing more
	mCc_ast_delete_parameter(stmt);
}

TEST(Parser, Function_1)
{
	const char input[] = "void foo() {bool flag;}";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto stmt = result.function_def;

	//root

	mCc_ast_delete_function_def(stmt);
}

TEST(Parser, Function_2)
{
	const char input[] = "string foo2(int x, float y) {}";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto stmt = result.function_def;

	//root

	mCc_ast_delete_function_def(stmt);
}
*/

TEST(Parser, Call_Expr_1)
{
	const char input[] = "foo()";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto expr = result.expression;

	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_CALL_EXPR, expr->type);

	auto call_expr = expr->call_expr;
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_IDENTIFIER, call_expr.identifier->type);
	ASSERT_STREQ("foo", call_expr.identifier->id_value);

	mCc_ast_delete_expression(expr);
}

TEST(Parser, Call_Expr_2)
{
	const char input[] = "foo(a, 6.2)";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto expr = result.expression;

	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_CALL_EXPR, expr->type);

	auto call_expr = expr->call_expr;

	//call_expr.identifier
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_IDENTIFIER, call_expr.identifier->type);
	ASSERT_STREQ("foo", call_expr.identifier->id_value);

	// call_expr->expression
	auto args = call_expr.arguments;
	auto args_expr = args->expression;

	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, args_expr->type);
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_IDENTIFIER, args_expr->literal->type);
	ASSERT_STREQ("a", args_expr->literal->id_value);

	// call_expr->next->expression
	auto next_expr = args->next->expression;

	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, next_expr->type);
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_FLOAT, next_expr->literal->type);
	ASSERT_EQ(6.2, next_expr->literal->f_value);

	mCc_ast_delete_expression(expr);
}

TEST(Parser, Call_Expr_3)
{
	const char input[] = "foo(";
	auto result = mCc_parser_parse_string(input);

	ASSERT_NE(MCC_PARSER_STATUS_OK, result.status);
}
