#include "mCc/sym_table.h"

#include <assert.h>
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
struct mCc_st_table *mCc_st_new_table()
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
	if (NULL != table->head) {
		//Link old first node to entry
		table->head->next = entry;

	} else {
		//point head to new first node
		table->head = entry;
	}
	++(table->size);
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

