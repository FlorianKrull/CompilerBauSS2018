#include "mCc/ast_print.h"

#include <assert.h>

#include "mCc/ast_visit.h"

#define LABEL_SIZE 64

const char *mCc_ast_print_unary_op(enum mCc_ast_unary_op op)
{
	switch(op){
		case MCC_AST_UNARY_OP_MINUS: return "-";
		case MCC_AST_UNARY_OP_EXCLAM: return "!";
	}

	return "unknown op";
}

const char *mCc_ast_print_binary_op(struct mCc_ast_expression *expression)
{
    if(expression->binary_op_type == MCC_AST_BINARY_OP_TYPE_ADD){
      switch(expression->add_op){
        case MCC_AST_BINARY_OP_ADD: return "+";
        case MCC_AST_BINARY_OP_SUB: return "-";
      }
    }
    else if(expression->binary_op_type == MCC_AST_BINARY_OP_TYPE_MUL){
      switch(expression->mul_op){
        case MCC_AST_BINARY_OP_MUL: return "*";
        case MCC_AST_BINARY_OP_DIV: return "/";
      }
    }
    else if(expression->binary_op_type == MCC_AST_BINARY_OP_TYPE_BINARY){
      switch(expression->op){
        case MCC_AST_BINARY_OP_AND: return "&&";
        case MCC_AST_BINARY_OP_OR: return "||";
      }
    }
    else if(expression->binary_op_type == MCC_AST_BINARY_OP_TYPE_COMPARE){
      switch (expression->compare_op){
        case MCC_AST_BINARY_OP_SMT: return "<";
        case MCC_AST_BINARY_OP_GRT: return ">";
        case MCC_AST_BINARY_OP_GRE: return ">=";
        case MCC_AST_BINARY_OP_SME: return "<=";
        case MCC_AST_BINARY_OP_EQ: return "==";
        case MCC_AST_BINARY_OP_UEQ: return "!=";
      }
    }

	return "unknown op";
}

const char *mCc_ast_print_var_type(enum mCc_ast_type type)
{
        switch(type){
                case MCC_AST_TYPE_INT: return "int";
                case MCC_AST_TYPE_FLOAT: return "float";
                case MCC_AST_TYPE_BOOL: return "boolean";
                case MCC_AST_TYPE_STRING: return "string";
                case MCC_AST_TYPE_VOID: return "void";
        }

        return "unknown type";
}

const char *mCc_ast_print_declaration_type(enum mCc_ast_declaration_type type)
{
        switch(type){
                case MCC_AST_DECLARATION_TYPE_NORMAL: return "";
                case MCC_AST_DECLARATION_TYPE_ARRAY: return "[]";
        }

        return "unknown type";
}

const char *mCc_ast_print_assignment_type(enum mCc_ast_assignment_type type)
{
        switch(type){
                case MCC_AST_ASSIGNMENT_TYPE_NORMAL: return "";
                case MCC_AST_ASSIGNMENT_TYPE_ARRAY: return "[]";
        }

        return "unknown type";
}

const char *mCc_ast_print_function_type(enum mCc_ast_type type)
{
        switch(type){
                case MCC_AST_TYPE_INT: return "int";
                case MCC_AST_TYPE_FLOAT: return "float";
                case MCC_AST_TYPE_BOOL: return "bool";
                case MCC_AST_TYPE_STRING: return "string";
                case MCC_AST_TYPE_VOID: return "void";
        }

        return "unknown type";
}

/* ------------------------------------------------------------- DOT Printer */

static void print_dot_begin(FILE *out)
{
	assert(out);

	fprintf(out, "digraph \"AST\" {\n");
	fprintf(out, "\tnodesep=0.6\n");
}

static void print_dot_end(FILE *out)
{
	assert(out);

	fprintf(out, "}\n");
}

static void print_dot_node(FILE *out, const void *node, const char *label)
{
	assert(out);
	assert(node);
	assert(label);

	fprintf(out, "\t\"%p\" [shape=box, label=\"%s\"];\n", node, label);
}

static void print_dot_edge(FILE *out, const void *src_node,
                           const void *dst_node, const char *label)
{
	assert(out);
	assert(src_node);
	assert(dst_node);
	assert(label);

	fprintf(out, "\t\"%p\" -> \"%p\" [label=\"%s\"];\n", src_node, dst_node,
	        label);
}

static void print_dot_expression_literal(struct mCc_ast_expression *expression,
                                         void *data)
{
	assert(expression);
	assert(data);

	FILE *out = data;
	print_dot_node(out, expression, "expr: lit");
	print_dot_edge(out, expression, expression->literal, "literal");
}

