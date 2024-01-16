#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

/* Minimalist PhoneBook : */
/* Users can Add, Search, Delete and update contacts. */


char* messageErr;
static char *sql;
//Function Prototypes
void createDatabase();
void createContact();
void deleteContact();
void searchContact();
int mainMenu();
void printBook();
int searchMenu();
void updateContact();

//Prints the output of the database.
static int callback(void* data, int argc, char** argv, char** azColName) 
{ 
    int i; 
    for (i = 0; i < argc; i++) { 
        printf("    %s    |",  argv[i] ? argv[i] : "NULL"); 

    } 
  
    printf("\n"); 
    return 0; 
} 

//checks if contacts.db file exists if not then creates it.
//uses switch statments to call the right function.
int main(){
	bool quit = true;

	FILE *f = fopen("contacts.db", "r");
	if(f==NULL){
		printf("[-] Database does not exist. Creating database...\n");
		createDatabase();	
	}else {
	
		fclose(f);
	}
	sleep(2);
	system("clear");
	while (quit) {	
		
		int choice = mainMenu();
		switch (choice) {

			case 1:
				createContact();
				break;
			case 2:
				printBook();
				break;
			case 3:
				searchContact();
				break;

			case 4:
				deleteContact();
				break;
			case 5:
				updateContact();
				break;

			case 6:
				quit = false;
				break;
			default:
				quit = false;
				break;
	}
	}
	return 0; 
}

