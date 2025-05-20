#include "database.h"

//function for opening the database
bool database_init(sqlite3 **db) {
    int rc = sqlite3_open(DATABASE_PATH, db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(*db));
        return false;
    }
    return true;
}


// Create table with dynamic columns using a map-like structure
bool create_table(sqlite3 **db, const char *table_name, int num_columns, ColumnDef *columns) {
    char *sql = malloc(TABLE_CMD_MAX_LENGTH);
    if (!sql) return false;

    snprintf(sql, TABLE_CMD_MAX_LENGTH, "CREATE TABLE IF NOT EXISTS %s (", table_name);
    size_t len = strlen(sql);

    for (int i = 0; i < num_columns; ++i) {
        len += snprintf(sql + len, TABLE_CMD_MAX_LENGTH - len, "%s %s", columns[i].key, columns[i].value);
        if (i < num_columns - 1) {
            len += snprintf(sql + len, TABLE_CMD_MAX_LENGTH - len, ", ");
        }
    }
    snprintf(sql + len, TABLE_CMD_MAX_LENGTH - len, ");");

    char *err_msg = NULL;
    int rc = sqlite3_exec(*db, sql, 0, 0, &err_msg);
    free(sql);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return false;
    }
    return true;
}

//function for closing the database
bool database_close(sqlite3 *db) {
    int rc = sqlite3_close(db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot close database: %s\n", sqlite3_errmsg(db));
        return false;
    }
    return true;
}