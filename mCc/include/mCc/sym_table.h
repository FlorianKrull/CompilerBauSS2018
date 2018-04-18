#ifndef MCC_SYM_TABLE_H
#define MCC_SYM_TABLE_H

#ifdef __cplusplus
extern "C" {
#endif

#define SIZE 99

struct Entry {
	char *name;
	int count;
	struct Entry *next;
};

struct Table {
	struct Entry entries[100];
	struct Table *parent;
};

typedef struct Entry *EntryPtr;

#ifdef __cplusplus
}
#endif

#endif
