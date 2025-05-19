#include "ErmesGraphical.h"

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

//Initialization of the ncurses library
void ncurses_init() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0); // Show cursor, set to 0 to hide if preferred
}

// Main interface function with menu
void show_interface(sqlite3* db) {

    bool running = true;
    while (running) {
        int selected = menu_select("Welcome to Ermes140", start_menu, START_MENU_OPTIONS);
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
                running = false;
                break;
        }
    }
}

//function for displaying the login screen
void login_screen(sqlite3* db) {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    
    int authenticated = 0;
    clear();
    while (!authenticated) {
        mvprintw(2, 5, "Please insert your credentials");
        mvprintw(4, 5, "Username: ");
        echo();
        getnstr(username, MAX_USERNAME - 1);
        mvprintw(5, 5, "Password: ");
        noecho();
        getnstr(password, MAX_PASSWORD - 1);

        if (user_authentication(db, username, password)) {
            authenticated = 1;
        } else {
            mvprintw(7, 5, "Invalid credentials. Press 'esc' to return to the home page or press any other key to try again.");
            refresh();

            if (getch() == 27) { // ESC key
                return;
            }

            clear();
        }
    }  
    home_screen();
    refresh();
}

//function for displaying the registrationg screen
void registration_screen(sqlite3* db) {
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];

    clear();
    mvprintw(2, 5, "Choose a username and a password for your account");
    mvprintw(4, 5, "Username: ");
    echo();
    getnstr(username, MAX_USERNAME - 1);
    mvprintw(5, 5, "Password: ");
    noecho();
    getnstr(password, MAX_PASSWORD - 1);

    if (user_registration(db, username, password)) {       
        //creating the directory for the user audio files, the directory will be named with the username
        char *directory_path = malloc(strlen(AUDIOS_FOLDER_PATH) + strlen(username) + 2);
        if (directory_path == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            return;
        }
        strcpy(directory_path, AUDIOS_FOLDER_PATH);
        strcat(directory_path, username);
        strcat(directory_path, "/");
        if (create_directory(directory_path) != 0) {
            fprintf(stderr, "Failed to create directory: %s\n", directory_path);
            free(directory_path);
            return;
        }
        free(directory_path);

        mvprintw(7, 5, "Registration successful! Press any key to continue...");
        refresh();
        getch();
    } else {
        mvprintw(7, 5, "Registration failed. Press any key to try again.");
        refresh();
        getch();
    }
}

void reproduce_screen() {
    mvprintw(2, 5, "Please select an audio file to reproduce."); 
    refresh();
    getch();
}

//function for displaying the home screen
void home_screen() {
    clear();
    mvprintw(7, 5, "Login successful! You are now inside the Ermes140 home interface.");
    int selected = menu_select("Home Menu", home_menu, HOME_MENU_OPTIONS);
    clear();
    switch (selected) {
        case 0:
            mvprintw(2, 5, "Record option selected.");
            break;
        case 1:
            reproduce_screen();
            break;
        case 2:
            mvprintw(2, 5, "Compress option selected.");
            break;
        case 3:
            mvprintw(2, 5, "Delete option selected.");
            break;
        case 4:
            mvprintw(2, 5, "Manage account option selected.");
            break;
        case 5:
            mvprintw(2, 5, "Notes generator option selected.");
            break;
        case 6:
        default:
            ncurses_close();
            return;
    }
    
    refresh();
    getch();
}

//close ncurses window
void ncurses_close() {
    clear();
    mvprintw(2, 5, "Exiting Ermes140. Press any key to close...");
    refresh();
    endwin();
}