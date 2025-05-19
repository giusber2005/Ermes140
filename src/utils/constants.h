#ifndef CONSTANTS_H
#define CONSTANTS_H

//GLOBAL CONSTANTS FOR THE PROJECT
#define PROJECT_NAME "Ermes140"
#define VERSION "1.0"
#define AUTHOR "Giuseppe Berardi"
#define DESCRIPTION "Ermes140 is a CLI audio tool for recording, reproducing, compressing, and managing audio files."

#define STRING_CONSTRUCTOR 256

//CONSTANTS FOR THE USERS MANAGEMENT
extern char *current_user; // username of the user logged in

//CONSTANT FOR THE DATABASE
extern char *DATABASE_PATH; // database name

#define TABLE_CMD_MAX_LENGTH 1024
#define USERS_COLUMNS_COUNT 3
#define AUDIOS_COLUMNS_COUNT 4

//strcture for a Map in c, (used in database table creation)
typedef struct {
    char *key;   // column name
    char *value; // column type and constraints
} ColumnDef;

extern ColumnDef user_columns[]; // columns for the users table
extern ColumnDef audios[]; // columns for the audios table


//CONSTANTS FOR THE GRAPHICAL INTERFACE
#define ENTER_KEY 10
#define MAX_USERNAME 50
#define MAX_PASSWORD 50

//CONSTANTS FOR THE GRAPHICAL INTERFACE
extern char *start_menu[]; // start menu options
#define START_MENU_OPTIONS 3
extern char *home_menu[]; // home menu options
#define HOME_MENU_OPTIONS 7

//CONSTANTS FOR THE UTILS AREA
#define COMMON_PASSWORDS_FILE "./public/10k-most-common.txt"
#define MAX_FILES_PER_USER 500
#define FILE_PATH_MAX 1024

//CONSTANT FOR THE AUDIO REPRODUCTION
#define AUDIOS_FOLDER_PATH "./audios/"
#define FRAMES_PER_BUFFER 512 


#endif
