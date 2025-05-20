//FUNCTIONS FOR MANAGEMENT FUNCTIONS OF THE PROGRAM 
//(for users and audios)

#include "management.h"

//Authentication of the user
bool user_authentication(sqlite3 *db, const char *username, const char *password) {
    char hashed_password[65];
    if (generate_password_hash(password, hashed_password) != 0) {
        fprintf(stderr, "Failed to hash password.\n");
        return false;
    }

    // Prepare the SQL statement
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id FROM users WHERE username = ? AND password = ?";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return false;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, hashed_password, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    bool authenticated = (rc == SQLITE_ROW);

    sqlite3_finalize(stmt);
    return authenticated;
}

//Checking if the user exists in the database
bool user_exists(sqlite3 *db, const char *username) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id FROM users WHERE username = ?";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return false;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    bool exists = (rc == SQLITE_ROW);

    sqlite3_finalize(stmt);
    return exists;
}

//registration of the user in the database
bool user_registration(sqlite3 *db, const char *username, const char *password) {
    if (user_exists(db, username)) {
        fprintf(stderr, "User already exists.\n");
        return false;
    }

    // Check password strength
    int strength = password_strength(password);
    if (strength < 2) {
        fprintf(stderr, "\tPassword is too weak.\n");
        return false;
    }
    if (check_if_common(password)) {
        fprintf(stderr, "\tPassword is too common.\n");
        return false;
    }

    char *hashed_password = malloc(65);
    if (generate_password_hash(password, hashed_password) != 0) {
        fprintf(stderr, "Failed to hash password.\n");
        free(hashed_password);
        return false;
    }

    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO users (username, password) VALUES (?, ?)";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return false;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, hashed_password, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    bool success = (rc == SQLITE_DONE);

    sqlite3_finalize(stmt);
    free(hashed_password);

    return success;
}

//Elimination of the user from the database
bool user_elimination(sqlite3 *db, const char *username) {
    if (!user_exists(db, username)) {
        fprintf(stderr, "User does not exist.\n");
        return false;
    }

    sqlite3_stmt *stmt;
    const char *sql = "DELETE FROM users WHERE username = ?";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return false;
    }

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    bool success = (rc == SQLITE_DONE);

    sqlite3_finalize(stmt);
    return success;
}