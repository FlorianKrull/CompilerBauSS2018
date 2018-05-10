#ifndef MCC_SYM_TABLE_H
#define MCC_SYM_TABLE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "mCc/ast.h"

/* Declare the symbol table as a list of hashtables
 * More information: https://www.d.umn.edu/~rmaclin/cs5641/Notes/L15_SymbolTable.pdf
*/

enum mCc_st_entry_type {
	MCC_ST_ENTRY_TYPE_VARIABLE,
	MCC_ST_ENTRY_TYPE_FUNCTION
};
/* The basic element; storing declaration of a variables
 * It's organized as linked list; in which next pointer hold the address of the
 * next node. */
struct mCc_st_entry {
	enum mCc_ast_type data_type;
	enum mCc_st_entry_type entry_type;
	char *name;
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

/* ---------------------------------------------------------------- Initialization */

struct mCc_st_entry *mCc_st_new_entry(const char* name,
									enum mCc_ast_type data_type,
									enum mCc_st_entry_type entry_type);

struct mCc_st_table *mCc_st_new_table();

void mCc_st_update_scope (struct mCc_st_table* table, int scope);

void mCc_st_delete_entry(struct mCc_st_entry* entry);

void mCc_st_delete_table(struct mCc_st_table* table);

/* ---------------------------------------------------------------- Hash function */
int mCc_st_hash(const char* str);

// Basic function: look up, insert and delete
/* ---------------------------------------------------------------- Insert element */

void mCc_st_insert_entry(struct mCc_st_table *table, struct mCc_st_entry *entry);

/* ---------------------------------------------------------------- Delete element */

void mCc_st_remove_entry(struct mCc_st_table *table, struct mCc_st_entry *entry);

//bool mCc_st_lookup(struct mCc_ast_assignment *assign, struct mCc_st_table* table);

bool mCc_st_lookup(const char *var_name, struct mCc_st_table *table);

#ifdef __cplusplus
}
#endif

#endif
