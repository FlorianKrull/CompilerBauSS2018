#include "mCc/sym_table.h"
#include "mCc/parser.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define SIZE 50	//Size of each table
#define HASH_VALUE 131	//A hash value should a minimum prime number larger than ASCII's alphabet size

/* ---------------------------------------------------------------- Entries */
int mCc_st_hash(const char* str) {
	//	long hash = 0;
	unsigned long hash = 5381;
	const int len_s = strlen(str);
	for (int i = 0; i < len_s; i++) {
		//		hash += (long) pow(HASH_VALUE, len_s - (i+1)) * str[i];
		//		hash = hash % SIZE;
		hash = ((hash << 5) + hash) + str[i];
	}
	return (int) hash;
}

struct mCc_st_entry *mCc_st_new_entry(const char* name,
		enum mCc_ast_type data_type,
		enum mCc_st_entry_type entry_type)
{
	struct mCc_st_entry *en = malloc(sizeof(*en));
	if (!en) {
		return NULL;
	}

	en->name = strdup(name);
	en->data_type = data_type;
	en->entry_type = entry_type;
	en->next = NULL;
	return en;
}


void mCc_st_delete_entry(struct mCc_st_entry* entry)
{
	assert(entry);
	free(entry->name);
	free(entry);
}

/* ---------------------------------------------------------------- Tables */
// Create an empty table
struct mCc_st_table *mCc_st_new_empty_table()
{
	struct mCc_st_table *tab = malloc(sizeof(*tab));
	if (!tab) {
		return NULL;
	}

	tab->head = NULL;
	tab->prev = NULL;
	tab->next = NULL;
	tab->size = 0;
	return tab;
}

void mCc_st_update_scope(struct mCc_st_table* table, int scope)
{
	assert(table);
	table->scope = scope;
}

void mCc_st_delete_table(struct mCc_st_table* table)
{
	assert(table);
	if (NULL != table->head) {
		mCc_st_delete_entry(table->head);
	}
	free(table);
}
/* ---------------------------------------------------------------- Insert */

//Insert entry at the last location of table
void mCc_st_insert_entry(struct mCc_st_table *table, struct mCc_st_entry *entry)
{
	struct mCc_st_entry *last = table->head;
	if (NULL != last) {
		// Traverse until the end
		while (last->next != NULL) {
			last = last->next;
		}
		last->next = entry;

	} else {
		//point head to new first node
		table->head = entry;
	}
	entry->next = NULL;
	++(table->size);
}

// Get the AST tree from parser and construct symbol table
struct mCc_st_table *mCc_st_new_table(const char *input)
{
//	assert(result);

	struct mCc_st_table *table = mCc_st_new_empty_table();

	if (!table) {
		return NULL;
	}
	int scope = 1;
	mCc_st_update_scope(table, scope);

	auto result = mCc_parser_parse_string(input);
	auto list = result.program->function_def_list;
	auto func = list->function_def;

	// Insert first fuction
	struct mCc_st_entry *entry = mCc_st_new_entry(func->identifier->id_value,
						func->type, MCC_ST_ENTRY_TYPE_FUNCTION);

	mCc_st_insert_entry(table, entry);

	// Insert the remaining functions
	auto next_list = list->next;
	while (next_list != NULL) {
		auto next_func = next_list->function_def;
		struct mCc_st_entry *entry = mCc_st_new_entry(next_func->identifier->id_value,
				next_func->type, MCC_ST_ENTRY_TYPE_FUNCTION);

		mCc_st_insert_entry(table, entry);

		next_list = next_list->next;
	}

	mCc_parser_delete_result(&result);
	return table;
}

/* ---------------------------------------------------------------- Delete */
/*
void mCc_st_remove_item(struct mCc_st_entry *entry, struct mCc_st_item *item)
{
	struct mCc_st_item *current = entry->head;
	struct mCc_st_item *prev = NULL;	// Hold the current value while iterating

	while (current->next != NULL && current != item) {	//Iterate while item is not found
		prev = current;
		current = current->next;
	}

	if (current == item) {	// Item found
		if (current == entry->head) {
			entry->head = entry->head->next;
		} else {
			prev->next = current->next;
		}
		mCc_st_delete_item(current);
	}
}
 */
void mCc_st_remove_entry(struct mCc_st_table *table, struct mCc_st_entry *entry)
{
	struct mCc_st_entry *current = table->head;
	struct mCc_st_entry *prev = NULL; // Hold the current value while iterating

	while (current->next != NULL && current != entry) { //Iterate while item is not found
		prev = current;
		current = current->next;
	}

	if (current == entry) {	// Entry found
		if (current == table->head) {
			table->head = table->head->next;
		} else {
			prev->next = current->next;
		}
		mCc_st_delete_entry(current);
		--(table->size);
	}

}

/* ---------------------------------------------------------------- Print */
void mCc_st_print_entry(struct mCc_st_entry *en) {
	assert(en);
	printf("(Entry: %s, Type: %d)", en->name, en->data_type);

}

void mCc_st_print_table(struct mCc_st_table *table) {
	printf("Table in scope %i : %p\n", table->scope, table);
	assert(table);
	struct mCc_st_entry *ptr = table->head;
	//start from the beginning
	while(ptr != NULL) {
		mCc_st_print_entry(ptr);
		printf(" -> ");
		ptr = ptr->next;
	}
	printf("Size = %i\n", table->size);
}

void mCc_st_print_table_list(struct mCc_st_table *tab_tail) {
	assert(tab_tail);
	mCc_st_print_table(tab_tail);
	if (NULL != tab_tail->prev) {
		mCc_st_print_table_list(tab_tail->prev);
	}
}