static void
print_dot_expression_binary_op(struct mCc_ast_expression *expression,
                               void *data)
{
	assert(expression);
	assert(data);

	char label[LABEL_SIZE] = { 0 };
	snprintf(label, sizeof(label), "expr: %s",
	         mCc_ast_print_binary_op(expression));

	FILE *out = data;
	print_dot_node(out, expression, label);
	print_dot_edge(out, expression, expression->lhs, "lhs");
	print_dot_edge(out, expression, expression->rhs, "rhs");
}

static void
print_dot_expression_unary_op(struct mCc_ast_expression *expression,
				void *data)
{
	assert(expression);
	assert(data);

        char label[LABEL_SIZE] = { 0 };
	snprintf(label, sizeof(label), "expr: %s",
		 mCc_ast_print_unary_op(expression->unary_op));
	FILE *out = data;
	print_dot_node(out, expression, label);
	print_dot_edge(out, expression, expression->u_rhs, "rhs");

}

static void print_dot_expression_parenth(struct mCc_ast_expression *expression,
                                         void *data)
{
	assert(expression);
	assert(data);

	FILE *out = data;
	print_dot_node(out, expression, "( )");
	print_dot_edge(out, expression, expression->expression, "expression");
}

static void print_dot_literal_int(struct mCc_ast_literal *literal, void *data)
{
	assert(literal);
	assert(data);

	char label[LABEL_SIZE] = { 0 };
	snprintf(label, sizeof(label), "%ld", literal->i_value);

	FILE *out = data;
	print_dot_node(out, literal, label);
}

static void print_dot_literal_float(struct mCc_ast_literal *literal, void *data)
{
	assert(literal);
	assert(data);

	char label[LABEL_SIZE] = { 0 };
	snprintf(label, sizeof(label), "%f", literal->f_value);

	FILE *out = data;
	print_dot_node(out, literal, label);
}

static void print_dot_literal_bool(struct mCc_ast_literal *literal, void *data)
{
	assert(literal);
	assert(data);

	char label[LABEL_SIZE] = { 0 };
	snprintf(label, sizeof(label), "%i", literal->b_value);

	FILE *out = data;
	print_dot_node(out, literal, label);
}

static void print_dot_literal_string(struct mCc_ast_literal *literal, void *data)
{
        assert(literal);
        assert(data);

        char* print_string = literal->s_value;
        int i;
        for(i = 0; print_string[i] != '\0'; ++i) {
        }
        print_string[--i] = '\\';

        char label[LABEL_SIZE] = { 0 };
        snprintf(label, sizeof(label), "%c%s%c", '\\', print_string, '"');

        FILE *out = data;
        print_dot_node(out, literal, label);
}

static void print_dot_literal_identifier(struct mCc_ast_literal *literal,
                                         void *data)
{
        assert(literal);
        assert(data);

        char label[LABEL_SIZE] = { 0 };
        snprintf(label, sizeof(label), "%s", literal->id_value);

        FILE *out = data;
        print_dot_node(out, literal, label);
}

static void print_dot_statement_declaration(struct mCc_ast_statement *statement,
                                            void *data)
{
        assert(statement);
        assert(data);

        FILE *out = data;
        print_dot_node(out, statement, "statement;");
        print_dot_edge(out, statement, statement->declaration, "declaration");
}

static void print_dot_declaration(struct mCc_ast_declaration *declaration,
                                  void *data)
{
        assert(declaration);
        assert(data);

        char label[LABEL_SIZE];
        snprintf(label, sizeof(label), "declaration: %s%s",
                 mCc_ast_print_var_type(declaration->var_type),
                 mCc_ast_print_declaration_type(declaration->type));

        FILE *out = data;
        print_dot_node(out, declaration, label);
        if(declaration->type == MCC_AST_DECLARATION_TYPE_ARRAY){
                print_dot_edge(out, declaration,
                               declaration->array_decl.identifier, "identifier");
                print_dot_edge(out, declaration,
                               declaration->array_decl.size, "size");
        } else {
                print_dot_edge(out, declaration,
                               declaration->normal_decl.identifier, "identifier");
        }
}

static void print_dot_statement_assignment(struct mCc_ast_statement *statement,
                                           void *data)
{
        assert(statement);
        assert(data);

        FILE *out = data;
        print_dot_node(out, statement, "statement;");
        print_dot_edge(out, statement, statement->assignment, "assignment");
}

