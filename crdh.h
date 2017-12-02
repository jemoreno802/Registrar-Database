#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct CRDH{
	char course[6];
	char room[15];
	char day[3];
	char hour[5];
};

typedef struct CRDH_chain{ //_list_t_
	struct CRDH *dataEntry;
	struct CRDH_chain *next;
}CRDH_chain;

typedef struct CRDH_hashTable{
	int size;
	CRDH_chain **table;
}CRDH_hashTable;

CRDH_hashTable makeCRDH_hashTable(int size);

int hashCRDH(CRDH_hashTable *ht, char val[6]);

struct CRDH* makeCRDH(char *course, char *room ,char *day, char *hour);

struct CRDH* findCRDH(CRDH_hashTable *ht, struct CRDH *crdh);

void insertCRDH(CRDH_hashTable *ht, struct CRDH *crdh);

int compareCRDH(struct CRDH *listCRDH, struct CRDH *targetCRDH);

void deleteCRDH(CRDH_hashTable *ht, struct CRDH *crdh);

struct CRDH* lookupCRDH(char *course, char *room ,char *day, char *hour, CRDH_hashTable *ht);
struct CRDH* lookupCRDH2(char *course, char *room ,char *day, char *hour, CRDH_hashTable *ht);