#include <gtest/gtest.h>

#include "mCc/ast_print.h"
#include "mCc/ast.h"
#include "mCc/parser.h"


TEST(dot_printer, PrintIntLiteral_1)
{
  struct mCc_ast_literal *lit = mCc_ast_new_literal_int(5);
  FILE *output = fopen("int_literal.dot", "w");
  mCc_ast_print_dot_literal(output, lit);
  fclose(output);
  mCc_ast_delete_literal(lit);
}

TEST(dot_printer, PrintFloatLiteral_1)
{
  struct mCc_ast_literal *lit = mCc_ast_new_literal_float(4.4);
  FILE *output = fopen("float_literal.dot", "w");
  mCc_ast_print_dot_literal(output, lit);
  fclose(output);
  mCc_ast_delete_literal(lit);
}

TEST(dot_printer, PrintBoolLiteral_1)
{
  struct mCc_ast_literal *lit = mCc_ast_new_literal_bool(true);
  FILE *output = fopen("bool_literal.dot", "w");
  mCc_ast_print_dot_literal(output, lit);
  fclose(output);
  mCc_ast_delete_literal(lit);
}

TEST(dot_printer, PrintStringLiteral_1)
{
  struct mCc_ast_literal *lit = mCc_ast_new_literal_string("\"my string\"");
  FILE *output = fopen("string_literal.dot", "w");
  mCc_ast_print_dot_literal(output, lit);
  fclose(output);
  mCc_ast_delete_literal(lit);
}

TEST(dot_printer, PrintBinaryOp_1)
{
  struct mCc_ast_literal *lhs = mCc_ast_new_literal_int(5);
  struct mCc_ast_literal *rhs = mCc_ast_new_literal_int(4);
  struct mCc_ast_expression *expr = mCc_ast_new_expression_compare_op(MCC_AST_BINARY_OP_UEQ, mCc_ast_new_expression_literal(lhs), mCc_ast_new_expression_literal(rhs));
  FILE *output = fopen("binary_op.dot", "w");
  mCc_ast_print_dot_expression(output, expr);
  fclose(output);
  mCc_ast_delete_expression(expr);
}

TEST(dot_print, PrintUnaryOp_1)
{
  struct mCc_ast_literal *rhs = mCc_ast_new_literal_int(5);
  struct mCc_ast_expression *expr = mCc_ast_new_expression_unary_op(MCC_AST_UNARY_OP_MINUS, mCc_ast_new_expression_literal(rhs));
  FILE *output = fopen("unary_op.dot", "w");
  mCc_ast_print_dot_expression(output, expr);
  fclose(output);
  mCc_ast_delete_expression(expr);
}

TEST(dot_print, PrintParenthesis_1)
{
  struct mCc_ast_literal *lhs = mCc_ast_new_literal_int(5);
  struct mCc_ast_literal *rhs = mCc_ast_new_literal_int(4);
  struct mCc_ast_expression *expr = mCc_ast_new_expression_compare_op(
          MCC_AST_BINARY_OP_UEQ,
          mCc_ast_new_expression_literal(lhs),
          mCc_ast_new_expression_literal(rhs)
  );
  struct mCc_ast_expression *parenth = mCc_ast_new_expression_parenth(expr);
  FILE *output = fopen("parenthesis.dot", "w");
  mCc_ast_print_dot_expression(output, parenth);
  fclose(output);
  mCc_ast_delete_expression(parenth);
}

TEST(dot_print, PrintNested_1)
{
  const char input[] = "(-192 + 3.14 * 42 == 1) == false";
  auto result = mCc_parser_parse_string(input);

  auto expr = result.expression;
  FILE *output = fopen("nested.dot", "w");
  mCc_ast_print_dot_expression(output, expr);
  fclose(output);
  mCc_ast_delete_expression(expr);
}

TEST(dot_print, PrintNested_2)
{
  const char input[] = "my_string == \"my_string\"";
  auto result = mCc_parser_parse_string(input);

  auto expr = result.expression;
  FILE *output = fopen("nested2.dot", "w");
  mCc_ast_print_dot_expression(output, expr);
  fclose(output);
  mCc_ast_delete_expression(expr);
}

TEST(dot_print, PrintDeclaration_1)
{
  const char input[] = "int x;";
  auto result = mCc_parser_parse_string(input);

  auto stmt = result.statement;
  FILE *output = fopen("declaration.dot", "w");
  mCc_ast_print_dot_statement(output, stmt);
  fclose(output);
  mCc_ast_delete_statement(stmt);
}

TEST(dot_print, PrintDeclaration_2)
{
  const char input[] = "float[10] x;";
  auto result = mCc_parser_parse_string(input);

  auto stmt = result.statement;
  FILE *output = fopen("declaration2.dot", "w");
  mCc_ast_print_dot_statement(output, stmt);
  fclose(output);
  mCc_ast_delete_statement(stmt);
}

TEST(dot_print, PrintAssignment_1)
{
  const char input[] = "x = 1+2;";
  auto result = mCc_parser_parse_string(input);

  auto stmt = result.statement;
  FILE *output = fopen("assignment.dot", "w");
  mCc_ast_print_dot_statement(output, stmt);
  fclose(output);
  mCc_ast_delete_statement(stmt);
}

TEST(dot_print, PrintAssignment_2)
{
  const char input[] = "arr[1] = 0;";
  auto result = mCc_parser_parse_string(input);

  auto stmt = result.statement;
  FILE *output = fopen("assignment2.dot", "w");
  mCc_ast_print_dot_statement(output, stmt);
  fclose(output);
  mCc_ast_delete_statement(stmt);
}

