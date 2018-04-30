#include <gtest/gtest.h>
#include <stdbool.h>

#include "mCc/sym_table.h"

/* ---------------------------------------------------------------- Initialization */
TEST(sym_table, NewItem_1)
{
	struct mCc_st_item *item = mCc_st_new_item("int", 1);

	ASSERT_STREQ("int", item->type);
	ASSERT_EQ(1, item->scope);
	ASSERT_EQ(NULL, item->next);

	mCc_st_delete_item(item);
}

TEST(sym_table, NewEntry_1)
{
	struct mCc_st_entry *entry = mCc_st_new_entry("my_var", "bool", 2);
	ASSERT_STREQ("my_var", entry->name);

	auto head = entry->head;
	ASSERT_STREQ("bool", head->type);
	ASSERT_EQ(2, head->scope);

	ASSERT_EQ(NULL, entry->next);

	mCc_st_delete_entry(entry);
}

TEST(sym_table, NewTable_1)
{
	struct mCc_st_table *table = mCc_st_new_table();
	ASSERT_EQ(0, table->size);
	ASSERT_EQ(NULL, table->head);

	mCc_st_delete_table(table);
}

/* ---------------------------------------------------------------- Insert */
TEST(sym_table, InsertItem_1)
{
	struct mCc_st_entry *entry = mCc_st_new_entry("_1st", "bool", 2);
	struct mCc_st_item *new_item = mCc_st_new_item("float", 1);
	ASSERT_STREQ("_1st", entry->name);

	mCc_st_insert_item(entry, new_item);

	auto head = entry->head;
	ASSERT_STREQ("float", head->type);
	ASSERT_EQ(1, head->scope);

	auto _2nd_item = head->next;
	ASSERT_STREQ("bool", _2nd_item->type);
	ASSERT_EQ(2, _2nd_item->scope);

	mCc_st_delete_entry(entry);
}

TEST(sym_table, InsertEntry_1)
{
	struct mCc_st_entry *entry = mCc_st_new_entry("a1", "string", 3);
	struct mCc_st_item *new_item1 = mCc_st_new_item("int", 2);
	struct mCc_st_item *new_item2 = mCc_st_new_item("float", 1);

	mCc_st_insert_item(entry, new_item1);
	mCc_st_insert_item(entry, new_item2);

	struct mCc_st_table *table = mCc_st_new_table();
	mCc_st_insert_entry(table, entry);

	ASSERT_EQ(1, table->size);

	auto tab_entry1 = table->head;
	ASSERT_STREQ("a1", tab_entry1->name);

	auto head = tab_entry1->head;
	ASSERT_STREQ("float", head->type);
	ASSERT_EQ(1, head->scope);

	auto next_item1 = head->next;
	ASSERT_STREQ("int", next_item1->type);
	ASSERT_EQ(2, next_item1->scope);

	auto next_item2 = next_item1->next;
	ASSERT_STREQ("string", next_item2->type);
	ASSERT_EQ(3, next_item2->scope);

	mCc_st_delete_table(table);
}

/* ---------------------------------------------------------------- Delete */
TEST(sym_table, RemoveItem_1)
{
	struct mCc_st_entry *entry = mCc_st_new_entry("_1st", "bool", 2);

	mCc_st_remove_item(entry, entry->head);

	ASSERT_EQ(NULL, entry->head);

	mCc_st_delete_entry(entry);
}

TEST(sym_table, RemoveItem_2)
{
	struct mCc_st_entry *entry = mCc_st_new_entry("_1st", "bool", 1);
	struct mCc_st_item *new_item = mCc_st_new_item("float", 2);

	mCc_st_insert_item(entry, new_item);
	mCc_st_remove_item(entry, new_item);

	auto head = entry->head;
	ASSERT_STREQ("bool", head->type);
	ASSERT_EQ(1, head->scope);
	ASSERT_EQ(NULL, head->next);

	mCc_st_delete_entry(entry);
}

TEST(sym_table, RemoveItem_3)
{
	struct mCc_st_entry *entry = mCc_st_new_entry("_1st", "bool", 1);
	struct mCc_st_item *new_item1 = mCc_st_new_item("float", 2);
	struct mCc_st_item *new_item2 = mCc_st_new_item("string", 3);

	mCc_st_insert_item(entry, new_item1);
	mCc_st_insert_item(entry, new_item2);

	mCc_st_remove_item(entry, new_item1);

	auto head = entry->head;
	ASSERT_STREQ("string", head->type);
	ASSERT_EQ(3, head->scope);

	auto next = head->next;
	ASSERT_STREQ("bool", next->type);
	ASSERT_EQ(1, next->scope);

	mCc_st_delete_entry(entry);
}

TEST(sym_table, RemoveEntry_1)
{
	struct mCc_st_table *table = mCc_st_new_table();
	struct mCc_st_entry *new_entry = mCc_st_new_entry("c2", "float", 2);

	mCc_st_insert_entry(table, new_entry);
	mCc_st_remove_entry(table, new_entry);

	ASSERT_EQ(0, table->size);
	ASSERT_EQ(NULL, table->head);

	mCc_st_delete_table(table);
}

// Remove head
TEST(sym_table, RemoveEntry_2)
{
	struct mCc_st_table *table = mCc_st_new_table();
	struct mCc_st_entry *new_entry1 = mCc_st_new_entry("a1", "float", 2);
	struct mCc_st_entry *new_entry2 = mCc_st_new_entry("b2", "string", 3);

	mCc_st_insert_entry(table, new_entry1);
	mCc_st_insert_entry(table, new_entry2);

	mCc_st_remove_entry(table, new_entry1);

	auto entry2 = table->head;
	ASSERT_STREQ("b2", entry2->name);
	auto entry2_item = entry2->head;
	ASSERT_STREQ("string", entry2_item->type);
	ASSERT_EQ(3, entry2_item->scope);

	ASSERT_EQ(NULL, entry2->next);

	mCc_st_delete_table(table);
}

//Remove last element
TEST(sym_table, RemoveEntry_3)
{
	struct mCc_st_table *table = mCc_st_new_table();
	struct mCc_st_entry *new_entry1 = mCc_st_new_entry("a1", "float", 2);
	struct mCc_st_entry *new_entry2 = mCc_st_new_entry("b2", "string", 3);

	mCc_st_insert_entry(table, new_entry1);
	mCc_st_insert_entry(table, new_entry2);

	mCc_st_remove_entry(table, new_entry2);

	auto entry1 = table->head;
	ASSERT_STREQ("a1", entry1->name);

	auto entry1_item = entry1->head;
	ASSERT_STREQ("float", entry1_item->type);
	ASSERT_EQ(2, entry1_item->scope);

	ASSERT_EQ(NULL, entry1->next);

	mCc_st_delete_table(table);
}

/* ---------------------------------------------------------------- Lookup */
/*
TEST(sym_table, Table_Lookup_1)
{
	struct mCc_st_item *item = mCc_st_new_item("float", 2, NULL);
	struct mCc_st_entry *entry = mCc_st_new_entry("x", item);

	struct mCc_st_entry **arr_entries = (struct mCc_st_entry **) calloc(50, sizeof(struct mCc_st_entry));
	struct mCc_st_table *table = mCc_st_new_table(arr_entries);

	for (int i = 0; i < table->size; i++) {
		arr_entries[0] = entry;
	}

	bool result = mCc_st_lookup("x", table);
	ASSERT_EQ(true, result);
	ASSERT_STREQ("x", table->entries[0]->name);

	mCc_st_delete_table(table);
}
*/
