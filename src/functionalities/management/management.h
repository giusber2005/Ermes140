#ifndef MANAGEMENT_H
#define MANAGEMENT_H

#include "management.c"

bool user_authentication(sqlite3 *db, const char *username, const char *password);
bool user_registration(sqlite3 *db, const char *username, const char *password);
bool user_exists(sqlite3 *db, const char *username);
bool user_elimination(sqlite3 *db, const char *username);

#endif