#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crdh.h"

/*
Jacquelyn Moreno
This file contains all the methods for the Course-Room-Day-Hour relation
*/

CRDH_hashTable makeCRDH_hashTable(int size){
	CRDH_hashTable *newTable;
	if(size<1){ //cannot make a table of this size
		newTable = NULL;
	}if((newTable = malloc(sizeof(CRDH_hashTable)))== NULL){
		newTable = NULL;
	}if((newTable->table = malloc(sizeof(CRDH_chain *) * size)) == NULL){
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
int hashCRDH(CRDH_hashTable *ht, char val[6]){
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
	make a CRDH tuple from character array inputs
*/
struct CRDH* makeCRDH(char *course, char *room ,char *day, char *hour){
	struct CRDH *crdh = (struct CRDH *)malloc(sizeof(struct CRDH));
	
	strcpy((crdh->course), course);
	strcpy((crdh->room), room);
	strcpy((crdh->day), day);
	strcpy((crdh->hour), hour );
	return crdh;
}

/*
	find a given CRDH in a given hashtable
*/
struct CRDH* findCRDH(CRDH_hashTable *ht, struct CRDH *crdh){
	CRDH_chain *list;
	int found = 0;
	unsigned int hashValue = hashCRDH(ht, crdh->course);
	for((list = ht->table[hashValue]); list != NULL; list = list->next){
		if(list->dataEntry ==NULL){
			found = 0;
			//do nothing
		}else if(compareCRDH(list->dataEntry, crdh)==1){
			found = 1; //found the given CRDH
		}
	}if(found == 0){
		crdh = NULL;
		//printf("NOT FOUND\n");
	}
	return crdh;
}


/*
	insert a given CRDH into a given hashtable
*/
void insertCRDH(CRDH_hashTable *ht, struct CRDH *crdh){
	CRDH_chain *head = malloc(sizeof(CRDH_chain));
	struct CRDH *test = findCRDH(ht, crdh);
	unsigned int hashValue = hashCRDH(ht, crdh->course); //hash the coursevalue
	if(test == NULL){ // if lookup returned nothing, insert into linked list
		head->dataEntry = crdh;
		head->next = ht->table[hashValue];
		ht->table[hashValue] = head;
	}else{
		printf("ALREADY IN BUCKET\n");
	}
}

/*
	compare two CRDH tuple's, if they are equal return 1
*/
int compareCRDH(struct CRDH *listCRDH, struct CRDH *targetCRDH){
	if((strcmp(targetCRDH->course, listCRDH->course) == 0)&&(strcmp(targetCRDH->room, listCRDH->room) == 0) && (strcmp(targetCRDH->day,listCRDH->day)==0)&& (strcmp(targetCRDH->hour,listCRDH->hour)==0)){
		return 1;
	}else{
		return 0;
	}
}

/*
	delete a given CRDH from a given hashtable
*/
void deleteCRDH(CRDH_hashTable *ht, struct CRDH *crdh){
	printf("\ndelete((\"%s\" , \"%s\",\"%s\"), CRDH)\n", crdh->course, crdh->day, crdh->hour);
	struct CRDH *test = findCRDH(ht, crdh);
	unsigned int hashValue = hashCRDH(ht, crdh->course); //hash the coursevalue
	if(test == NULL){
		printf("NOTHING TO DELETE\n");
	}else{
		CRDH_chain *list;
		int i =0;
		for((list = ht->table[hashValue]); list != NULL; list = list->next){
			if(i == 0){ //deleting head of list

				if(compareCRDH(list->dataEntry, crdh)==1){
					printf("	FOUND AND DELETED\n");					
					if(list->next == NULL){  
						list->dataEntry = NULL;
						list->next= NULL;
					}else{//if next is not null, set entire list to start at list->next
						ht->table[hashValue] = list->next; 
					}
				}//deleting second element in list
				if(compareCRDH((list->next)->dataEntry, crdh)==1){
					printf("	FOUND AND DELETED\n");	
					list->next = (list->next)->next;
				}
			//deleting a middle or end value in list
			}else if(list->next !=NULL){ 
				if(compareCRDH((list->next)->dataEntry, crdh)==1){
					printf("	FOUND AND DELETED\n");	
					list->next = (list->next)->next;
				}
			}
			i++;
		}
	}
}

/*
	lookup a CRDH (given by individual elements) in a given hashtable
*/
struct CRDH* lookupCRDH(char *course, char *room ,char *day, char *hour, CRDH_hashTable *ht){
	printf("\nlookup((\"%s\" ,\"%s\", \"%s\"), CRDH)\n",course, day, hour);
	struct CRDH *crdh = makeCRDH(course, room,day,hour);
	if(findCRDH(ht, crdh)==NULL){
		printf("	NOT FOUND\n");
	}else{
		printf("	FOUND, %s meets at %s on %s\n", course, hour, day);
	}

	return findCRDH(ht, crdh);
}
