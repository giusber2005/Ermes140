#ifndef CONSTANTS_H
#define CONSTANTS_H

char* DATABASE_PATH = "database.db";

#define TABLE_CMD_MAX_LENGTH 1024

//strcture for a Map in c, (used in database table creation)
typedef struct {
    char *key;   // column name
    char *value; // column type and constraints
} ColumnDef;

ColumnDef user_columns[] = {
    {"id", "INTEGER PRIMARY KEY AUTOINCREMENT"},
    {"username", "TEXT UNIQUE NOT NULL"},
    {"password", "TEXT NOT NULL"}
};

ColumnDef audios[] = {
    {"id", "INTEGER PRIMARY KEY AUTOINCREMENT"},
    {"filename", "TEXT NOT NULL"},
    {"userid", "INTEGER NOT NULL"},
    {"FOREIGN KEY(userid)", "REFERENCES users(id)"}
};


#define ENTER_KEY 10
#define MAX_USERNAME 50
#define MAX_PASSWORD 50

#endif