#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cdh.h"

/*
Jacquelyn Moreno
This file contains all the methods for the Course-Day-Hour relation
*/

CDH_hashTable makeCDH_hashTable(int size){
	CDH_hashTable *newTable;
	if(size<1){ //cannot make a table of this size
		newTable = NULL;
	}if((newTable = malloc(sizeof(CDH_hashTable)))== NULL){
		newTable = NULL;
	}if((newTable->table = malloc(sizeof(CDH_chain *) * size)) == NULL){
		newTable = NULL;
	}
	int i;
	newTable->size = size;
	for(i=0; i<size;i++){
		newTable->table[i]= NULL;
	}
	return *newTable;
}

/*
	take in a course value, sum the ASCII code for each character, modulo the table size
*/
int hashCDH(CDH_hashTable *ht, char val[6]){
	unsigned int hashValue;
	hashValue = 0;
	int i = strlen(val);
	while(i>0){
		hashValue += val[i-1]; //add each character in course
		i--;
	}
	return hashValue%(ht->size);
}

/*
	make a CDH tuple from character array inputs
*/
struct CDH* makeCDH(char *course, char *day, char *hour){
	struct CDH *cdh = (struct CDH *)malloc(sizeof(struct CDH));
	strcpy((cdh->course), course);
	strcpy((cdh->day), day);
	strcpy((cdh->hour), hour );
	return cdh;
}

/*
	find a given CDH in a given hashtable
*/
struct CDH* findCDH(CDH_hashTable *ht, struct CDH *cdh){
	CDH_chain *list;
	int found = 0;
	unsigned int hashValue = hashCDH(ht, cdh->course);
	for((list = ht->table[hashValue]); list != NULL; list = list->next){ //check each value in the list at this bucket
		if(list->dataEntry ==NULL){
			found = 0;
			//do nothing
		}else if(compareCDH(list->dataEntry, cdh)==1){
			found = 1; //found the given CDH
		}
	}if(found == 0){
		cdh = NULL;
	}
	return cdh;
}

/*
	insert a given CDH into a given hashtable
*/
void insertCDH(CDH_hashTable *ht, struct CDH *cdh){
	printf("insert((\"%s\" , \"%s\",\"%s\"), CDH)\n", cdh->course, cdh->day, cdh->hour);
	CDH_chain *head = malloc(sizeof(CDH_chain));
	struct CDH *test = findCDH(ht, cdh);
	unsigned int hashValue = hashCDH(ht, cdh->course); //hash the coursevalue
	if(test == NULL){ // if lookup returned nothing, insert into linked list
		head->dataEntry = cdh;
		head->next = ht->table[hashValue];
		ht->table[hashValue] = head;
	}else{
		printf("ALREADY IN BUCKET\n");
	}
}
void insertCDH2(CDH_hashTable *ht, struct CDH *cdh){
	CDH_chain *head = malloc(sizeof(CDH_chain));
	struct CDH *test = findCDH(ht, cdh);
	unsigned int hashValue = hashCDH(ht, cdh->course); //hash the coursevalue
	if(test == NULL){ // if lookup returned nothing, insert into linked list
		head->dataEntry = cdh;
		head->next = ht->table[hashValue];
		ht->table[hashValue] = head;
	}
}

/*
	compare two CDH tuple's, if they are equal return 1
*/
int compareCDH(struct CDH *listCDH, struct CDH *targetCDH){
	if((strcmp(targetCDH->course, listCDH->course) == 0) && (strcmp(targetCDH->day,listCDH->day)==0)&& (strcmp(targetCDH->hour,listCDH->hour)==0)){
		return 1;
	}else{
		return 0;
	}
}

/*
	delete a given CDH from a given hashtable
*/
void deleteCDH(CDH_hashTable *ht, struct CDH *cdh){
	printf("\ndelete((\"%s\" , \"%s\",\"%s\"), CDH)\n", cdh->course, cdh->day, cdh->hour);
	struct CDH *test = findCDH(ht, cdh);
	unsigned int hashValue = hashCDH(ht, cdh->course); //hash the coursevalue
	if(test == NULL){
		printf("NOTHING TO DELETE\n");
	}else{
		CDH_chain *list;
		int i =0;
		for((list = ht->table[hashValue]); list != NULL; list = list->next){
			if(i == 0){ //deleting head of list
				if(compareCDH(list->dataEntry, cdh)==1){
					printf("	FOUND AND DELETED\n");					
					if(list->next == NULL){  
						list->dataEntry = NULL;
						list->next= NULL;
					}else{//if next is not null, set entire list to start at list->next
						ht->table[hashValue] = list->next; 
					}
				}//deleting second element in list
				if(compareCDH((list->next)->dataEntry, cdh)==1){
					printf("	FOUND AND DELETED\n");	
					list->next = (list->next)->next;
				}
			//deleting a middle or end value in list
			}else if(list->next !=NULL){ 
				if(compareCDH((list->next)->dataEntry, cdh)==1){
					printf("	FOUND AND DELETED\n");	
					list->next = (list->next)->next;
				}
			}
			i++;
		}
	}
}

/*
	lookup a CDH (given by individual elements) in a given hashtable
*/
struct CDH* lookupCDH(char *course, char *day, char *hour, CDH_hashTable *ht){
	printf("\nlookup((\"%s\" ,\"%s\", \"%s\"), CDH)\n",course, day, hour);
	struct CDH *cdh = makeCDH(course, day,hour);
	if(findCDH(ht, cdh)==NULL){
		printf("	NOT FOUND\n");
	}else{
		printf("	FOUND, %s meets at %s on %s\n", course, hour, day);
	}
	return findCDH(ht, cdh);
}

struct CDH* lookupCDH2(char *course, char *day, char *hour, CDH_hashTable *ht){
	struct CDH *cdh = makeCDH(course, day,hour);
	return findCDH(ht, cdh);
}
