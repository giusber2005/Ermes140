#include "./utils/constants.h"
#include "./utils/mainLibraries.h"
#include "./utils/utils.h"
#include "./functionalities/database/database.h"
#include "./functionalities/ErmesGraphical/ErmesGraphical.h"
#include "./functionalities/reproduce/reproduce.h"

int main(void) {
    //portaudio initialization
    if (initialize_portaudio() != 0) {
        fprintf(stderr, "Failed to initialize PortAudio.\n");
        return 1;
    }

    //database initialization
    sqlite3 *db;
    if (!database_init(&db)) {
        fprintf(stderr, "Failed to initialize database.\n");
        return 1;
    }

    if (!create_table(&db, "users", USERS_COLUMNS_COUNT, user_columns)) {
        fprintf(stderr, "Failed to create users table.\n");
        database_close(db);
        return 1;
    }
    if (!create_table(&db, "audios", AUDIOS_COLUMNS_COUNT, audios)) {
        fprintf(stderr, "Failed to create audios table.\n");
        database_close(db);
        return 1;
    }

    //ncurses initialization
    ncurses_init();

    //main part of the program
    show_interface(db);

    //closing protocol of the program
    ncurses_close();
    if (!database_close(db)) {
        fprintf(stderr, "Failed to close database.\n");
        return 1;
    }

    //closing portaudio
    if (close_portaudio() != 0) {
        fprintf(stderr, "Failed to close PortAudio.\n");
        return 1;
    }
    return 0;
}