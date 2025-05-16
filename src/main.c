#include "./utils/constants.h"
#include "./utils/mainLibraries.h"
#include "./utils/utils.h"
#include "./functionalities/database/database.h"
#include "./functionalities/ErmesGraphical/ErmesGraphical.h"

int main(void) {
    //database initialization
    sqlite3 *db;
    if (!database_init(&db)) {
        fprintf(stderr, "Failed to initialize database.\n");
        return 1;
    }

    if (!create_table(&db, "users", sizeof(user_columns) / sizeof(ColumnDef), user_columns)) {
        fprintf(stderr, "Failed to create users table.\n");
        database_close(db);
        return 1;
    }
    if (!create_table(&db, "audios", sizeof(audios) / sizeof(ColumnDef), audios)) {
        fprintf(stderr, "Failed to create audios table.\n");
        database_close(db);
        return 1;
    }

    //ncurses initialization
    ncurses_init();

    show_interface(db);

    //closing protocol of the program
    ncurses_close();
    if (!database_close(db)) {
        fprintf(stderr, "Failed to close database.\n");
        return 1;
    }
    return 0;
}