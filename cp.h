#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct CP{
	char course[6];
	char preReq[6];
};

typedef struct CP_chain{ //_list_t_
	struct CP *dataEntry;
	struct CP_chain *next;
}CP_chain;

typedef struct CP_hashTable{
	int size;
	CP_chain **table;
}CP_hashTable;

CP_hashTable makeCP_hashTable(int size);

int hashCP(CP_hashTable *ht, char course[6], char preReq[6]);

struct CP* makeCP(char *course, char *preReq);

struct CP* findCP(CP_hashTable *ht, struct CP *cp);

void insertCP(CP_hashTable *ht, struct CP *cp);

int compareCP(struct CP *listCP, struct CP *targetCP);

void deleteCP(CP_hashTable *ht, struct CP *cp);

struct CP* lookupCP(char *course, char *preReq, CP_hashTable *ht);
