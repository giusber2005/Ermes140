#ifndef DATABASE_H
#define DATABASE_H

bool database_init(sqlite3 **db);
bool create_table(sqlite3 **db, const char *table_name, int num_columns, ColumnDef *columns);
bool database_close(sqlite3 *db);

#endif