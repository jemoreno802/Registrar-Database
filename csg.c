#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csg.h"

/*
Jacquelyn Moreno
This file contains all the methods for the Course-Student-Grade relation
*/

CSG_hashTable makeCSG_hashTable(int size){
	CSG_hashTable *newTable;
	
	if(size<1){ //cannot make a table of this size
		newTable = FAILED;
	}if((newTable = malloc(sizeof(CSG_hashTable)))== NULL){
		newTable = FAILED;
	}if((newTable->table = malloc(sizeof(CSG_chain *) * size)) == NULL){
		newTable = FAILED;
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
int hashCSG(CSG_hashTable *ht, char val[6]){ //hash on courses
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
	make a CSG tuple from character array inputs
*/
struct CSG* makeCSG(char *course, char *id, char *grade){
	struct CSG *csg = (struct CSG *)malloc(sizeof(struct CSG));
	int studentId = atoi(id);
	strcpy((csg->course), course);
	csg->id = studentId;
	strcpy((csg->grade), grade );
	return csg;
}

/*
	find a given CSG in a given hashtable
*/

struct CSG* findCSG(CSG_hashTable *ht, struct CSG *csg){
	CSG_chain *list;
	int found = 0;
	unsigned int hashValue = hashCSG(ht, csg->course);
	for((list = ht->table[hashValue]); list != NULL; list = list->next){
		if(list->dataEntry ==NULL){
			found = 0;
			//do nothing
		}else if((strcmp(csg->course, list->dataEntry->course) == 0)&& (csg->id == list->dataEntry->id) && (strcmp(csg->grade,list->dataEntry->grade)==0)){
			found = 1; //found the given CSG
		}
	}if(found == 0){
		csg = FAILED;
	}
	return csg;
}

/*
	insert a given CSG into a given hashtable
*/
void insertCSG(CSG_hashTable *ht, struct CSG *csg){
	printf("insert((\"%s\" , \"%d\",\"%s\"), CSG)\n", csg->course, csg->id, csg->grade);
	CSG_chain *head = malloc(sizeof(CSG_chain));
	struct CSG *test = findCSG(ht, csg);
	unsigned int hashValue = hashCSG(ht, csg->course); //hash the coursevalue
	if(test == NULL){ // if lookup returned nothing, insert into linked list
		head->dataEntry = csg;
		head->next = ht->table[hashValue];
		ht->table[hashValue] = head;
	}else{
		printf("ALREADY IN BUCKET\n");
	}
}

void insertCSG2(CSG_hashTable *ht, struct CSG *csg){
	CSG_chain *head = malloc(sizeof(CSG_chain));
	struct CSG *test = findCSG(ht, csg);
	unsigned int hashValue = hashCSG(ht, csg->course); //hash the coursevalue
	if(test == NULL){ // if lookup returned nothing, insert into linked list
		head->dataEntry = csg;
		head->next = ht->table[hashValue];
		ht->table[hashValue] = head;
	}else{
		printf("ALREADY IN BUCKET\n");
	}
}

/*
	compare two CSG tuples, if they are equal return 1
*/
int compare(struct CSG *listCSG, struct CSG *targetCSG){
	if((strcmp(targetCSG->course, listCSG->course) == 0) && (targetCSG->id == listCSG->id)&& (strcmp(targetCSG->grade,listCSG->grade)==0)){
		return 1;
	}else{
		return 0;
	}
}

/*
	delete a given CSG from a given hashtable
*/
void deleteCSG(CSG_hashTable *ht, struct CSG *csg){
	printf("\ndelete((\"%s\" , \"%d\",\"%s\"), CSG)\n", csg->course, csg->id, csg->grade);
	struct CSG *test = findCSG(ht, csg);
	unsigned int hashValue = hashCSG(ht, csg->course); //hash the coursevalue
	if(test == NULL){
		printf("NOTHING TO DELETE\n");
	}else{
		CSG_chain *list;
		int i =0;
		for((list = ht->table[hashValue]); list != NULL; list = list->next){
			if(i == 0){ //deleting head of list
				if(compare(list->dataEntry, csg)==1){
					printf("	FOUND AND DELETED\n");					
					if(list->next == NULL){  
						list->dataEntry = NULL;
						list->next= NULL;
					}else{//if next is not null, set entire list to start at list->next
						ht->table[hashValue] = list->next; 
					}
				}//deleting second element in list
				if(compare((list->next)->dataEntry, csg)==1){
					printf("	FOUND AND DELETED\n");	
					list->next = (list->next)->next;
				}
			//deleting a middle or end value in list
			}else if(list->next !=NULL){ 
				if(compare((list->next)->dataEntry, csg)==1){
					printf("	FOUND AND DELETED\n");	
					list->next = (list->next)->next;
				}
			}
			i++;
		}
	}
}

/*
	lookup a CSG (given by individual elements) in a given hashtable
*/
struct CSG* lookupCSG(char *course, char *id, char *grade, CSG_hashTable *ht){
	printf("\nlookup((\"%s\" , \"%s\",\"%s\"), CSG)\n",course, id, grade);

	struct CSG *csg = NULL;
	CSG_chain *list;
	int found = 0;
	char courseVal[6];
	strcpy(courseVal, course);
	unsigned int hashValue = hashCSG(ht, courseVal);
	int studentId = atoi(id);
	if(strcmp(grade, " ") == 0){ //if grade not specified
		for((list = ht->table[hashValue]); list != NULL; list = list->next){
			if(list->dataEntry == NULL){
				printf("	NOT FOUND\n");
				return NULL;
			}else if((strcmp(courseVal, list->dataEntry->course) == 0) && (studentId == list->dataEntry->id)){
				found = 1; //found the given CSG
				csg = list->dataEntry;
				printf("	FOUND, grade = %s\n", csg->grade);
			}
		}
	}else{
		csg = makeCSG(course, id, grade);
		if(findCSG(ht, csg) == NULL){
			printf("	NOT FOUND\n");
		}else{
			printf("	FOUND\n");
		}
		return findCSG(ht, csg);
	}
	if(found == 0){
		printf("	NOT FOUND\n");
	}
	return csg;
}

