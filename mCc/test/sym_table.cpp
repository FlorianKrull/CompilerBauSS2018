#include <gtest/gtest.h>
#include <stdbool.h>

#include "mCc/sym_table.h"
#include "mCc/parser.h"

/* ---------------------------------------------------------------- Initialization */

TEST(sym_table, NewEntry_1)
{
	struct mCc_st_entry *entry = mCc_st_new_entry("my_var", MCC_AST_TYPE_BOOL, MCC_ST_ENTRY_TYPE_VARIABLE);
	ASSERT_STREQ("my_var", entry->name);

	ASSERT_EQ(MCC_AST_TYPE_BOOL, entry->data_type);
	ASSERT_EQ(MCC_ST_ENTRY_TYPE_VARIABLE, entry->entry_type);

	ASSERT_EQ(NULL, entry->next);

	mCc_st_delete_entry(entry);
}

TEST(sym_table, NewTable_1)
{
	struct mCc_st_table *table = mCc_st_new_empty_table();
	ASSERT_EQ(0, table->size);
	ASSERT_EQ(NULL, table->head);
	ASSERT_EQ(NULL, table->prev);
	ASSERT_EQ(NULL, table->next);

	mCc_st_delete_table(table);
}

/* ---------------------------------------------------------------- Insert */

TEST(sym_table, InsertEntry_1)
{
	struct mCc_st_entry *entry1 = mCc_st_new_entry("_1st", MCC_AST_TYPE_BOOL, MCC_ST_ENTRY_TYPE_FUNCTION);
	struct mCc_st_entry *entry2 = mCc_st_new_entry("_2nd", MCC_AST_TYPE_INT, MCC_ST_ENTRY_TYPE_VARIABLE);

	struct mCc_st_table *table = mCc_st_new_empty_table();
	mCc_st_insert_entry(table, entry1);
	mCc_st_insert_entry(table, entry2);

	ASSERT_EQ(2, table->size);

	auto head = table->head;
	ASSERT_STREQ("_1st", head->name);
	ASSERT_EQ(MCC_AST_TYPE_BOOL, head->data_type);
	ASSERT_EQ(MCC_ST_ENTRY_TYPE_FUNCTION, head->entry_type);

	auto _2nd_item = head->next;
	ASSERT_STREQ("_2nd", _2nd_item->name);
	ASSERT_EQ(MCC_AST_TYPE_INT, _2nd_item->data_type);
	ASSERT_EQ(MCC_ST_ENTRY_TYPE_VARIABLE, _2nd_item->entry_type);

	mCc_st_delete_table(table);
}

TEST(sym_table, Insert_1)
{
	const char input[] = "void main(bool id){} float sub(){} float add(int x, float y) {float z; z = x + y; return z;}";

	auto parse_result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, parse_result.status);

	struct mCc_st_table *table = mCc_st_new_table(parse_result);

	// Print
	mCc_st_print_table_list(table);

	mCc_st_delete_table(table);
	mCc_parser_delete_result(&parse_result);
}

/* ---------------------------------------------------------------- Delete */
TEST(sym_table, RemoveEntry_1)
{
	struct mCc_st_table *table = mCc_st_new_empty_table();
	struct mCc_st_entry *new_entry = mCc_st_new_entry("c2", MCC_AST_TYPE_BOOL, MCC_ST_ENTRY_TYPE_VARIABLE);

	mCc_st_insert_entry(table, new_entry);
	mCc_st_remove_entry(table, new_entry);

	ASSERT_EQ(0, table->size);
	ASSERT_EQ(NULL, table->head);

	mCc_st_delete_table(table);
}

// Remove head
TEST(sym_table, RemoveEntry_2)
{
	struct mCc_st_table *table = mCc_st_new_empty_table();
	struct mCc_st_entry *new_entry1 = mCc_st_new_entry("a1",
									MCC_AST_TYPE_STRING, MCC_ST_ENTRY_TYPE_VARIABLE);
	struct mCc_st_entry *new_entry2 = mCc_st_new_entry("b2",
									MCC_AST_TYPE_VOID, MCC_ST_ENTRY_TYPE_FUNCTION);

	mCc_st_insert_entry(table, new_entry1);
	mCc_st_insert_entry(table, new_entry2);

	mCc_st_remove_entry(table, new_entry1);

	auto entry2 = table->head;
	ASSERT_STREQ("b2", entry2->name);
	ASSERT_EQ(MCC_AST_TYPE_VOID, entry2->data_type);
	ASSERT_EQ(MCC_ST_ENTRY_TYPE_FUNCTION, entry2->entry_type);

	ASSERT_EQ(NULL, entry2->next);

	mCc_st_delete_table(table);
}

