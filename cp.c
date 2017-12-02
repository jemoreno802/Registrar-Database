#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cp.h"

/*
Jacquelyn Moreno
This file contains all the methods for the Course-Prerequisite relation
*/

CP_hashTable makeCP_hashTable(int size){
	CP_hashTable *newTable;
	
	if(size<1){ //cannot make a table of this size
		newTable = NULL;
	}if((newTable = malloc(sizeof(CP_hashTable)))== NULL){
		newTable = NULL;
	}if((newTable->table = malloc(sizeof(CP_chain *) * size)) == NULL){
		newTable = NULL;
	}
	int i;
	newTable->size = size;
	for(i=0; i<size;i++){
		newTable->table[i]= NULL;
	}
	//printf("MADE\n");
	return *newTable;
}

/*
	take in a course and preReq value, sum the ASCII code for each character, modulo the table size
*/
int hashCP(CP_hashTable *ht, char course[6], char preReq[6]){
	unsigned int hashValue;
	hashValue = 0;
	int i = strlen(course);
	int j = strlen(preReq);
	while(i>0){
		hashValue += course[i-1]; //add each character in course
		i--;
	}while(j>0){
		hashValue += preReq[j-1]; //add each character in preReq
		j--;
	}
	return hashValue%(ht->size);
}

/*
	make a CP tuple from character array inputs
*/
struct CP* makeCP(char *course, char *preReq){
	struct CP *cp = (struct CP *)malloc(sizeof(struct CP));
	strcpy((cp->course), course);
	strcpy((cp->preReq), preReq);	
	return cp;
}

/*
	find a given CP in a given hashtable
*/
struct CP* findCP(CP_hashTable *ht, struct CP *cp){
	CP_chain *list;
	int found = 0;
	unsigned int hashValue = hashCP(ht, cp->course, cp->preReq);
	for((list = ht->table[hashValue]); list != NULL; list = list->next){
		if(list->dataEntry ==NULL){
			found = 0;
			//do nothing
		}else if((strcmp(cp->course, list->dataEntry->course) == 0) && (strcmp(cp->preReq,list->dataEntry->preReq)==0)){
			found = 1; //found the given CP
		}
	}
	if(found == 0){
		cp = NULL;
	}
	return cp;
}

/*
	insert a given CP into a given hashtable
*/
void insertCP(CP_hashTable *ht, struct CP *cp){
	printf("insert((\"%s\" , \"%s\"), course-prerequisite)\n",cp->course, cp->preReq);
	CP_chain *head = malloc(sizeof(CP_chain));
	struct CP *test = findCP(ht, cp);
	unsigned int hashValue = hashCP(ht, cp->course, cp->preReq); //hash the coursevalue
	if(test == NULL){ // if lookup returned nothing, insert into linked list
		head->dataEntry = cp;
		head->next = ht->table[hashValue];
		ht->table[hashValue] = head;
	}else{
		printf("ALREADY IN BUCKET\n");
	}
}

/*
	compare two CP tuple's, if they are equal return 1
*/
int compareCP(struct CP *listCP, struct CP *targetCP){
	if((strcmp(targetCP->course, listCP->course) == 0) && (strcmp(targetCP->preReq,listCP->preReq)==0)){
		return 1;
	}else{
		return 0;
	}
}

/*
	delete a given CP from a given hashtable
*/
void deleteCP(CP_hashTable *ht, struct CP *cp){
	printf("\ndelete((\"%s\" , \"%s\"), course-prerequisite)\n",cp->course, cp->preReq);
	struct CP *test = findCP(ht, cp);
	unsigned int hashValue = hashCP(ht, cp->course, cp->preReq); //hash the coursevalue
	if(test == NULL){
		printf("NOTHING TO DELETE\n");
	}else{
		CP_chain *list;
		int i =0;
		for((list = ht->table[hashValue]); list != NULL; list = list->next){
			if(i == 0){ //deleting head of list
				if(compareCP(list->dataEntry, cp)==1){
					printf("	FOUND AND DELETED\n");					
					if(list->next == NULL){  
						list->dataEntry = NULL;
						list->next = NULL;
					}else{//deleting second element in list
						if(compareCP((list->next)->dataEntry, cp)==1){
							printf("	FOUND AND DELETED\n");					
							list->next = (list->next)->next;
						}
					}
				}
			//deleting a middle or end value in list
			}else if(list->next !=NULL){ 
				if(compareCP((list->next)->dataEntry, cp)==1){
					printf("	FOUND AND DELETED\n");					
					list->next = (list->next)->next;
				}
			}
			i++;
		}
	}
}

/*
	lookup a CP (given by individual elements) in a given hashtable
*/
struct CP* lookupCP(char *course, char *preReq, CP_hashTable *ht){
	printf("\nlookup((\"%s\" , \"%s\"), course-prerequisite)\n",course, preReq);
	struct CP *cp = makeCP(course, preReq);
	if(findCP(ht, cp)==NULL){
		printf("	NOT FOUND, %s is NOT a pre-requisite of %s\n", preReq, course);
	}else{
		printf("	FOUND, %s is a pre-requisite of %s\n", preReq, course);
	}
	return findCP(ht, cp);
}
