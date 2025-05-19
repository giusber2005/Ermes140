// This file is part of Ermes140 project, it contains the graphical interface functions for the Ermes140 CLI Audio Tool.

#ifndef ERMESGRAPHICAL_H
#define ERMESGRAPHICAL_H

#include "../../utils/mainLibraries.h"
#include "../../utils/constants.h"
#include "../management/management.h"
#include "../reproduce/reproduce.h"

//functions to be used in other files
void show_interface(sqlite3* db);
void ncurses_init();
void ncurses_close();

//functions to be used only in ErmesGraphical.c
void home_screen();
void login_screen(sqlite3* db);
void registration_screen(sqlite3* db);
void reproduce_screen();
int menu_select(const char* title, char* options[], int n_options);

#endif