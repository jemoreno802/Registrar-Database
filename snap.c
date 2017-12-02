#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snap.h"

/*
Jacquelyn Moreno
This file contains all the methods for the StudentId-Name-Address-Phone relation
*/
SNAP_hashTable makeSNAP_hashTable(int size){
	SNAP_hashTable *newTable;
	
	if(size<1){ //cannot make a table of this size
		newTable = NULL;
	}if((newTable = malloc(sizeof(SNAP_hashTable)))== NULL){
		newTable = NULL;
	}if((newTable->table = malloc(sizeof(SNAP_chain *) * size)) == NULL){
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
	take in a name, sum the ASCII code for each character, modulo the table size
*/
int hashSNAP(SNAP_hashTable *ht, char *name){
	unsigned int hashValue;
	hashValue = 0;
	int i = strlen(name);
	while(i>0){
		hashValue += name[i-1]; //add each character in course
		i--;
	}
	return hashValue%(ht->size);
}

/*
	make a SNAP tuple from character array inputs
*/
struct SNAP* makeSNAP(char *id, char *name, char *address, char *phone){
	struct SNAP *snap = (struct SNAP *)malloc(sizeof(struct SNAP));
	int studentId = atoi(id);
	snap->id = studentId;
	strcpy((snap->name), name);
	strcpy((snap->address), address);
	strcpy((snap->phone), phone);
	return snap;
}

/*
	find a given SNAP in a given hashtable
*/
struct SNAP* findSNAP(SNAP_hashTable *ht, struct SNAP *snap){
	SNAP_chain *list;
	int found = 0;
	unsigned int hashValue = hashSNAP(ht, snap->name);
	for((list = ht->table[hashValue]); list != NULL; list = list->next){
		if(list->dataEntry ==NULL){
			found = 0;
			//do nothing
		}else if(compareSNAP(list->dataEntry, snap) == 1){
			found = 1; //found the given SNAP
		}
	}if(found == 0){
		snap = FAILED;
	}
	return snap;
}

/*
	insert a given SNAP into a given hashtable
*/
void insertSNAP(SNAP_hashTable *ht, struct SNAP *snap){
	printf("insert(( \"%d\", \"%s\" , \"%s\", \"%s\"), SNAP)\n", snap->id, snap->name, snap->address, snap->phone);
	SNAP_chain *head = malloc(sizeof(SNAP_chain));
	struct SNAP *test = findSNAP(ht, snap);
	unsigned int hashValue = hashSNAP(ht, snap->name); //hash the id
	if(test == NULL){ // if lookup returned nothing, insert into linked list
		head->dataEntry = snap;
		head->next = ht->table[hashValue];
		ht->table[hashValue] = head;
	}else{
		printf("ALREADY IN BUCKET\n");
	}
}

/*
	compare two SNAP tuples, if they are equal return 1
*/
int compareSNAP(struct SNAP *listSNAP, struct SNAP *targetSNAP){
	if((targetSNAP->id == listSNAP->id) && (strcmp(targetSNAP->name, listSNAP->name) == 0) &&  (strcmp(targetSNAP->address,listSNAP->address)==0)&& (strcmp(targetSNAP->phone,listSNAP->phone)==0)){
		//printf("EQUAL\n");
		return 1;
	}else{
		return 0;
	}
}

/*
	delete a given SNAP from a given hashtable
*/
void deleteSNAP(SNAP_hashTable *ht, struct SNAP *snap){
	printf("\ndelete((\"%d\" , \"%s\" , \"%s\" , \"%s\"), SNAP)\n",snap->id, snap->name, snap->address, snap->phone);

	struct SNAP *test = findSNAP(ht, snap);
	unsigned int hashValue = hashSNAP(ht, snap->name); //hash the coursevalue
	if(test == NULL){
		printf("NOTHING TO DELETE\n");
	}else{
		SNAP_chain *list;
		int i =0;
		for((list = ht->table[hashValue]); list != NULL; list = list->next){
			if(i == 0){ //deleting head of list
				if(compareSNAP(list->dataEntry, snap)==1){
					printf("	FOUND AND DELETED\n");					
					if(list->next == NULL){  
						list->dataEntry = NULL;
						list->next = NULL;
					}else{//deleting second element in list
						if(compareSNAP((list->next)->dataEntry, snap)==1){
							printf("	FOUND AND DELETED\n");					
							list->next = (list->next)->next;
						}
					}
				}
			//deleting a middle or end value in list
			}else if(list->next !=NULL){ 
				if(compareSNAP((list->next)->dataEntry, snap)==1){
					printf("	FOUND AND DELETED\n");					
					list->next = (list->next)->next;
				}
			}
			i++;
		}
	}
}

struct SNAP* lookupSNAP(char *id, char *name, char *address,char *phone, SNAP_hashTable *ht){
	printf("\nlookup((\"%s\" , \"%s\" , \"%s\" , \"%s\"), SNAP)\n",id,name,address,phone);
	struct SNAP *snap = NULL;
	if((strcmp(id, " ") == 0) && (strcmp(address, " ") == 0)&&(strcmp(phone, " ") == 0)){
		
		SNAP_chain *list;
		unsigned int hashValue = hashSNAP(ht, name);

		for((list = ht->table[hashValue]); list != NULL; list = list->next){
			if(list->dataEntry == NULL){
				printf("	NOT FOUND\n");
				return NULL;
			}else if(strcmp(name, list->dataEntry->name)==0){
				snap = list->dataEntry;
				printf("	FOUND, studentID: %d, name: %s, address: %s, phone: %s\n", snap->id, snap->name, snap->address, snap->phone);
			}
		}
	}
	else{
		struct SNAP *snap = makeSNAP(id, name, address, phone);
		if(findSNAP(ht, snap)==NULL){
			printf("	NOT FOUND\n");
		}else{
			printf("	FOUND\n");	}

		return findSNAP(ht, snap);
	}
	return snap;
}

/*
	lookup a SNAP (given by individual elements) in a given hashtable
*/
struct SNAP* lookupSNAP2(char *id, char *name, char *address,char *phone, SNAP_hashTable *ht){
	struct SNAP *snap = NULL;
	if((strcmp(id, " ") == 0) && (strcmp(address, " ") == 0)&&(strcmp(phone, " ") == 0)){
		SNAP_chain *list;
		unsigned int hashValue = hashSNAP(ht, name);
		for((list = ht->table[hashValue]); list != NULL; list = list->next){
			if(list->dataEntry == NULL){
				return NULL;
			}else if(strcmp(name, list->dataEntry->name)==0){
				snap = list->dataEntry;
			}
		}
	}
	else{
	}
	return snap;
}