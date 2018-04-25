#include "mCc/sym_table.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SIZE 50	//Size of each table
#define HASH_VALUE 131	//A hash value should a minimum prime number larger than ASCII's alphabet size

/* ---------------------------------------------------------------- Items */
struct mCc_st_item *mCc_st_new_item(const char* type, int scope, struct mCc_st_item *next)
{
//	assert(next);
	struct mCc_st_item *item = malloc(sizeof(*item));
	if (!item) {
		return NULL;
	}
	item->type = strdup(type);
	item->scope = scope;
	item->next = next;
	return item;
}

void mCc_st_delete_item(struct mCc_st_item* item)
{
	assert(item);
	free(item->type);
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

struct mCc_st_entry *mCc_st_new_entry(const char* name, struct mCc_st_item* head)
{
	struct mCc_st_entry *en = malloc(sizeof(*en));
	if (!en) {
		return NULL;
	}
	en->name = strdup(name);
	en->head = head;
	en->id = mCc_st_hash(name);
	return en;
}


void mCc_st_delete_entry(struct mCc_st_entry* entry)
{
	assert(entry);
	free(entry->name);
//	if (NULL != entry->head) {
//		mCc_st_delete_item(entry->head);
//	}
	free(entry);
}

/* ---------------------------------------------------------------- Tables */
struct mCc_st_table *mCc_st_new_table(struct mCc_st_entry **entries)
{
	struct mCc_st_table *tab = malloc(sizeof(*tab));
	if (!tab) {
		return NULL;
	}

	tab->entries = entries;
	return tab;
}

void mCc_st_delete_table(struct mCc_st_table* table)
{
	assert(table);

	for (int i = 0; i < SIZE; i++) {
		struct mCc_st_entry *current_entry = table->entries[i];
		if (NULL != current_entry) {
			mCc_st_delete_entry(current_entry);
		}
	}
	free(table->entries);
	free(table);
}
