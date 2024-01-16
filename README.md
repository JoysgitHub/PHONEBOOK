# PHONEBOOK
Minimalist Phonebook to manage contacts

-This is a console application written in C with a sqlite3 database for persistence.
-Features:
        -Add Contact
        -Search contact(Name or Surname)
        -Print All Contacts
        -Update contacts

Required Headers:
#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

Install sqlite3 lib:
          sudo apt install libsqlite3-dev

Compile:
      - sudo apt install make
      - make
      - ./phoneBook

or
      gcc phoneBook.c -o phoneBook -lsqlite3


(Only tested on linux and arm64)