static void print_dot_assignment(struct mCc_ast_assignment *assignment,
                                 void *data)
{
        assert(assignment);
        assert(data);

        char label[LABEL_SIZE];
        snprintf(label, sizeof(label), "assignment: =");

        FILE *out = data;
        print_dot_node(out, assignment, label);
        print_dot_edge(out, assignment, assignment->identifier, "identifier");
        if(assignment->type == MCC_AST_ASSIGNMENT_TYPE_ARRAY){
                print_dot_edge(out, assignment, assignment->array_asmt.index, "index");
                print_dot_edge(out, assignment, assignment->array_asmt.rhs, "rhs");
        } else {
                print_dot_edge(out, assignment, assignment->normal_asmt.rhs, "rhs");
        }
}

static void print_dot_statement_expression(struct mCc_ast_statement *statement,
                                           void *data){
        assert(statement);
        assert(data);

        FILE *out = data;
        print_dot_node(out, statement, "statement;");
        print_dot_edge(out, statement, statement->expression, "expression");
}

static void print_dot_statement_compound(struct mCc_ast_statement *statement,
                                         void *data)
{
        assert(statement);
        assert(data);

        FILE *out = data;
        print_dot_node(out, statement, "{ }");
        print_dot_edge(out, statement, statement->statement, "statement");
}

static void print_dot_statement_compound_empty(struct mCc_ast_statement *statement,
                                               void *data)
{
        assert(statement);
        assert(data);

        FILE *out = data;
        print_dot_node(out, statement, "{ }");
}

static void print_dot_statement_return(struct mCc_ast_statement *statement,
                                       void *data)
{
        assert(statement);
        assert(data);

        FILE *out = data;
        print_dot_node(out, statement, "return");
        print_dot_edge(out, statement, statement->expression, "expression");
}

static void print_dot_statement_return_empty(struct mCc_ast_statement *statement,
                                             void *data)
{
        assert(statement);
        assert(data);

        FILE *out = data;
        print_dot_node(out, statement, "return;");
}

static void print_dot_statement_while(struct mCc_ast_statement *statement,
                                      void *data)
{
        assert(statement);
        assert(data);

        FILE *out = data;
        print_dot_node(out, statement, "while");
        print_dot_edge(out, statement, statement->expr, "expression");
        print_dot_edge(out, statement, statement->stmt, "statement");
}

static void print_dot_statement_if(struct mCc_ast_statement *statement,
                                   void *data)
{
        assert(statement);
        assert(data);

        FILE *out = data;
        print_dot_node(out, statement, "if");
        print_dot_edge(out, statement, statement->expr, "expression");
        print_dot_edge(out, statement, statement->stmt, "statement");
}

static void print_dot_statement_if_else(struct mCc_ast_statement *statement,
                                        void *data)
{
        assert(statement);
        assert(data);

        FILE *out = data;
        print_dot_node(out, statement, "if else");
        print_dot_edge(out, statement, statement->expr, "expression");
        print_dot_edge(out, statement, statement->if_else_stmt.stmt_1, "if->statement");
        print_dot_edge(out, statement, statement->if_else_stmt.stmt_2, "else->statement");
}

static void print_dot_parameter(struct mCc_ast_parameter *parameter,
                                void *data)
{
        assert(parameter);
        assert(data);

        FILE *out = data;
        print_dot_node(out, parameter, "parameter");
        print_dot_edge(out, parameter, parameter->declaration, "declaration");
        if(parameter->next != NULL){
                print_dot_edge(out, parameter, parameter->next, "next parameter");
        }
}

static void print_dot_expression_call(struct mCc_ast_expression *expression,
                                      void *data)
{
        assert(expression);
        assert(data);

        FILE *out = data;
        print_dot_node(out, expression, "expression: call()");
        print_dot_edge(out, expression, expression->call_expr.identifier, "identifier");
        if(expression->call_expr.arguments != NULL){
                print_dot_edge(out, expression, expression->call_expr.arguments, "arguments");
        }
}

static void print_dot_arguments(struct mCc_ast_argument_list *arguments,
                                void *data)
{
        assert(arguments);
        assert(data);

        FILE *out = data;
        print_dot_node(out, arguments, "argument");
        print_dot_edge(out, arguments, arguments->expression, "expression");
        if(arguments->next != NULL){
                print_dot_edge(out, arguments, arguments->next, "next argument");
        }
}

static void print_dot_function_def(struct mCc_ast_function_def *function_def,
                                   void *data)
{
        assert(function_def);
        assert(data);

        char label[LABEL_SIZE];
        snprintf(label, sizeof(label), "function def: %s",
                 mCc_ast_print_function_type(function_def->type));
        FILE *out = data;
        print_dot_node(out, function_def, label);
        print_dot_edge(out, function_def, function_def->identifier, "identifier");
        if(function_def->parameters != NULL){
                print_dot_edge(out, function_def, function_def->parameters, "parameters");
        }
        print_dot_edge(out, function_def, function_def->compound_stmt, "statement");
}

