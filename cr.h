#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct CR{
	char course[6];
	char room[15];
};

typedef struct CR_chain{ //_list_t_
	struct CR *dataEntry;
	struct CR_chain *next;
}CR_chain;

typedef struct CR_hashTable{
	int size;
	CR_chain **table;
}CR_hashTable;

CR_hashTable makeCR_hashTable(int size);

int hashCR(CR_hashTable *ht, char course[6]);

struct CR* makeCR(char *course, char *room);

struct CR* findCR(CR_hashTable *ht, struct CR *cr);

void insertCR(CR_hashTable *ht, struct CR *cr);

int compareCR(struct CR *listCR, struct CR *targetCR);

void deleteCR(CR_hashTable *ht, struct CR *cr);

struct CR* lookupCR(char *course, char *room, CR_hashTable *ht);
struct CR* lookupCR2(char *course, char *room, CR_hashTable *ht);
