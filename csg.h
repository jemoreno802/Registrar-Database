#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define FAILED NULL;

struct CSG{
	char course[6];
	int id;
	char grade[3];
};

typedef struct CSG_chain{ //_list_t_
	struct CSG *dataEntry;
	struct CSG_chain *next;
}CSG_chain;

typedef struct CSG_hashTable{
	int size;
	CSG_chain **table;
}CSG_hashTable;

CSG_hashTable makeCSG_hashTable(int size);

int hashCSG(CSG_hashTable *ht, char val[6]);

struct CSG* makeCSG(char *course, char *id, char *grade);

struct CSG* findCSG(CSG_hashTable *ht, struct CSG *csg);

void insertCSG(CSG_hashTable *ht, struct CSG *csg);
void insertCSG2(CSG_hashTable *ht, struct CSG *csg);

int compare(struct CSG *listCSG, struct CSG *targetCSG);

void deleteCSG(CSG_hashTable *ht, struct CSG *csg);

struct CSG* lookupCSG(char *course, char *id, char *grade, CSG_hashTable *ht);