//Remove last element
TEST(sym_table, RemoveEntry_3)
{
	struct mCc_st_table *table = mCc_st_new_empty_table();
	struct mCc_st_entry *new_entry1 = mCc_st_new_entry("a1",
										MCC_AST_TYPE_STRING, MCC_ST_ENTRY_TYPE_VARIABLE);
		struct mCc_st_entry *new_entry2 = mCc_st_new_entry("b2",
										MCC_AST_TYPE_VOID, MCC_ST_ENTRY_TYPE_FUNCTION);

	mCc_st_insert_entry(table, new_entry1);
	mCc_st_insert_entry(table, new_entry2);

	mCc_st_remove_entry(table, new_entry2);

	auto entry1 = table->head;
	ASSERT_STREQ("a1", entry1->name);
	ASSERT_EQ(MCC_AST_TYPE_STRING, entry1->data_type);
	ASSERT_EQ(MCC_ST_ENTRY_TYPE_VARIABLE, entry1->entry_type);

	ASSERT_EQ(NULL, entry1->next);

	mCc_st_delete_table(table);
}

/* ---------------------------------------------------------------- Lookup */

TEST(sym_table, Table_Lookup_1)
{
	struct mCc_st_table *table = mCc_st_new_empty_table();
	struct mCc_st_entry *entry1 = mCc_st_new_entry("y", MCC_AST_TYPE_FLOAT, MCC_ST_ENTRY_TYPE_VARIABLE);
	struct mCc_st_entry *entry2 = mCc_st_new_entry("x", MCC_AST_TYPE_INT, MCC_ST_ENTRY_TYPE_VARIABLE);

	mCc_st_insert_entry(table, entry1);
	mCc_st_insert_entry(table, entry2);

	const char input[] = "x = 1;";
	auto parse_result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, parse_result.status);

	auto asmt = parse_result.statement->assignment;

	// asmt->identifier;
	auto id = asmt->identifier->id_value;
	ASSERT_STREQ("x", id);
	bool result = mCc_st_lookup(id, table);

	ASSERT_EQ(true, result);

	mCc_st_delete_table(table);
}

TEST(sym_table, Table_Lookup_2)
{
	struct mCc_st_table *table = mCc_st_new_empty_table();
	struct mCc_st_entry *entry1 = mCc_st_new_entry("y", MCC_AST_TYPE_FLOAT, MCC_ST_ENTRY_TYPE_VARIABLE);
	struct mCc_st_entry *entry2 = mCc_st_new_entry("x", MCC_AST_TYPE_INT, MCC_ST_ENTRY_TYPE_VARIABLE);
	struct mCc_st_entry *entry3 = mCc_st_new_entry("main", MCC_AST_TYPE_FLOAT, MCC_ST_ENTRY_TYPE_FUNCTION);

	mCc_st_insert_entry(table, entry1);
	mCc_st_insert_entry(table, entry2);
	mCc_st_insert_entry(table, entry3);

	const char input[] = "z = 1;";
	auto parse_result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, parse_result.status);

	auto asmt = parse_result.statement->assignment;

	// asmt->identifier;
	auto id = asmt->identifier->id_value;
	ASSERT_STREQ("z", id);
	bool result = mCc_st_lookup(id, table);

	ASSERT_EQ(false, result);

	mCc_st_delete_table(table);
}
/*\
TEST(sym_table, Table_Lookup_3)
{
	const char input[] = "void add(int x, float y) {x = 1;}";
	auto parse_result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, parse_result.status);

	struct mCc_st_table *table = mCc_st_new_empty_table();
	int scope = 1;
	mCc_st_update_scope(table, scope);

	// Add declaration to symbol table
	auto program = parse_result.program;

	//program->function_def_list->function_def
	auto func = program->function_def_list->function_def;

	//func->function type
	ASSERT_EQ(MCC_AST_TYPE_VOID, func->type);

	//func->identifier
	auto id = func->identifier;
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_IDENTIFIER, id->type);
	ASSERT_STREQ("add", id->id_value);

	// Add function declaration to table
	struct mCc_st_entry *func_entry = mCc_st_new_entry(id->id_value, func->type, MCC_ST_ENTRY_TYPE_FUNCTION);
	mCc_st_insert_entry(table, func_entry);

	auto param = func->parameters;
	//func->param->x declaration
	auto x_decl = param->declaration;
	ASSERT_EQ(MCC_AST_DECLARATION_TYPE_NORMAL, x_decl->type);

	//x->var_type
	ASSERT_EQ(MCC_AST_TYPE_INT, x_decl->var_type);

	//x->identifier
	auto x_subid = x_decl->normal_decl.identifier;
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_IDENTIFIER, x_subid->type);
	ASSERT_STREQ("x", x_subid->id_value);

	// Add variable declaration to table
	struct mCc_st_entry *var_x = mCc_st_new_entry(x_subid->id_value, x_decl->var_type, MCC_ST_ENTRY_TYPE_VARIABLE);
	mCc_st_insert_entry(table, var_x);

	//x->next = y declaration
	auto y_decl = param->next->declaration;
	ASSERT_EQ(MCC_AST_DECLARATION_TYPE_NORMAL, y_decl->type);

	//y->var_type
	ASSERT_EQ(MCC_AST_TYPE_FLOAT, y_decl->var_type);

	//y->identifier
	auto y_subid = y_decl->normal_decl.identifier;
	ASSERT_EQ(MCC_AST_LITERAL_TYPE_IDENTIFIER, y_subid->type);
	ASSERT_STREQ("y", y_subid->id_value);

	// Add variable declaration to table
	struct mCc_st_entry *var_y = mCc_st_new_entry(y_subid->id_value, y_decl->var_type, MCC_ST_ENTRY_TYPE_VARIABLE);
	mCc_st_insert_entry(table, var_y);

	// Start looking up
	//func->compound_stmt
	auto stmt = func->compound_stmt;
	ASSERT_EQ(MCC_AST_STATEMENT_TYPE_COMPOUND, stmt->type);
	ASSERT_EQ(MCC_AST_STATEMENT_TYPE_ASSIGNMENT, stmt->statement_list->statement->type);

	//compound_stmt->statement->assignment
	auto asmt = stmt->statement->assignment;
	ASSERT_EQ(MCC_AST_ASSIGNMENT_TYPE_NORMAL, asmt->type);

	// asmt->identifier;
	auto asmt_id = asmt->identifier;

	ASSERT_EQ(MCC_AST_LITERAL_TYPE_IDENTIFIER, asmt_id->type);
	ASSERT_STREQ("x", asmt_id->id_value);

	// asmt->normal_asmt->rhs
	auto asmt_expr = asmt->normal_asmt.rhs;
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, asmt_expr->type);
	auto x_value = asmt_expr->literal->i_value;
	ASSERT_EQ(1, x_value);

	auto result = mCc_st_lookup(asmt_id->id_value, table);
	ASSERT_EQ(true, result);

	mCc_st_delete_table(table);
	mCc_ast_delete_program(program);
}
*/
/* ---------------------------------------------------------------- Type checking */
TEST(sym_table, Type_Checking_1)
{
	const char input[] = "x = 1.5;";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto expr = result.statement->assignment->normal_asmt.rhs;

	ASSERT_EQ(MCC_AST_LITERAL_TYPE_FLOAT, mCc_st_check_type_expression(expr));

	mCc_parser_delete_result(&result);

}
/*
TEST(sym_table, Type_Checking_2)
{
	const char input[] = "!(true)";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto expr = result.expression;

	ASSERT_EQ(MCC_AST_LITERAL_TYPE_BOOL, mCc_st_check_type_expression(expr));

	mCc_parser_delete_result(&result);

}
*/
TEST(sym_table, Type_Checking_3)
{
	const char input[] = "-(4 + 6)";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto expr = result.expression;

	ASSERT_EQ(MCC_AST_LITERAL_TYPE_BOOL, mCc_st_check_type_expression(expr));

	mCc_parser_delete_result(&result);

}

