#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct CDH{
	char course[6];
	char day[3];
	char hour[5];
};

typedef struct CDH_chain{ //_list_t_
	struct CDH *dataEntry;
	struct CDH_chain *next;
}CDH_chain;

typedef struct CDH_hashTable{
	int size;
	CDH_chain **table;
}CDH_hashTable;

CDH_hashTable makeCDH_hashTable(int size);

int hashCDH(CDH_hashTable *ht, char val[6]);

struct CDH* makeCDH(char *course, char *day, char *hour);

struct CDH* findCDH(CDH_hashTable *ht, struct CDH *cdh);

void insertCDH(CDH_hashTable *ht, struct CDH *cdh);
void insertCDH2(CDH_hashTable *ht, struct CDH *cdh);

int compareCDH(struct CDH *listCDH, struct CDH *targetCDH);

void deleteCDH(CDH_hashTable *ht, struct CDH *cdh);

struct CDH* lookupCDH(char *course, char *day, char *hour, CDH_hashTable *ht);
struct CDH* lookupCDH2(char *course, char *day, char *hour, CDH_hashTable *ht);