//
// Created by 정해민 on 2023/12/04.
//

#ifndef STSQL_PROJECT_PARSER_H
#define STSQL_PROJECT_PARSER_H

#include "../storage/storage.h"
#include <stdbool.h>

typedef enum {
    SQL_UNKNOWN,
    SQL_CREATE_TABLE,
    SQL_INSERT,
    SQL_SELECT
} SQLCommandType;

typedef struct {
    char *tableName;
    char **columnNames;  // Array of column names
    int numColumns;
    int idColumnIndex;   // Index of the ID column, -1 if not present
} CreateTableCommand;

typedef struct {
    char *tableName;
    char **values;       // Array of values as strings
    int numValues;
} InsertCommand;

typedef struct {
    char *tableName;
} SelectCommand;

typedef struct {
    SQLCommandType type;
    union {
        CreateTableCommand createTableCommand;
        InsertCommand insertCommand;
        SelectCommand selectCommand;
    };
} ParsedSQLCommand;

ParsedSQLCommand parseSQL(const char *sql);
// Function prototype for parsing CREATE TABLE
bool parseCreateTable(const char *sql, CreateTableCommand *command);

// Function prototype for parsing INSERT INTO
bool parseInsertInto(const char *sql, InsertCommand *command);

bool parseSelect(const char *sql, SelectCommand *command);

#endif //STSQL_PROJECT_PARSER_H