TEST(dot_print, PrintStatementExpression_1)
{
  const char input[] = "2 + 1;";
  auto result = mCc_parser_parse_string(input);

  auto stmt = result.statement;
  FILE *output = fopen("statementExpression.dot", "w");
  mCc_ast_print_dot_statement(output, stmt);
  fclose(output);
  mCc_ast_delete_statement(stmt);
}

TEST(dot_print, PrintStatementCompound_1)
{
  const char input[] = "{var = 1;}";
  auto result = mCc_parser_parse_string(input);

  auto stmt = result.statement;
  FILE *output = fopen("statementCompound.dot", "w");
  mCc_ast_print_dot_statement(output, stmt);
  fclose(output);
  mCc_ast_delete_statement(stmt);
}

TEST(dot_print, PrintReturnStatement_1)
{
  const char input[] = "return 0;";
  auto result = mCc_parser_parse_string(input);

  auto stmt = result.statement;
  FILE *output = fopen("returnStatement.dot", "w");
  mCc_ast_print_dot_statement(output, stmt);
  fclose(output);
  mCc_ast_delete_statement(stmt);
}

TEST(dot_print, PrintReturnStatement_2)
{
  const char input[] = "return;";
  auto result = mCc_parser_parse_string(input);

  auto stmt = result.statement;
  FILE *output = fopen("returnStatementEmpty.dot", "w");
  mCc_ast_print_dot_statement(output, stmt);
  fclose(output);
  mCc_ast_delete_statement(stmt);
}

TEST(dot_print, PrintWhileStatement_1)
{
  const char input[] = "while (flag) {}";
  auto result = mCc_parser_parse_string(input);

  auto stmt = result.statement;
  FILE *output = fopen("whileStatement.dot", "w");
  mCc_ast_print_dot_statement(output, stmt);
  fclose(output);
  mCc_ast_delete_statement(stmt);
}

TEST(dot_print, PrintIfStatement_1)
{
  const char input[] = "if (c3 == 0) c3 = 2;";
  auto result = mCc_parser_parse_string(input);

  auto stmt = result.statement;
  FILE *output = fopen("ifStatement.dot", "w");
  mCc_ast_print_dot_statement(output, stmt);
  fclose(output);
  mCc_ast_delete_statement(stmt);
}

TEST(dot_print, PrintIfElseStatement_1)
{
  const char input[] = "if (c3 == 0) c3 = 2; else c3 = 1;";
  auto result = mCc_parser_parse_string(input);

  auto stmt = result.statement;
  FILE *output = fopen("ifElseStatement.dot", "w");
  mCc_ast_print_dot_statement(output, stmt);
  fclose(output);
  mCc_ast_delete_statement(stmt);
}

TEST(dot_print, PrintParameter_1)
{
  const char input[] = "string _a3, float b4";
  auto result = mCc_parser_parse_string(input);

  auto param = result.parameter;
  FILE *output = fopen("parameter.dot", "w");
  mCc_ast_print_dot_parameter(output, param);
  fclose(output);
  mCc_ast_delete_parameter(param);
}

TEST(dot_print, PrintCallExpression_1)
{
  const char input[] = "foo()";
  auto result = mCc_parser_parse_string(input);

  auto expr = result.expression;
  FILE *output = fopen("callExpression.dot", "w");
  mCc_ast_print_dot_expression(output, expr);
  fclose(output);
  mCc_ast_delete_expression(expr);
}

TEST(dot_print, PrintCallExpression_2)
{
  const char input[] = "foo(a, b)";
  auto result = mCc_parser_parse_string(input);

  auto expr = result.expression;
  FILE *output = fopen("callExpression2.dot", "w");
  mCc_ast_print_dot_expression(output, expr);
  fclose(output);
  mCc_ast_delete_expression(expr);
}

TEST(dot_print, PrintFunctionDef_1)
{
  const char input[] = "void foo() {}";
  auto result = mCc_parser_parse_string(input);

  auto program = result.program;
  FILE *output = fopen("functionDef.dot", "w");
  mCc_ast_print_dot_function_def(output, program->function_def_list->function_def);
  fclose(output);
  mCc_ast_delete_program(program);
}

TEST(dot_print, PrintFunctionDef_2)
{
  const char input[] = "void foo(int a) {if(a==1){ x=2;}}";
  auto result = mCc_parser_parse_string(input);

  auto program = result.program;
  FILE *output = fopen("functionDef2.dot", "w");
  mCc_ast_print_dot_function_def(output, program->function_def_list->function_def);
  fclose(output);
  mCc_ast_delete_program(program);
}

TEST(dot_print, PrintFunctionDefList_1)
{
  const char input[] = "void main() { _sub(2, 3.2);} void _sub(int x, float y) {z = x - y;}";
  auto result = mCc_parser_parse_string(input);

  auto program = result.program;
  auto func_list = program->function_def_list;

  FILE *output = fopen("functionDefList.dot", "w");
  mCc_ast_print_dot_function_def_list(output, func_list);
  fclose(output);
  mCc_ast_delete_program(program);
}

TEST(dot_print, PrintProgram_1)
{
  const char input[] = "void main() { _sub(2, 3.2);} int _sub(int x, float y) {z = x - y;}";
  auto result = mCc_parser_parse_string(input);

  auto program = result.program;

  FILE *output = fopen("program.dot", "w");
  mCc_ast_print_dot_program(output, program);
  fclose(output);
  mCc_ast_delete_program(program);
}