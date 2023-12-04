//
// Created by 정해민 on 2023/12/04.
//

#ifndef STSQL_PROJECT_STORAGE_H
#define STSQL_PROJECT_STORAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the basic structure for a column
typedef struct {
    char *name;
    char *type;
} Column;

// Define the structure for a table schema
typedef struct {
    char *tableName;
    int numColumns;
    Column *columns;
} TableSchema;

// Define the structure for a row
typedef struct {
    void **values; // Array of pointers to the actual data
} Row;

// Define the structure for a table
typedef struct {
    TableSchema schema;
    Row *rows;
    int numRows;
} Table;

typedef struct {
    Table **tables;      // Array of pointers to tables
    int numTables;       // Number of tables in the database
} Database;


// Function prototypes
Table *createTable(TableSchema schema);
void freeTable(Table *table);
void addRow(Table *table, Row newRow);
Row createRow(TableSchema schema, char **values);

void saveTableToFile(const Table *table, const char *filename);
Table *loadTableFromFile(const char *filename, TableSchema schema);

// New function prototype for finding a table
Table *findTable(const char *tableName);
void addTableToDB(Table *table);

#endif //STSQL_PROJECT_STORAGE_H