TEST(sym_table, Type_Checking_4)
{
	const char input[] = "(a && b) || (x || y)";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto expr = result.expression;

	ASSERT_EQ(MCC_AST_LITERAL_TYPE_BOOL, mCc_st_check_type_expression(expr));

	mCc_parser_delete_result(&result);

}

TEST(sym_table, Type_Checking_5)
{
	const char input[] = "void add(int x, float y) {x = 1;}";
	auto parse_result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, parse_result.status);

	auto program = parse_result.program;

	// Insert function to table
	struct mCc_st_table *table = mCc_st_new_table(parse_result);

	auto func = parse_result.program->function_def_list->function_def;
	auto param = func->parameters;
	//x declaration
	auto x_decl = param->declaration;

	//y->var_type
	ASSERT_EQ(MCC_AST_TYPE_INT, x_decl->var_type);
	auto x_subid = param->declaration->normal_decl.identifier;
	ASSERT_STREQ("x", x_subid->id_value);

	// Insert variable declaration to table
	struct mCc_st_entry *var_x = mCc_st_new_entry(x_subid->id_value, x_decl->var_type, MCC_ST_ENTRY_TYPE_VARIABLE);
	mCc_st_insert_entry(table, var_x);

	//x->next = y declaration
	auto y_decl = param->next->declaration;

	//y->var_type
	ASSERT_EQ(MCC_AST_TYPE_FLOAT, y_decl->var_type);

	//y->identifier
	auto y_subid = y_decl->normal_decl.identifier;
	ASSERT_STREQ("y", y_subid->id_value);

	// Add variable declaration to table
	struct mCc_st_entry *var_y = mCc_st_new_entry(y_subid->id_value, y_decl->var_type, MCC_ST_ENTRY_TYPE_VARIABLE);
	mCc_st_insert_entry(table, var_y);

	// Start check type and value
	//func->compound_stmt
	auto stmt = func->compound_stmt;

	//compound_stmt->statement->assignment
	auto asmt = stmt->statement_list->statement->assignment;

	// asmt->identifier;
	auto asmt_id = asmt->identifier;

	ASSERT_STREQ("x", asmt_id->id_value);

	// asmt->normal_asmt->rhs
	auto literal = asmt->normal_asmt.rhs->literal;

	auto result = mCc_st_check_type_value(x_decl->var_type, literal);
	ASSERT_EQ(true, result);


	mCc_st_delete_table(table);
	mCc_ast_delete_program(program);
}
