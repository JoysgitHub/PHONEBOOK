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

struct User{

	char name[50];
	char surname[50];
	char sex[2];
	char phoneNumber[11];
};

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
	#ifdef _WIN32
        // Windows
        system("cls");
    #else
        // Unix-like systems (Linux, macOS, etc.)
        system("clear");
    #endif
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
		/* system("cls"); */
	#ifdef _WIN32
     
        system("cls");
   	 #else

        system("clear");
  	  #endif
		continue;	
	}else {
	#ifdef _WIN32
        system("cls");
   	 #else
	 system("clear");
   	 #endif

		return choice;
	}
	

	}
	
}

//gets the user input for the name,surname,sex and Phonenumber and
//adds it to the database.


void createContact(){
	struct User u1;
	char statment[512];
	char* sql;
	bool loop = true;
	int exit;
	sqlite3_stmt *stmt;
	sqlite3* db;
	printf("New Contact:\n---------------------\n ");
	
	 printf("Name: ");
    scanf("%49s", u1.name);

    printf("Surname:");
    scanf(" %49s", u1.surname);



	while (loop) {
	printf("Sex: ");
	scanf("%s", u1.sex);
	if (	strcmp(u1.sex,"M" ) == 0) {
		/* sex[1] = '\0'; */
		loop = false;
	}else if (	strcmp(u1.sex,"F" ) == 0) {
	loop = false;	
	}else {
		continue;
	}
	}
	loop = true;
	while (loop) {
	printf("Phone: ");
	scanf("%s", u1.phoneNumber);
	if (strlen(u1.phoneNumber) == 11) {
		loop = false;
	}else {
		continue;
	}
	}
	exit = sqlite3_open("contacts.db", &db); 
	const char *insertSQL = "INSERT INTO CONTACT VALUES(NULL ,?,?,?,?);";
	int rc = sqlite3_prepare_v2(db, insertSQL, -1, &stmt, 0);
	if (rc != SQLITE_OK) {
		printf("[-] Sql Error Prep: %s\n", sqlite3_errmsg(db));
	}

	sqlite3_bind_text(stmt, 1,u1.name, -1,SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2,u1.surname, -1,SQLITE_STATIC);
	sqlite3_bind_text(stmt, 3,u1.sex, -1,SQLITE_STATIC);
	sqlite3_bind_text(stmt, 4,u1.phoneNumber, -1,SQLITE_STATIC);
	rc = sqlite3_step(stmt);

	printf("[+] Record Added Successfully\n");

	sqlite3_finalize(stmt);
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

	#ifdef _WIN32

        system("cls");
    #else

        system("clear");
    #endif
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
	struct User u1;
	char statment[512];
	char* sql;
	bool loop = true;
	int exit;
	int id;
	char idS[1];
	sqlite3_stmt *stmt;


	sqlite3* db;
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


	printf("Update Contact:\n---------------------\n ");
	
	 printf("Name: ");
    scanf("%49s", u1.name);

    printf("Surname:");
    scanf(" %49s", u1.surname);



	while (loop) {
	printf("Sex: ");
	scanf("%s", u1.sex);
	if (	strcmp(u1.sex,"M" ) == 0) {
		/* sex[1] = '\0'; */
		loop = false;
	}else if (	strcmp(u1.sex,"F" ) == 0) {
	loop = false;	
	}else {
		continue;
	}
	}
	loop = true;
	while (loop) {
	printf("Phone: ");
	scanf("%s", u1.phoneNumber);
	if (strlen(u1.phoneNumber) == 11) {
		loop = false;
	}else {
		continue;
	}
	}


	exit = sqlite3_open("contacts.db", &db); 
	const char *insertSQL = "UPDATE CONTACT SET ID=?,NAME = ?,SURNAME = ?,SEX=?, PHONENUMBER=? WHERE ID=?;";
	int rc = sqlite3_prepare_v2(db, insertSQL, -1, &stmt, 0);
	if (rc != SQLITE_OK) {
		printf("[-] Sql Error Prep: %s\n", sqlite3_errmsg(db));
	}
	
	sprintf(idS, "%d",id);
	sqlite3_bind_text(stmt, 1, idS,-1,SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2,u1.name, -1,SQLITE_STATIC);
	sqlite3_bind_text(stmt, 3,u1.surname, -1,SQLITE_STATIC);
	sqlite3_bind_text(stmt, 4,u1.sex, -1,SQLITE_STATIC);
	sqlite3_bind_text(stmt, 5,u1.phoneNumber, -1,SQLITE_STATIC);

	sqlite3_bind_text(stmt, 6,idS, -1,SQLITE_STATIC);
	rc = sqlite3_step(stmt);

	sqlite3_finalize(stmt);

	printf("\n[+] Contact Updated Successfully\n");
	
	sqlite3_close(db); 

}



