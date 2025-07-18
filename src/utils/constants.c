#include "constants.h"

//VARIABLES FOR THE USERS MANAGEMENT
char *current_user;

//VARIABLES FOR THE DATABASE

char* DATABASE_PATH = "ErmesDatabase.db";

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

//VARIABALES FOR THE GRAPHICAL INTERFACE

char* start_menu[] = {
    "Login",
    "Register",
    "Close"
};

char* home_menu[] = {
    "Record",
    "Reproduce",
    "Compress",
    "Notes generator",
    "Delete",
    "Manage account",
    "Exit"
};