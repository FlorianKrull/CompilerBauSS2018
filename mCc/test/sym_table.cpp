#include <gtest/gtest.h>

#include "mCc/sym_table.h"

TEST(sym_table, NewItem_1)
{
	struct mCc_st_item *item = mCc_st_new_item("int", 1, NULL);
	mCc_st_delete_item(item);
}

TEST(sym_table, NewEntry_1)
{
	struct mCc_st_item *item2 = mCc_st_new_item("int", 2, NULL);
	struct mCc_st_item *item1 = mCc_st_new_item("bool", 1, item2); // item1->item2
	struct mCc_st_entry *entry = mCc_st_new_entry("my_var", item2);
	mCc_st_delete_entry(entry);
}

TEST(sym_table, NewTable_1)
{
	struct mCc_st_item *item = mCc_st_new_item("float", 2, NULL);
	struct mCc_st_entry *entry = mCc_st_new_entry("x", item);

	struct mCc_st_entry **arr_entries = (struct mCc_st_entry **) calloc(50, sizeof(struct mCc_st_entry));
	arr_entries[0] = entry;

	struct mCc_st_table *table = mCc_st_new_table(arr_entries);

	mCc_st_delete_table(table);
}
