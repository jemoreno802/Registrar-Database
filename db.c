#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csg.h"
#include "cp.h"
#include "snap.h"
#include "cdh.h"
#include "cr.h"
#include "crdh.h"

char* findGrade(char *name, char *course, CSG_hashTable *CSG_table, SNAP_hashTable *SNAP_table){
	printf("What grade did %s get in %s?\n",name, course );
	struct SNAP *snap = lookupSNAP2(" ", name , " ", " ", SNAP_table);
	CSG_chain *list;
	int found = 0;
	unsigned int hashValue = hashCSG(CSG_table, course);
	struct CSG *csg;
	for((list = CSG_table->table[hashValue]); list != NULL; list = list->next){
		if(list->dataEntry == NULL){
			printf("	Student: %s is not enrolled in %s\n",name, course);
		}else if(snap->id == list->dataEntry->id){
			csg = list->dataEntry;
			printf("	grade: %s\n\n", csg->grade);
			found = 1;
			return csg->grade;
		}
	}
	if(found ==0){
		printf("	Student: %s is not enrolled in %s\n\n",name, course);

	}
	return "n/a";
}

void findStudent(char *name, char *hour, char * day, CSG_hashTable *CSG_table, SNAP_hashTable *SNAP_table, CDH_hashTable *CDH_table, CR_hashTable *CR_table){
	printf("Where is %s at %s on %s?\n",name, hour, day);
	struct SNAP *snap = lookupSNAP2(" ", name , " ", " ", SNAP_table);
	CSG_chain *list;
	int i;
	struct CSG *csg;
	int found = 0;
	for(i = CSG_table->size; i>0; i--){
		for((list = CSG_table->table[i]); list != NULL; list = list->next){
			if(list->dataEntry == NULL){
				//printf("	Student: %s is not enrolled in %s\n",name, course);
			}else if(snap->id == list->dataEntry->id){
				csg = list->dataEntry;
				
				struct CDH *cdh = lookupCDH2(csg->course, day, hour,CDH_table);
				if(cdh != NULL){
					struct CR *cr = lookupCR2(csg->course, " ",CR_table);
					printf("	%s is in %s at %s on %s\n\n",name, cr->room,  hour, day);
					found = 1;
				}
			}
		}
	}
	if(found == 0){
		printf("	%s is not enrolled in a course at %s on %s\n", name,hour, day );
	}
}

CSG_hashTable selectCSG(char *course, CSG_hashTable *CSG_table){
	printf("SELECT(Course = CS101)(CSG):\n");
	CSG_hashTable newTable = makeCSG_hashTable(197);
	unsigned int hashValue = hashCSG(CSG_table, course);
	CSG_chain *list;
	for((list = CSG_table->table[hashValue]); list != NULL; list = list->next){
		insertCSG2(&newTable, list->dataEntry);
		printf("(\"%s\", \"%d\", \"%s\")\n",list->dataEntry->course,list->dataEntry->id,list->dataEntry->grade );
	}
	printf("\n");
	return newTable;
}

void projectCSG(CSG_hashTable CSG_table){
	printf("PROJECT(studentID)(SELECT(Course = CS101)(CSG)):\n");
	
	unsigned int hashValue = hashCSG(&CSG_table, "CS101");
	CSG_chain *list;
	for((list = CSG_table.table[hashValue]); list != NULL; list = list->next){
		printf("(\"%d\")\n",list->dataEntry->id);
	}
	printf("\n");
}

CRDH_hashTable join(CDH_hashTable *CDH_table,CR_hashTable *CR_table){
	printf("CR JOIN CDH:\n");
	CRDH_hashTable newTable = makeCRDH_hashTable(197);
	int i;
	CDH_chain *list;
	for(i = CDH_table->size; i>0; i--){
		for((list = CDH_table->table[i]); list != NULL; list = list->next){
			struct CR *cr = lookupCR2(list->dataEntry->course, " " ,CR_table);
			if(cr != NULL){
				insertCRDH(&newTable,makeCRDH(cr->course,cr->room, list->dataEntry->day,list->dataEntry->hour));
				printf("(\"%s\", \"%s\",\"%s\", \"%s\")\n",cr->course, cr->room, list->dataEntry->day, list->dataEntry->hour );
			}
		}
	}
	return newTable;
}

