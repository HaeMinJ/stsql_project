//
// Created by 정해민 on 2023/12/04.
//

#include "storage.h"

// Function to create a new table
Table *createTable(TableSchema schema) {
    Table *table = (Table *)malloc(sizeof(Table));
    table->schema.tableName = schema.tableName;
    table->schema = schema;
    table->rows = NULL;
    table->numRows = 0;
    return table;
}

// Function to free memory allocated for a table
void freeTable(Table *table) {
    // Free each row
    for (int i = 0; i < table->numRows; i++) {
        free(table->rows[i].values);
    }
    free(table->rows);
    free(table);
}

// Function to add a row to a table
void addRow(Table *table, Row newRow) {
    table->rows = realloc(table->rows, sizeof(Row) * (table->numRows + 1));
    table->rows[table->numRows] = newRow;
    table->numRows++;
}

// Function to create a row with given values
Row createRow(TableSchema schema, char **values) {
    Row row;
    row.values = malloc(sizeof(void *) * schema.numColumns);
    for (int i = 0; i < schema.numColumns; i++) {
        row.values[i] = strdup(values[i]); // Copy the value
    }
    return row;
}
// Function to save a table to a file
void saveTableToFile(const Table *table, const char *filename) {
    FILE *file = fopen(filename, "wb"); // Open file in binary write mode
    if (!file) {
        perror("Error opening file for writing");
        return;
    }

    // Write the number of rows
    fwrite(&table->numRows, sizeof(int), 1, file);

    // Write table data
    for (int i = 0; i < table->numRows; ++i) {
        for (int j = 0; j < table->schema.numColumns; ++j) {
            // Assuming each value is a null-terminated string
            fwrite(table->rows[i].values[j], strlen(table->rows[i].values[j]) + 1, 1, file);
        }
    }

    fclose(file);
}

// Function to load a table from a file
Table *loadTableFromFile(const char *filename, TableSchema schema) {
    FILE *file = fopen(filename, "rb"); // Open file in binary read mode
    if (!file) {
        perror("Error opening file for reading");
        return NULL;
    }

    Table *table = createTable(schema);

    // Read the number of rows
    fread(&table->numRows, sizeof(int), 1, file);

    // Allocate memory for rows
    table->rows = malloc(sizeof(Row) * table->numRows);

    // Read table data
    for (int i = 0; i < table->numRows; ++i) {
        table->rows[i].values = malloc(sizeof(void *) * schema.numColumns);
        for (int j = 0; j < schema.numColumns; ++j) {
            char buffer[256]; // Buffer for reading strings, adjust size as needed
            fread(buffer, sizeof(buffer), 1, file);
            table->rows[i].values[j] = strdup(buffer); // Duplicate the string
        }
    }

    fclose(file);
    return table;
}

// Global database instance
Database myDatabase = { NULL, 0 };

// Existing functions...

// Function to find a table by name
Table *findTable(const char *tableName) {
    for (int i = 0; i < myDatabase.numTables; i++) {
        if (strcmp(myDatabase.tables[i]->schema.tableName, tableName) == 0) {
            return myDatabase.tables[i];
        }
    }
    return NULL; // Table not found
}

// Function to add a table to the database (not previously defined)
void addTableToDB(Table *table) {
    myDatabase.tables = realloc(myDatabase.tables, sizeof(Table *) * (myDatabase.numTables + 1));
    myDatabase.tables[myDatabase.numTables] = table;
    myDatabase.numTables++;
}
