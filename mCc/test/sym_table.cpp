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
	const char input[] = "void main(bool id){} float sub(){string s;} float add(int x, float y) {float z; z = x + y; return z;}";

	auto parse_result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, parse_result.status);

	struct mCc_st_table *table = mCc_st_new_table(parse_result);

	// Print
	mCc_st_print_table_list(table);

	mCc_st_delete_table(table);
	mCc_parser_delete_result(&parse_result);
}

// input is part of the example leap_year.mC
TEST(sym_table, Insert_2)
{
	const char input[] = "bool isLeapYear(int n) {  if ((modulo(n,4) == 0 && modulo(n,100) != 0) || (modulo(n,400) == 0)){ return true;  } return false; } int modulo(int k, int i) { while (k > 0){ k = k - i;  } return k;}";

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

	const char input[] = "void f() {x = 1;}";
	auto parse_result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, parse_result.status);

	auto asmt = parse_result.program->function_def_list->function_def->compound_stmt->
			statement_list->statement->assignment;

	// Look up for variable x
	auto id = asmt->identifier->id_value;
	ASSERT_STREQ("x", id);
	auto check_manager = mCc_st_lookup(id, 1, table);
	ASSERT_EQ(false, check_manager->is_error);

	mCc_st_delete_checking(check_manager);
	mCc_st_delete_table(table);
	mCc_parser_delete_result(&parse_result);
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

	const char input[] = "void f() {z = 1;}";
	auto parse_result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, parse_result.status);

	auto asmt = parse_result.program->function_def_list->function_def->compound_stmt->
				statement_list->statement->assignment;

	// Look up for variable z
	auto id = asmt->identifier->id_value;
	ASSERT_STREQ("z", id);
	struct mCc_st_checking *check_manager = mCc_st_lookup(id, 1, table);
	ASSERT_EQ(true, check_manager->is_error);
	ASSERT_STREQ("Undeclared variable", check_manager->msg);

//	mCc_st_delete_checking(check_manager);
	mCc_st_delete_table(table);
	mCc_parser_delete_result(&parse_result);
}

// Look up in nested table
TEST(sym_table, Table_Lookup_3)
{
	const char input[] = "void add(int x, float y) {x = 1;}";
	auto parse_result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, parse_result.status);

	struct mCc_st_table *table = mCc_st_new_table(parse_result);
	int scope = 1;

	// Start looking up
	//compound_stmt->statement->assignment
	auto asmt = parse_result.program->function_def_list->function_def->compound_stmt->statement_list->statement->assignment;
	scope++;

	// asmt->identifier;
	auto asmt_id = asmt->identifier;

	ASSERT_EQ(MCC_AST_LITERAL_TYPE_IDENTIFIER, asmt_id->type);
	ASSERT_STREQ("x", asmt_id->id_value);

	// asmt->normal_asmt->rhs
	auto asmt_expr = asmt->normal_asmt.rhs;
	ASSERT_EQ(MCC_AST_EXPRESSION_TYPE_LITERAL, asmt_expr->type);
	auto x_value = asmt_expr->literal->i_value;
	ASSERT_EQ(1, x_value);

	struct mCc_st_checking *check_manager = mCc_st_lookup(asmt_id->id_value, scope, table);
	ASSERT_EQ(false, check_manager->is_error);

//	mCc_st_delete_checking(check_manager);
	mCc_st_delete_table(table);
	mCc_parser_delete_result(&parse_result);
}

/* ---------------------------------------------------------------- Type checking */
TEST(sym_table, Type_Checking_1)
{
	const char input[] = "void f() {x = 1.5;}";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto expr = result.program->function_def_list->function_def->compound_stmt->statement_list->statement->assignment->normal_asmt.rhs;

	ASSERT_EQ(MCC_AST_LITERAL_TYPE_FLOAT, mCc_st_return_type_expression(expr));

	mCc_parser_delete_result(&result);

}
/*
TEST(sym_table, Type_Checking_2)
{
	const char input[] = "!(true)";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto expr = result.expression;

	ASSERT_EQ(MCC_AST_LITERAL_TYPE_BOOL, mCc_st_return_type_expression(expr));

	mCc_parser_delete_result(&result);

}
*/
TEST(sym_table, Type_Checking_3)
{
	const char input[] = "-(4 + 6)";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto expr = result.expression;

	ASSERT_EQ(MCC_AST_LITERAL_TYPE_BOOL, mCc_st_return_type_expression(expr));

	mCc_parser_delete_result(&result);

}

TEST(sym_table, Type_Checking_4)
{
	const char input[] = "(a && b) || (x || y)";
	auto result = mCc_parser_parse_string(input);

	ASSERT_EQ(MCC_PARSER_STATUS_OK, result.status);

	auto expr = result.expression;

	ASSERT_EQ(MCC_AST_LITERAL_TYPE_BOOL, mCc_st_return_type_expression(expr));

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

	// Check if "x = 1" is compatible with "int x"
	auto asmt = parse_result.program->function_def_list->function_def->compound_stmt->
			statement_list->statement->assignment;
	auto id = asmt->identifier->id_value;
	auto expr = asmt->normal_asmt.rhs;
	auto *check_manager = mCc_st_lookup(id, 2, table);

	bool tc_result = mCc_st_type_rules(check_manager->entry->data_type, mCc_st_return_type_expression(expr));
	ASSERT_EQ(true, tc_result);

	// Print
	mCc_st_print_table_list(table);

//	mCc_st_delete_checking(check_manager);
	mCc_st_delete_table(table);
	mCc_parser_delete_result(&parse_result);
}
