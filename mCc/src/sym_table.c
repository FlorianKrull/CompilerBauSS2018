#include "mCc/sym_table.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SIZE 50	//Size of each table
#define HASH_VALUE 131	//A hash value should a minimum prime number larger than ASCII's alphabet size

/* ---------------------------------------------------------------- Items */
struct mCc_st_item *mCc_st_new_item(const char* type, int scope)
{
	//	assert(next);
	struct mCc_st_item *item = malloc(sizeof(*item));
	if (!item) {
		return NULL;
	}
	item->type = strdup(type);
	item->scope = scope;
	item->next = NULL;
	return item;
}

void mCc_st_delete_item(struct mCc_st_item* item)
{
	assert(item);
	free(item->type);
//	free(item->scope);
	if (NULL != item->next) {
		mCc_st_delete_item(item->next);
	}
	free(item);
}

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

struct mCc_st_entry *mCc_st_new_entry(const char* name, const char* type, int scope)
{
	struct mCc_st_entry *en = malloc(sizeof(*en));
	if (!en) {
		return NULL;
	}

	en->head = mCc_st_new_item(type, scope);
	en->name = strdup(name);
	en->id = mCc_st_hash(name);
	en->next = NULL;
	return en;
}


void mCc_st_delete_entry(struct mCc_st_entry* entry)
{
	assert(entry);
	free(entry->name);
//	free(entry->id);
	if (NULL != entry->head) {
		mCc_st_delete_item(entry->head);
	}
	if (NULL != entry->next) {
		mCc_st_delete_entry(entry->next);
	}
	free(entry);
}

/* ---------------------------------------------------------------- Tables */
struct mCc_st_table *mCc_st_new_table(const char* type)
{
	struct mCc_st_table *tab = malloc(sizeof(*tab));
	if (!tab) {
		return NULL;
	}

	tab->type = type;
	tab->head = NULL;
	tab->size = 0;
	return tab;
}

void mCc_st_delete_table(struct mCc_st_table* table)
{
	assert(table);
//	free(table->size);
	/*
	for (int i = 0; i < SIZE; i++) {
		struct mCc_st_entry *current_entry = table->entries[i];
		if (NULL != current_entry) {
			mCc_st_delete_entry(current_entry);
		}
	}*/
	if (NULL != table->head) {
		mCc_st_delete_entry(table->head);
	}
	free(table);
}
/* ---------------------------------------------------------------- Insert */
void mCc_st_insert_item(struct mCc_st_entry *entry, struct mCc_st_item *item)
{
	if (NULL != entry->head) {
		//point it to old first node
		item->next = entry->head;
	}

	//point head to new first node
	entry->head = item;
}

void mCc_st_insert_entry(struct mCc_st_table *table, struct mCc_st_entry *entry)
{
	if (NULL != table->head) {
		table->head->next = entry;

	} else {
		table->head = entry;
	}
	++(table->size);
}

/* ---------------------------------------------------------------- Look up */
bool mCc_st_lookup(const char* var, struct mCc_st_table *table)
{
	assert(table);
	/*
	struct mCc_st_entry **tab_entries = table->entries;
	int len = table->size;
	if (strcmp(var, strdup(tab_entries[0]->name))) {
		return true;
	}
	for (int i = 0; i < len; i++) {
		struct mCc_st_entry *current_entry = tab_entries[i];
		if (!current_entry) return false;
		else
 		if (var == strdup(current_entry->name)) {
			return true;
		}
	}*/
	return false;
}
