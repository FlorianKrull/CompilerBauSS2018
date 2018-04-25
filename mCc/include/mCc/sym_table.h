#ifndef MCC_SYM_TABLE_H
#define MCC_SYM_TABLE_H

#ifdef __cplusplus
extern "C" {
#endif

//#define SIZE 50	//Size of each table
//#define HASH_VALUE 131	//A hash value should a minimum prime number larger than ASCII's alphabet size

/* Declare the symbol table as a hashtable of list.
 * More information: https://www.d.umn.edu/~rmaclin/cs5641/Notes/L15_SymbolTable.pdf
 * When processing a scope S, the structure of the symbol table should look
 * like this:
 *
 * f : void, 1
 * a : int, 2
 * x : char, 3 -> int, 2
 * y : char, 3 -> float 2
 * z : bool, 3
 *
 * The structure corresponding to the following code:
 * void f(int a) {
 * int x; float y;
 * if (...) {
 * char x, y;
 * bool z;
 * ...
 * }
 * }
 * void main() {
 * f(10);
 * }
 * */

/* The basic element; storing declaration of a variables and its scope, respectively.
 * It's also organized as linked list; in which next pointer hold the address of the
 * next node. */
struct mCc_st_item {
	char *type;
	int scope;
	struct mCc_st_item *next;
};

/* The variable bucket, in which a head pointer pointing to current node. */
struct mCc_st_entry {
	char *name;
	int id;
	struct mCc_st_item *head;
};

/* Hash pointer, stores an array of pointers to entries, and its level of scope. */
struct mCc_st_table {
	struct mCc_st_entry** entries;
	int scope;
};

// Init and delete function
struct mCc_st_item *mCc_st_new_item(const char* type, int scope, struct mCc_st_item* next);

struct mCc_st_entry *mCc_st_new_entry(const char* name, struct mCc_st_item* head);

struct mCc_st_table *mCc_st_new_table(struct mCc_st_entry **entries);

void mCc_st_delete_item(struct mCc_st_item* item);

void mCc_st_delete_entry(struct mCc_st_entry* entry);

void mCc_st_delete_table(struct mCc_st_table* table);

//Hash function
int mCc_st_hash(const char* str);

#ifdef __cplusplus
}
#endif

#endif
