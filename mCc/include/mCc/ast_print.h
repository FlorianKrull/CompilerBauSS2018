#ifndef MCC_AST_PRINT_H
#define MCC_AST_PRINT_H

#include <stdio.h>

#include "mCc/ast.h"

#ifdef __cplusplus
extern "C" {
#endif

const char *mCc_ast_print_unary_op(enum mCc_ast_unary_op op);

const char *mCc_ast_print_binary_op(struct mCc_ast_expression *expression);

const char *mCc_ast_print_var_type(enum mCc_ast_var_type type);

const char *mCc_ast_print_declaration_type(enum mCc_ast_declaration_type type);

const char *mCc_ast_print_assignment_type(enum mCc_ast_assignment_type type);

/* ------------------------------------------------------------- DOT Printer */

void mCc_ast_print_dot_expression(FILE *out,
                                  struct mCc_ast_expression *expression);

void mCc_ast_print_dot_literal(FILE *out, struct mCc_ast_literal *literal);

void mCc_ast_print_dot_statement(FILE *out,
                                 struct mCc_ast_statement *statement);

#ifdef __cplusplus
}
#endif

#endif