//if contacts.db does not exist then creates it.
//after creating the database it creates the tables.
void createDatabase(){

	sqlite3* db;	
	int rc = sqlite3_open("contacts.db", &db);

	if(rc != SQLITE_OK){
		fprintf(stderr, "[-] %s", sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(-1);
	}
	sqlite3_close(db);
	sleep(3);

	char* sql = "CREATE TABLE IF NOT EXISTS CONTACT("
		  "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
		  "NAME TEXT NOT NULL,"
		  "SURNAME TEXT NOT NULL,"
		  "SEX TEXT NOT NULL,"
		  "PHONENUMBER INTEGER NOT NULL CHECK (PHONENUMBER > 0));";
	int exit = 0;
	exit = sqlite3_open("contacts.db",&db);
	if (exit == SQLITE_OK) {
	exit = sqlite3_exec(db, sql, NULL, 0, &messageErr );

	if (exit != SQLITE_OK) {
		fprintf(stderr, "[-] %s", sqlite3_errmsg(db));
	}else {
		printf("[+] Tables Created\n");
		sqlite3_close(db);
	}

	
	}
}

//Prints the main menu, validates user input and returns user choice.
int mainMenu(){
	int choice;
	bool inMenu = true;
	while (inMenu) {
	
	printf("	  PHONEBOOK		\n");
	printf("------------------------\n");
	printf("	1. Create Contact\n");
	printf("	2. Print Book\n");
	printf("	3. Search Contact\n");
	printf("	4. Delete Contact\n");
	printf("	5. Update Contact\n");
	printf("	6. Exit\n");
	printf("> ");
	scanf("%d", &choice);
	if (choice > 6 || choice < 1) {
		system("clear");
		continue;	
	}else {
		system("clear");

		return choice;
	}
	

	}
	
}

//gets the user input for the name,surname,sex and Phonenumber and
//adds it to the database.

void createContact(){
	char name[50];
	char surname[50];
	char sex[1];
	char phoneNumber[11];
	char statment[512];
	char* sql;
	bool loop = true;
	int exit;

	printf("New Contact:\n---------------------\nNAME: ");
	scanf("%s", name);
	
	while ((getchar()) != '\n');
	printf("SURNAME: ");
	scanf("%s", surname);

	while ((getchar()) != '\n');
	
	while (loop) {
	
	printf("SEX: ");
	scanf("%s",sex);
	if (sex[0] == 'M' || sex[0] == 'F') {
		loop = false;
	}else {
		continue;
	}
	}

		

	while ((getchar()) != '\n');
	loop = true;
	while (loop) {
	printf("PHONENUMBER: ");
	scanf("%s", phoneNumber);
	if (strlen(phoneNumber) == 11) {
		loop = false;
	}else {
		continue;
	}

	
	}



	snprintf(statment,sizeof(statment),"INSERT INTO CONTACT VALUES(NULL ,'%s', '%s','%c','%s');",  name, surname, sex[0], phoneNumber);
	sql = statment;
	sqlite3* db;
	exit = sqlite3_open("contacts.db", &db); 

	exit = sqlite3_exec(db, sql,NULL,0,&messageErr);
	if (exit!=SQLITE_OK) {
		printf("[-] ERROR: %s\n", messageErr);
		sqlite3_free(messageErr);
		sqlite3_close(db);
	}
	else {
		printf("\n[+] Contact Added Successfully\n");
	}
	sqlite3_close(db); 



		
}
//Dumps the whole database with the help of callback function.
void printBook(){
	sqlite3* db;
	int exit = sqlite3_open("contacts.db", &db); 
    char *query = "SELECT * FROM CONTACT;"; 
  
    printf("[+] Phone Book\n");
	printf("-----------------------------------------------------------------------\n");
	printf("    ID    |    Name    |    Surname    |    Sex    |    Phonenumber    \n");
	printf("-----------------------------------------------------------------------\n");


  
    sqlite3_exec(db, query, callback, NULL, NULL);
	sqlite3_close(db); 
 
	printf("-----------------------------------------------------------------------\n");

}
//Prints the search menu which allows user to choose between name
//and surname to search.
int searchMenu(){
	int choice;
	bool checker = true;
	while (checker) {
		printf("Search Menu:\n");
		printf("------------------\n");
		printf("1. Search By Name\n");
		printf("2. search By Surname\n> ");

		scanf("%d", &choice);
		if (choice == 1 || choice == 2) {
			checker = false;
		}else {
			system("clear");
			continue;
		}

	}
		return choice;
}
//Allows the user to enter choose between name or surname and searches the database.
void searchContact(){
	sqlite3* db;
	char id;
	char searchTerm[50];
    char *query;
	char statment[512];

	int choice = searchMenu();

	switch (choice) {
		case 1:
			printf("[+] Enter Name >");
			scanf("%s", searchTerm);
			
			snprintf(statment,sizeof(statment),"SELECT * FROM CONTACT WHERE NAME LIKE '%%%s%%';",searchTerm );
			sql = statment;

			break;
		case 2:
			printf("[+] Enter Surname>");
			scanf("%s", searchTerm);
			
			snprintf(statment,sizeof(statment),"SELECT * FROM CONTACT WHERE SURNAME LIKE '%%%s%%';",searchTerm );
			sql = statment;


			break;
	}

	
	printf("[+] Search Contact:\n ");

    printf("[+] Phone Book\n");
	printf("-----------------------------------------------------------------------\n");
	printf("    ID    |    Name    |    Surname    |    Sex    |    Phonenumber    \n");
	printf("-----------------------------------------------------------------------\n");


	int exit = sqlite3_open("contacts.db", &db); 
			
    sqlite3_exec(db,sql, callback, NULL, NULL);
	sqlite3_close(db); 
 
	printf("-----------------------------------------------------------------------\n");	
}

//Dumps the database and allows the user to delete a contact based
//on id.
void deleteContact(){
	sqlite3* db;
	int id;
	char statment[512];
	int exit = sqlite3_open("contacts.db", &db); 
    char *query = "SELECT * FROM CONTACT;"; 
	printf("[+]Delete Contact\n"); 
    printf("[+] Phone Book\n");
	printf("-----------------------------------------------------------------------\n");
	printf("    ID    |    Name    |    Surname    |    Sex    |    Phonenumber    \n");
	printf("-----------------------------------------------------------------------\n");


  
    sqlite3_exec(db, query, callback, NULL, NULL);
	sqlite3_close(db);
 
	printf("-----------------------------------------------------------------------\n");
	
	printf("[*] Select ID > ");

	while ((getchar()) != '\n');
	scanf("%d", &id);

	snprintf(statment,sizeof(statment),"DELETE FROM CONTACT WHERE ID = %d;",id );
	sql = statment;
	/* printf("%s", statment); */
	exit = sqlite3_open("contacts.db", &db); 

	exit = sqlite3_exec(db, sql,NULL,0,&messageErr);
	if (exit!=SQLITE_OK) {
		printf("[-] ERROR: %s\n", messageErr);
		sqlite3_free(messageErr);
		sqlite3_close(db);
	}
	else {
		printf("\n[+] Contact  Deleted Successfully\n");
	}
	sqlite3_close(db); 
	
}
//Allows the user to update contact based on id.
void updateContact(){
	char name[50];
	char surname[50];
	char sex[1];
	char phoneNumber[11];
	char statment[512];
	char* sql;
	bool loop = true;
	int exit;



	sqlite3* db;
	int id;
	exit = sqlite3_open("contacts.db", &db); 
    char *query = "SELECT * FROM CONTACT;"; 
	printf("[+] Update Contact\n"); 
    printf("[+] Phone Book\n");
	printf("-----------------------------------------------------------------------\n");
	printf("    ID    |    Name    |    Surname    |    Sex    |    Phonenumber    \n");
	printf("-----------------------------------------------------------------------\n");


  
    sqlite3_exec(db, query, callback, NULL, NULL);
	sqlite3_close(db);
 
	printf("-----------------------------------------------------------------------\n");
	
	printf("[*] Select ID > ");
	scanf("%d", &id);
	while ((getchar()) != '\n');
	printf("\nNAME: ");
	scanf("%s", name);
	
	while ((getchar()) != '\n');
	printf("SURNAME: ");
	scanf("%s", surname);

	while ((getchar()) != '\n');
	
	while (loop) {
	
	printf("SEX: ");
	scanf("%s",sex);
	if (sex[0] == 'M' || sex[0] == 'F') {
		loop = false;
	}else {
		continue;
	}
	}

	while ((getchar()) != '\n');
	loop = true;
	while (loop) {
	printf("PHONENUMBER: ");
	scanf("%s", phoneNumber);
	if (strlen(phoneNumber) == 11) {
		loop = false;
	}else {
		continue;
	}

	
	}



	snprintf(statment,sizeof(statment),"UPDATE CONTACT SET ID=%d,NAME = '%s',SURNAME = '%s',SEX='%c', PHONENUMBER='%s' WHERE ID=%d;",  id,name, surname, sex[0], phoneNumber, id);

	sql = statment;
	/* printf("%s", statment); */
	exit = sqlite3_open("contacts.db", &db); 

	exit = sqlite3_exec(db, sql,NULL,0,&messageErr);
	if (exit!=SQLITE_OK) {
		printf("[-] ERROR: %s\n", messageErr);
		sqlite3_free(messageErr);
		sqlite3_close(db);
	}
	else {
		printf("\n[+] Contact Updated Successfully\n");
	}
	sqlite3_close(db); 


	

	

}