static void print_dot_function_def_list(struct mCc_ast_function_def_list *function_def_list,
                                        void *data)
{
        assert(function_def_list);
        assert(data);

        FILE *out = data;
        print_dot_node(out, function_def_list, "function def list");
        print_dot_edge(out, function_def_list, function_def_list->function_def, "function def");
        if(function_def_list->next != NULL) {
                print_dot_edge(out, function_def_list, function_def_list->next, "next");
        }
}

static void print_dot_program(struct mCc_ast_program *program, void *data)
{
        assert(program);
        assert(data);

        FILE *out = data;
        print_dot_node(out, program, "program");
        print_dot_edge(out, program, program->function_def_list, "functions");
}
static struct mCc_ast_visitor print_dot_visitor(FILE *out)
{
	assert(out);

	return (struct mCc_ast_visitor){
		.traversal = MCC_AST_VISIT_DEPTH_FIRST,
		.order = MCC_AST_VISIT_PRE_ORDER,

		.userdata = out,

		.statement_declaration = print_dot_statement_declaration,
                .statement_assignment = print_dot_statement_assignment,
                .statement_expression = print_dot_statement_expression,
                .statement_compound = print_dot_statement_compound,
                .statement_compound_empty = print_dot_statement_compound_empty,
                .statement_return = print_dot_statement_return,
                .statement_return_empty = print_dot_statement_return_empty,
                .statement_while = print_dot_statement_while,
                .statement_if = print_dot_statement_if,
                .statement_if_else = print_dot_statement_if_else,

                .declaration = print_dot_declaration,

                .assignment = print_dot_assignment,

                .parameter = print_dot_parameter,

                .arguments = print_dot_arguments,

                .function_def = print_dot_function_def,

                .function_def_list = print_dot_function_def_list,

                .program = print_dot_program,

		.expression_literal = print_dot_expression_literal,
		.expression_binary_op = print_dot_expression_binary_op,
		.expression_unary_op = print_dot_expression_unary_op,
		.expression_parenth = print_dot_expression_parenth,
                .expression_call = print_dot_expression_call,

		.literal_int = print_dot_literal_int,
		.literal_float = print_dot_literal_float,
                .literal_bool = print_dot_literal_bool,
                .literal_string = print_dot_literal_string,
                .literal_identifier = print_dot_literal_identifier,
	};
}

void mCc_ast_print_dot_expression(FILE *out,
                                  struct mCc_ast_expression *expression)
{
	assert(out);
	assert(expression);

	print_dot_begin(out);

	struct mCc_ast_visitor visitor = print_dot_visitor(out);
	mCc_ast_visit_expression(expression, &visitor);

	print_dot_end(out);
}

void mCc_ast_print_dot_literal(FILE *out, struct mCc_ast_literal *literal)
{
	assert(out);
	assert(literal);

	print_dot_begin(out);

	struct mCc_ast_visitor visitor = print_dot_visitor(out);
	mCc_ast_visit_literal(literal, &visitor);

	print_dot_end(out);
}

void mCc_ast_print_dot_statement(FILE *out,
				 struct mCc_ast_statement *statement)
{
	assert(out);
	assert(statement);

	print_dot_begin(out);

	struct mCc_ast_visitor visitor = print_dot_visitor(out);
	mCc_ast_visit_statement(statement, &visitor);

	print_dot_end(out);
}

void mCc_ast_print_dot_parameter(FILE *out,
                                 struct mCc_ast_parameter *parameter)
{
        assert(out);
        assert(parameter);

        print_dot_begin(out);

        struct mCc_ast_visitor visitor = print_dot_visitor(out);
        mCc_ast_visit_parameter(parameter, &visitor);

        print_dot_end(out);
}

void mCc_ast_print_dot_function_def(FILE *out,
                                    struct mCc_ast_function_def *function_def)
{
        assert(out);
        assert(function_def);

        print_dot_begin(out);

        struct mCc_ast_visitor visitor = print_dot_visitor(out);
        mCc_ast_visit_function_def(function_def, &visitor);

        print_dot_end(out);
}

void mCc_ast_print_dot_function_def_list(FILE *out,
                                         struct mCc_ast_function_def_list *function_def_list)
{
        assert(out);
        assert(function_def_list);

        print_dot_begin(out);

        struct mCc_ast_visitor visitor = print_dot_visitor(out);
        mCc_ast_visit_function_def_list(function_def_list, &visitor);

        print_dot_end(out);
}

void mCc_ast_print_dot_program(FILE *out, struct mCc_ast_program *program)
{
        assert(out);
        assert(program);

        print_dot_begin(out);

        struct mCc_ast_visitor visitor = print_dot_visitor(out);
        mCc_ast_visit_program(program, &visitor);

        print_dot_end(out);

}
