#ifndef ERMESGRAPHICAL_H
#define ERMESGRAPHICAL_H

// This file is part of Ermes140 project, it contains the graphical interface functions for the Ermes140 CLI Audio Tool.
#include "./ErmesGraphical.c"

void show_interface(sqlite3* db);
void ncurses_init();
void ncurses_close();

#endif