#include <gtest/gtest.h>

#include "mCc/ast_print.h"


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