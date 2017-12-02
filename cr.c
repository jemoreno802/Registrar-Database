#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cr.h"

/*
Jacquelyn Moreno
This file contains all the methods for the Course-Room relation
*/

CR_hashTable makeCR_hashTable(int size){
	CR_hashTable *newTable;

	if(size<1){ //cannot make a table of this size
		newTable = NULL;
	}if((newTable = malloc(sizeof(CR_hashTable)))== NULL){
		newTable = NULL;
	}if((newTable->table = malloc(sizeof(CR_chain *) * size)) == NULL){
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
	take in a course, sum the ASCII code for each character, modulo the table size
*/
int hashCR(CR_hashTable *ht, char course[6]){
	unsigned int hashValue;
	hashValue = 0;
	int i = strlen(course);
	while(i>0){
		hashValue += course[i-1]; //add each character in course
		i--;
	}
	return hashValue%(ht->size);
}

/*
	make a CR tuple from character array inputs
*/
struct CR* makeCR(char *course, char *room){
	struct CR *cr = (struct CR *)malloc(sizeof(struct CR));
	strcpy((cr->course), course);
	strcpy((cr->room), room);	
	return cr;
}

/*
	find a given CR in a given hashtable
*/
struct CR* findCR(CR_hashTable *ht, struct CR *cr){
	CR_chain *list;
	int found = 0;
	unsigned int hashValue = hashCR(ht, cr->course);
	for((list = ht->table[hashValue]); list != NULL; list = list->next){
		if(list->dataEntry ==NULL){
			found = 0;
			//do nothing
		}else if((strcmp(cr->course, list->dataEntry->course) == 0) && (strcmp(cr->room,list->dataEntry->room)==0)){
			found = 1; //found the given CR
		}
	}
	if(found == 0){
		cr = NULL;
		//printf("NOT FOUND\n");
	}
	return cr;
}

/*
	insert a given CR into a given hashtable
*/
void insertCR(CR_hashTable *ht, struct CR *cr){
	printf("insert((\"%s\" , \"%s\"), CR)\n",cr->course, cr->room);
	CR_chain *head = malloc(sizeof(CR_chain));
	struct CR *test = findCR(ht, cr);
	unsigned int hashValue = hashCR(ht, cr->course); //hash the coursevalue
	if(test == NULL){ // if lookup returned nothing, insert into linked list
		head->dataEntry = cr;
		head->next = ht->table[hashValue];
		ht->table[hashValue] = head;
	}else{
		printf("ALREADY IN BUCKET\n");
	}
}

/*
	compare two CR tuples, if they are equal return 1
*/
int compareCR(struct CR *listCR, struct CR *targetCR){
	if((strcmp(targetCR->course, listCR->course) == 0) && (strcmp(targetCR->room,listCR->room)==0)){
		return 1;
	}else{
		return 0;
	}
}

/*
	delete a given CR from a given hashtable
*/
void deleteCR(CR_hashTable *ht, struct CR *cr){
	printf("\ndelete((\"%s\" , \"%s\"), CR)\n",cr->course, cr->room);

	struct CR *test = findCR(ht, cr);
	unsigned int hashValue = hashCR(ht, cr->course); //hash the coursevalue
	if(test == NULL){
		printf("NOTHING TO DELETE\n");
	}else{
		CR_chain *list;
		int i =0;
		for((list = ht->table[hashValue]); list != NULL; list = list->next){
			if(i == 0){ //deleting head of list
				if(compareCR(list->dataEntry, cr)==1){
					printf("	FOUND AND DELETED\n");					
					if(list->next == NULL){  
						list->dataEntry = NULL;
						list->next = NULL;
					}else{//deleting second element in list
						if(compareCR((list->next)->dataEntry, cr)==1){
							printf("	FOUND AND DELETED\n");					
							list->next = (list->next)->next;
						}
					}
				}
			}else if(list->next !=NULL){ 
				if(compareCR((list->next)->dataEntry, cr)==1){
					printf("	FOUND AND DELETED\n");					
					list->next = (list->next)->next;
				}
			}
			i++;
		}
	}
}

/*
	lookup a CR (given by individual elements) in a given hashtable
*/
struct CR* lookupCR(char *course, char *room, CR_hashTable *ht){
	printf("\nlookup((\"%s\" , \"%s\"), CR)\n",course, room);
	struct CR *cr = NULL;
	CR_chain *list;
	int found = 0;
	unsigned int hashValue = hashCR(ht, course);
	if(strcmp(room, " ") == 0){ //if room not specified
		for((list = ht->table[hashValue]); list != NULL; list = list->next){
			if(list->dataEntry == NULL){
				printf("	NOT FOUND\n");
				return NULL;
			}else {
				found = 1; //found the given CSG
				cr = list->dataEntry;
				printf("	FOUND, %s meets in %s\n", cr->course, cr->room);
				return cr;
			}
		}
	}else{
	struct CR *cr = makeCR(course, room);
	if(findCR(ht, cr)==NULL){
		printf("	NOT FOUND\n");
	}else{
		printf("	FOUND, %s meets in %s\n", course, room);
	}
	return findCR(ht, cr);
	}
	return cr;
}

struct CR* lookupCR2(char *course, char *room, CR_hashTable *ht){
	struct CR *cr = NULL;
	CR_chain *list;
	int found = 0;
	unsigned int hashValue = hashCR(ht, course);
	if(strcmp(room, " ") == 0){ //if room not specified
		for((list = ht->table[hashValue]); list != NULL; list = list->next){
			if(list->dataEntry == NULL){
				return NULL;
			}else {
				found = 1; //found the given CSG
				cr = list->dataEntry;
				return cr;
			}
		}
	}else{
		struct CR *cr = makeCR(course, room);
		return findCR(ht, cr);
	}
	return cr;
}
