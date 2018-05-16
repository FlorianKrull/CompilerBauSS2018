#ifndef MCC_SYM_TABLE_H
#define MCC_SYM_TABLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "mCc/ast.h"
#include "mCc/parser.h"

/* Declare the symbol table as a list of hashtables
 * More information: https://www.d.umn.edu/~rmaclin/cs5641/Notes/L15_SymbolTable.pdf
*/

enum mCc_st_entry_type {
	MCC_ST_ENTRY_TYPE_VARIABLE,
	MCC_ST_ENTRY_TYPE_FUNCTION,
	MCC_ST_ENTRY_TYPE_ARGUMENT
};
/* The basic element; storing declaration of a variables
 * It's organized as linked list; in which next pointer hold the address of the
 * next node. */
struct mCc_st_entry {
	enum mCc_ast_type data_type;
	enum mCc_st_entry_type entry_type;
	char *name;
	int array_size; 		// in case entry is variable and an array, i.e: int[10] x -> array_size = 10
	char *function_name;	// In case entry is parameter of a function
	struct mCc_st_entry *next;
};

/* Table, in which a head pointer pointing to current entry
 * Also organized as linked list to another table
 */
struct mCc_st_table {
	struct mCc_st_entry *head;
	int scope;	// Necessary for nested scope
	int size;
	struct mCc_st_table *prev;
	struct mCc_st_table *next;
};
/*
struct mCc_st_error {
	bool is_error;
	const char *msg;
};
*/
// Used for Task 1 -> Task 4
struct mCc_st_checking {
	bool is_error;
	char *msg;		// Error message
	struct mCc_st_entry *entry;
	union {
		// for Task 4
		struct mCc_ast_assignment *assignment;
		struct mCc_ast_expression *call_expr;
	};
};

/* ---------------------------------------------------------------- Initialization */

struct mCc_st_entry *mCc_st_new_entry(const char* name,
									enum mCc_ast_type data_type,
									enum mCc_st_entry_type entry_type);

struct mCc_st_table *mCc_st_new_empty_table();

void mCc_st_update_scope(struct mCc_st_table* table, int scope);

void mCc_st_delete_entry(struct mCc_st_entry* entry);

void mCc_st_delete_table(struct mCc_st_table* table);

void mCc_st_delete_checking(struct mCc_st_checking *tc);

/* ---------------------------------------------------------------- Hash function */
int mCc_st_hash(const char* str);

// Basic function: look up, insert and delete
/* ---------------------------------------------------------------- Insert element */

void mCc_st_insert_entry(struct mCc_st_table *table, struct mCc_st_entry *entry);

void mCc_st_insert_variable(struct mCc_st_table *table, struct mCc_ast_declaration *func);

struct mCc_st_table *mCc_st_new_child_table(struct mCc_st_table *parent);

void mCc_st_insert_parameter(struct mCc_st_table *table,
		const char *func_name, struct mCc_ast_parameter *param);

void mCc_st_insert_statement(struct mCc_st_table *table, struct mCc_ast_statement *stmt);

void mCc_st_insert_function(struct mCc_st_table *table, struct mCc_ast_function_def *func);

struct mCc_st_table *mCc_st_new_table(struct mCc_parser_result result);

/* ---------------------------------------------------------------- Delete element */

void mCc_st_remove_entry(struct mCc_st_table *table, struct mCc_st_entry *entry);

/* ---------------------------------------------------------------- Print */
void mCc_st_print_entry(struct mCc_st_entry *en);

void mCc_st_print_table(struct mCc_st_table *table);

void mCc_st_print_table_list(struct mCc_st_table *tab_tail);

/* ---------------------------------------------------------------- Look up */
struct mCc_st_checking *mCc_st_lookup(const char *var_name, int scope, struct mCc_st_table *table);

/* ---------------------------------------------------------------- Type checking */

bool mCc_st_type_rules(enum mCc_ast_type type, enum mCc_ast_literal_type lit_type);

enum mCc_ast_literal_type
mCc_st_return_type_expression(struct mCc_ast_expression *expr);

struct mCc_st_checking *mCc_st_var_type_checking(struct mCc_st_table *table, int scope, struct mCc_ast_assignment *assignment);

struct mCc_st_checking *mCc_st_argument_type_checking(struct mCc_st_table *table, int scope, struct mCc_ast_expression *expression);

#ifdef __cplusplus
}
#endif

#endif