#include "../../utils/mainLibraries.h"
#include "../../utils/constants.h"
#include "../management/management.h"

//Initialization of the ncurses library
void ncurses_init() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(1); // Show cursor, set to 0 to hide if preferred
}

//function for displaying the login screen
void login_screen(sqlite3* db) {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    int authenticated = 0;

    while (!authenticated) {
        clear();
        mvprintw(2, 5, "Login to Ermes140");
        mvprintw(4, 5, "Username: ");
        echo();
        getnstr(username, MAX_USERNAME - 1);
        mvprintw(5, 5, "Password: ");
        noecho();
        getnstr(password, MAX_PASSWORD - 1);

        if (user_authentication(db, username, password)) {
            mvprintw(7, 5, "Login successful! Press any key to continue...");
            refresh();
            getch();
            authenticated = 1;
        } else {
            mvprintw(7, 5, "Invalid credentials. Press any key to try again.");
            refresh();
            getch();
        }
    }
}

//function for displaying the registrationg screen
void registration_screen(sqlite3* db) {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];

    clear();
    mvprintw(2, 5, "Register to Ermes140");
    mvprintw(4, 5, "Username: ");
    echo();
    getnstr(username, MAX_USERNAME - 1);
    mvprintw(5, 5, "Password: ");
    noecho();
    getnstr(password, MAX_PASSWORD - 1);

    if (user_registration(db, username, password)) {
        mvprintw(7, 5, "Registration successful! Press any key to continue...");
        refresh();
        getch();
    } else {
        mvprintw(7, 5, "Registration failed. Press any key to try again.");
        refresh();
        getch();
    }
}

//close ncurses window
void ncurses_close() {
    clear();
    mvprintw(2, 5, "Exiting Ermes140. Press any key to close...");
    refresh();
    endwin();
}

// Helper function for a dynamic menu
int menu_select(const char *title, const char *options[], int n_options) {
    int highlight = 0;
    int choice = -1;
    int c;
    while (1) {
        clear();
        mvprintw(2, 5, "%s", title);
        for (int i = 0; i < n_options; i++) {
            if (i == highlight) attron(A_REVERSE);
            mvprintw(4 + i, 7, "%s", options[i]);
            if (i == highlight) attroff(A_REVERSE);
        }
        refresh();
        c = getch();
        switch (c) {
            case KEY_UP:
                highlight = (highlight == 0) ? n_options - 1 : highlight - 1;
                break;
            case KEY_DOWN:
                highlight = (highlight == n_options - 1) ? 0 : highlight + 1;
                break;
            case 10: // Enter
                choice = highlight;
                return choice;
        }
    }
}

// Main interface function with menu
void show_interface(sqlite3* db) {
    const char *options[] = {"Login", "Register", "Close"};
    int n_options = 3;
    int running = 1;
    while (running) {
        int selected = menu_select("Welcome to Ermes140", options, n_options);
        switch (selected) {
            case 0:
                login_screen(db);
                break;
            case 1:
                registration_screen(db);
                break;
            case 2:
            default:
                ncurses_close();
                running = 0;
                break;
        }
    }
}