/* ---------------------------------------------------------------- Look up */
// Check for undeclared variables
bool mCc_st_lookup(const char *var_name, struct mCc_st_table *table)
{
	assert(table);

	bool result = false;

	struct mCc_st_entry *current = table->head;

	// Start looking up the table, compare variable to name of the current entry
	while (current != NULL) {
		if (strcmp(current->name, var_name) == 0) {
			result = true;
		}
		current = current->next;
	}

	return result;
}

/* ---------------------------------------------------------------- Type checking */

// Check if literal value and type are compatible, i.e: int x = 1; float y = 3.5;
bool mCc_st_check_type_value(enum mCc_ast_type type, struct mCc_ast_literal *literal)
{
	assert(literal);
	bool result = false;

	enum mCc_ast_literal_type lit_type = literal->type;
	switch(type) {
	case MCC_AST_TYPE_INT:	// int x = 1 -> valid
		if (lit_type == MCC_AST_LITERAL_TYPE_INT) {
			result = true;
		}

	case MCC_AST_TYPE_FLOAT: // float x = 1.5 or float x = 1
		if ((lit_type == MCC_AST_LITERAL_TYPE_FLOAT) || (lit_type == MCC_AST_LITERAL_TYPE_INT)) {
			result = true;
		}

	case MCC_AST_TYPE_STRING: // string x = "abc";
		if (lit_type != MCC_AST_LITERAL_TYPE_STRING) {
			result = true;
		}
	case MCC_AST_TYPE_BOOL:
		if (lit_type != MCC_AST_LITERAL_TYPE_BOOL) {
			result = true;
		}
	}
	return result;
}

// Check literal type of expression (int, float, bool) using bottom-up parsing
// Not check invalid type, i.e: "a" + "b"
enum mCc_ast_literal_type
mCc_st_check_type_expression(struct mCc_ast_expression *expr)
{
	assert(expr);

	switch(expr->type) {
	case MCC_AST_EXPRESSION_TYPE_LITERAL:
		return (expr->literal->type);

	case MCC_AST_EXPRESSION_TYPE_UNARY_OP:
		// -(4 + 6); !(a == 1); -> return type of expression inside
		return mCc_st_check_type_expression(expr->u_rhs);
//		return MCC_AST_LITERAL_TYPE_BOOL;
		break;

	case MCC_AST_EXPRESSION_TYPE_BINARY_OP:
		switch(expr->binary_op_type) {
		case MCC_AST_BINARY_OP_TYPE_BINARY:
			// a && b; a || b -> return bool;
			return MCC_AST_LITERAL_TYPE_BOOL;
//			break;
/*
		case MCC_AST_BINARY_OP_TYPE_ADD:
			// 6 - 1; 7 + 2.5;
			enum mCc_ast_literal_type lhs_type =
					mCc_st_check_type_expression(expr->lhs);
			enum mCc_ast_literal_type rhs_type =
					mCc_st_check_type_expression(expr->rhs);

			if ((lhs_type == MCC_AST_LITERAL_TYPE_INT) && (rhs_type == MCC_AST_LITERAL_TYPE_INT)) {
				return MCC_AST_LITERAL_TYPE_INT;
			} else {
				if ((lhs_type == MCC_AST_LITERAL_TYPE_INT) && (rhs_type == MCC_AST_LITERAL_TYPE_FLOAT)) {
					return MCC_AST_LITERAL_TYPE_FLOAT;
				}
				if ((lhs_type == MCC_AST_LITERAL_TYPE_FLOAT) && (rhs_type == MCC_AST_LITERAL_TYPE_INT)) {
					return MCC_AST_LITERAL_TYPE_FLOAT;
				}
			}
			break;

		case MCC_AST_BINARY_OP_TYPE_MUL:
			enum mCc_ast_literal_type lhs_type =
					mCc_st_check_type_expression(expr->lhs);
			enum mCc_ast_literal_type rhs_type =
					mCc_st_check_type_expression(expr->rhs);

			switch(expr->mul_op) {
			case MCC_AST_BINARY_OP_MUL:	// same with ADD operator
				if ((lhs_type == MCC_AST_LITERAL_TYPE_INT) && (rhs_type == MCC_AST_LITERAL_TYPE_INT)) {
					return MCC_AST_LITERAL_TYPE_INT;
				} else {
					if ((lhs_type == MCC_AST_LITERAL_TYPE_INT) && (rhs_type == MCC_AST_LITERAL_TYPE_FLOAT)) {
						return MCC_AST_LITERAL_TYPE_FLOAT;
					}
					if ((lhs_type == MCC_AST_LITERAL_TYPE_FLOAT) && (rhs_type == MCC_AST_LITERAL_TYPE_INT)) {
						return MCC_AST_LITERAL_TYPE_FLOAT;
					}
				}
				break;

			case MCC_AST_BINARY_OP_DIV:	// 2/5; 7.2/6; -> return FLOAT
				if ((lhs_type == MCC_AST_LITERAL_TYPE_INT) || (lhs_type == MCC_AST_LITERAL_TYPE_FLOAT)) {
					if ((rhs_type == MCC_AST_LITERAL_TYPE_INT) || (rhs_type == MCC_AST_LITERAL_TYPE_FLOAT)) {
						return MCC_AST_LITERAL_TYPE_FLOAT;
					}
				}
			}
			break;
*/
		case MCC_AST_BINARY_OP_TYPE_COMPARE:
			return MCC_AST_LITERAL_TYPE_BOOL;
			break;
		}
		break;

		case MCC_AST_EXPRESSION_TYPE_PARENTH:
			return mCc_st_check_type_expression(expr->expression);
		default:
			break;
	}
}