void projectSelectJoin(CRDH_hashTable CRDH_table){
	printf("\nPROJECT(Day, Hour)(SELECT(Room = Turing Aud.)(CR JOIN CDH)):\n");
	int i;
	CRDH_chain *list;
	for(i = CRDH_table.size; i>0; i--){
		for((list = CRDH_table.table[i]); list != NULL; list = list->next){
			if(strcmp(list->dataEntry->room, "Turing Aud.")==0){
				printf("(\"%s\", \"%s\")\n",list->dataEntry->day,list->dataEntry->hour );
			}			
		}
	}
}

int main(int argc, char *argv[]) {
	printf("Press enter to test PART 1");
	char enter = 0;
	while (enter != '\r' && enter != '\n') { 
		enter = getchar(); 
	}
	CSG_hashTable CSG_table = makeCSG_hashTable(197);
	struct CSG *csg1 = makeCSG("CS101", "12345", "A");
	struct CSG *csg2 = makeCSG("CS101", "67890", "B");
	struct CSG *csg3 = makeCSG("EE200", "12345", "C");
	struct CSG *csg4 = makeCSG("EE200", "22222", "B+");
	struct CSG *csg5 = makeCSG("CS101", "33333", "A-");
	struct CSG *csg6 = makeCSG("PH100", "67890", "C+");
	
	printf("\nINSERTING ALL CSG TUPLES\n" );
	insertCSG(&CSG_table, csg1);
	insertCSG(&CSG_table, csg2);
	insertCSG(&CSG_table, csg3);
	insertCSG(&CSG_table, csg4);
	insertCSG(&CSG_table, csg5);
	insertCSG(&CSG_table, csg6);

	printf("\nTESTING CSG METHODS\n");
	lookupCSG("EE200", "12345"," ", &CSG_table);
	deleteCSG(&CSG_table, csg3);
	lookupCSG("EE200", "12345","C", &CSG_table);

	//printf("%s\n", lookupCSG("CS101", "12345"," ", &CSG_table)->grade);
	//printf("C: %s S: %d G: %s\n",findCSG(&CSG_table, "CS101")->course, findCSG(&CSG_table, "CS101")->id, findCSG(&CSG_table, "CS101")->grade );
	printf("________________________________________________\n");
	printf("Press enter to continue to Course-Prerequisite");
	enter = 0;
	while (enter != '\r' && enter != '\n') { 
		enter = getchar(); 
	}
	CP_hashTable CP_table = makeCP_hashTable(197);
	struct CP *cp1 = makeCP("CS101", "CS100");
	struct CP *cp2 = makeCP("EE200", "EE005");
	struct CP *cp3 = makeCP("EE200", "CS100");
	struct CP *cp4 = makeCP("CS120", "CS101");
	struct CP *cp5 = makeCP("CS121", "CS120");
	struct CP *cp6 = makeCP("CS205", "CS101");
	struct CP *cp7 = makeCP("CS206", "CS121");
	struct CP *cp8 = makeCP("CS206", "CS205");

	printf("\nINSERTING ALL Course-Prerequisite TUPLES\n" );
	insertCP(&CP_table, cp1);
	insertCP(&CP_table, cp2);
	insertCP(&CP_table, cp3);
	insertCP(&CP_table, cp4);
	insertCP(&CP_table, cp5);
	insertCP(&CP_table, cp6);
	insertCP(&CP_table, cp7);
	insertCP(&CP_table, cp8);

	printf("\nTESTING Course-Prerequisite METHODS\n");
	lookupCP("CS205","CS101", &CP_table);

	deleteCP(&CP_table, cp6);
	lookupCP("CS205","CS101", &CP_table);

	lookupCP("CS205","CS120", &CP_table);
	struct CP *cp9 = makeCP("CS205", "CS120");
	printf(" \n");
	insertCP(&CP_table, cp9);
	lookupCP("CS205","CS120", &CP_table);

printf("________________________________________________\n");
	printf("Press enter to continue to SNAP");
	enter = 0;
	while (enter != '\r' && enter != '\n') { 
		enter = getchar(); 
	}

	SNAP_hashTable SNAP_table = makeSNAP_hashTable(197);
	struct SNAP *snap1 = makeSNAP("12345", "C. Brown", "12 Apple St.", "555-1234");
	struct SNAP *snap2 = makeSNAP("67890", "L. Van Pelt", "34 Pear Ave.", "555-5678");
	struct SNAP *snap3 = makeSNAP("22222", "P. Patty", "56 Grape Blvd.", "555-9999");

	printf("\nINSERTING ALL SNAP TUPLES\n" );
	insertSNAP(&SNAP_table, snap1);
	insertSNAP(&SNAP_table, snap2);
	insertSNAP(&SNAP_table, snap3);

	printf("\nTESTING SNAP METHODS\n");
	lookupSNAP("12345", "C. Brown", "12 Apple St.", "555-1234", &SNAP_table);
	deleteSNAP(&SNAP_table, snap1);
	lookupSNAP("12345", "C. Brown", "12 Apple St.", "555-1234", &SNAP_table);
	lookupSNAP(" ", "L. Van Pelt", " ", " ", &SNAP_table);

printf("________________________________________________\n");
	printf("Press enter to continue to CDH");
	enter = 0;
	while (enter != '\r' && enter != '\n') { 
		enter = getchar(); 
	}

	CDH_hashTable CDH_table = makeCDH_hashTable(197);

	struct CDH *cdh1 = makeCDH("CS101", "M","9AM");
	struct CDH *cdh2 = makeCDH("CS101", "W","9AM");
	struct CDH *cdh3 = makeCDH("CS101", "F","9AM");
	struct CDH *cdh4 = makeCDH("EE200", "Tu","10AM");
	struct CDH *cdh5 = makeCDH("EE200", "W","1PM");
	struct CDH *cdh6 = makeCDH("EE200", "Th","10AM");
	
	printf("\nINSERTING ALL CDH TUPLES\n" );
	insertCDH(&CDH_table, cdh1);
	insertCDH(&CDH_table, cdh2);
	insertCDH(&CDH_table, cdh3);
	insertCDH(&CDH_table, cdh4);
	insertCDH(&CDH_table, cdh5);
	insertCDH(&CDH_table, cdh6);

	printf("\nTESTING CDH METHODS\n");

	lookupCDH( "CS101", "W", "9AM",&CDH_table);
	deleteCDH(&CDH_table, cdh2);
	lookupCDH( "CS101", "W", "9AM",&CDH_table);
	insertCDH2(&CDH_table,cdh2);

printf("________________________________________________\n");
	printf("Press enter to continue to CR");
	enter = 0;
	while (enter != '\r' && enter != '\n') { 
		enter = getchar(); 
	}

	CR_hashTable CR_table = makeCR_hashTable(197);

	struct CR *cr1 = makeCR("CS101","Turing Aud.");
	struct CR *cr2 = makeCR("EE200","25 Ohm Hall");
	struct CR *cr3 = makeCR("PH100","Newton Lab.");
	//findCR(&CR_table, cr1);

	printf("\nINSERTING ALL CR TUPLES\n" );
	insertCR(&CR_table, cr1);
	insertCR(&CR_table, cr2);
	insertCR(&CR_table, cr3);

	lookupCR("PH100"," ", &CR_table);
	deleteCR(&CR_table,cr3);
	lookupCR("PH100","Newton Lab.", &CR_table);

printf("________________________________________________\n");
	printf("Press enter to test PART TWO\n");
	enter = 0;
	while (enter != '\r' && enter != '\n') { 
		enter = getchar(); 
	}
 
	findGrade("L. Van Pelt", "CS101", &CSG_table,&SNAP_table);
	findGrade("P. Patty", "EE200",&CSG_table,&SNAP_table);
	findGrade("P. Patty", "CS101",&CSG_table,&SNAP_table);

	findStudent("L. Van Pelt", "9AM","M",&CSG_table,&SNAP_table,&CDH_table,&CR_table);
	findStudent("P. Patty", "1PM","W",&CSG_table,&SNAP_table,&CDH_table,&CR_table);
	findStudent("L. Van Pelt", "1PM","W",&CSG_table,&SNAP_table,&CDH_table,&CR_table);

	printf("________________________________________________\n");
	printf("Press enter to test PART THREE\n");
	enter = 0;
	while (enter != '\r' && enter != '\n') { 
		enter = getchar(); 
	}
	//selectCSG("CS101",&CSG_table);

	projectCSG(selectCSG("CS101",&CSG_table));
	//join(&CDH_table,&CR_table);
	projectSelectJoin(join(&CDH_table,&CR_table));

}