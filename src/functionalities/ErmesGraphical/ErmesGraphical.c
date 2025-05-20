#include "ErmesGraphical.h"

// Helper function for a dynamic menu
int menu_select(const char *title, char *options[], int n_options) {
    int highlight = 0;
    int choice = -1;
    int c;
    while (1) {
        clear();
        mvprintw(2, 5, "%s", title);
        for (int i = 0; i < n_options; i++) {
            if (i == highlight) attron(A_REVERSE);
            mvprintw(4 + i, 7, "%s %s", "->", options[i]);
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
            current_user = malloc(strlen(username) + 1);
            if (current_user == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                return;
            }
            strcpy(current_user, username);

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
    //user folder path
    char *user_folder_path = malloc(strlen(AUDIOS_FOLDER_PATH) + strlen(current_user) + 2);
    if (user_folder_path == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    strcpy(user_folder_path, AUDIOS_FOLDER_PATH);
    strcat(user_folder_path, current_user);
    strcat(user_folder_path, "/");

    //ERROR HERE    
    char **options = NULL;
    int n_options = files_in_directory_list(user_folder_path, &options);
    if (n_options == -1) {
        mvprintw(2, 5, "Error reading audio files. Press any key to continue.");
        refresh();
        getch();
        return;
    }
    
    int selected = menu_select("Select an audio file to reproduce", options, n_options);
    if (selected == -1) {
        mvprintw(2, 5, "Error selecting audio file. Press any key to continue.");
        refresh();
        getch();
        return;
    }

    // Reproduce the selected audio file
    char *audio_path = malloc(strlen(AUDIOS_FOLDER_PATH) + strlen(options[selected]) + 1);
    if (audio_path == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    strcpy(audio_path, AUDIOS_FOLDER_PATH);
    strcat(audio_path, options[selected]);
    if (reproduce_audio(audio_path)) {
        mvprintw(2, 5, "Audio reproduction successful! Press any key to continue...");
    } else {
        mvprintw(2, 5, "Audio reproduction failed. Press any key to continue...");
    }
    free(audio_path);
    for (int i = 0; i < n_options; i++) {
        free(options[i]);
    }
    // Wait for user input before returning to the home screen
    mvprintw(4, 5, "Press any key to return to the home screen...");

    refresh();
    getch();
}

//function for displaying the home screen
void home_screen() {
    clear();

    //create a string with sprintf
    char welcome_message[STRING_CONSTRUCTOR];
    sprintf(welcome_message, "Hi %s, you are now inside the Ermes140 home interface!", current_user);
    int selected = menu_select(welcome_message, home_menu, HOME_MENU_OPTIONS);
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