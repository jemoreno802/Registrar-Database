#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FAILED NULL;

struct SNAP{
	int id;
	char name[30];
	char address[50];
	char phone[9];
};

typedef struct SNAP_chain{ //_list_t_
	struct SNAP *dataEntry;
	struct SNAP_chain *next;
}SNAP_chain;

typedef struct SNAP_hashTable{
	int size;
	SNAP_chain **table;
}SNAP_hashTable;

SNAP_hashTable makeSNAP_hashTable(int size);

int hashSNAP(SNAP_hashTable *ht, char* id);

struct SNAP* makeSNAP(char *id, char *name, char *address, char *phone);

struct SNAP* findSNAP(SNAP_hashTable *ht, struct SNAP *snap);

void insertSNAP(SNAP_hashTable *ht, struct SNAP *snap);

int compareSNAP(struct SNAP *listSNAP, struct SNAP *targetSNAP);

void deleteSNAP(SNAP_hashTable *ht, struct SNAP *snap);

struct SNAP* lookupSNAP(char *id, char *name, char *address,char *phone, SNAP_hashTable *ht);

struct SNAP* lookupSNAP2(char *id, char *name, char *address,char *phone, SNAP_hashTable *ht);