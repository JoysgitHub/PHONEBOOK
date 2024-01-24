# PHONEBOOK
Phonebook to manage contacts
<br>
-This is a console application written in C with a sqlite3 database for persistence.<br>
-Features:<br>
        -Add Contact<br>
        -Search contact(Name or Surname)<br>
        -Print All Contacts<br>
        -Update contacts<br>
        -Delete Contact<br>
<br>
Required Headers:<br>
#include <sqlite3.h><br>
#include <stdio.h><br>
#include <string.h><br>
#include <stdlib.h><br>
#include <stdbool.h><br>
#include <unistd.h><br>
<br>
Install sqlite3 lib:<br>
          sudo apt install libsqlite3-dev<br>

Compile:<br>
      - sudo apt install make<br>
      - make<br>
      - ./phoneBook<br>

or<br>
      gcc phoneBook.c -o phoneBook -lsqlite3<br>

<br>
Tested On:<br>
        Windows<br>
        Debian Linux<br>
        Arm64 Linux<br>
