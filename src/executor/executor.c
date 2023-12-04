//
// Created by 정해민 on 2023/12/04.
//

#include "executor.h"
#include "../storage/storage.h"
#include <stdio.h>

// Function to execute CREATE TABLE command
void executeCreateTable(const CreateTableCommand *command) {
    // Check if table already exists
    if (findTable(command->tableName) != NULL) {
        printf("Table %s already exists.\n", command->tableName);
        return;
    }

    // Create a new table in memory
    TableSchema schema = {
            .tableName = command->tableName,
            .numColumns = command->numColumns,
            .columns = malloc(sizeof(Column) * command->numColumns)
    };

    for (int i = 0; i < command->numColumns; i++) {
        schema.columns[i].name = strdup(command->columnNames[i]);
        schema.columns[i].type = (i == command->idColumnIndex) ? "ID" : "VARCHAR";
    }

    Table *newTable = createTable(schema);
    // Add the table to your database's table list (not shown here)
    addTableToDB(newTable);

    printf("Table %s created successfully.\n", command->tableName);
}

// Function to execute INSERT INTO command
void executeInsertInto(const InsertCommand *command) {
    // Find the table
    Table *table = findTable(command->tableName);
    if (table == NULL) {
        printf("Table %s not found.\n", command->tableName);
        return;
    }

    // Create a new row
    if (command->numValues != table->schema.numColumns) {
        printf("Mismatched column count for table %s.\n", command->tableName);
        return;
    }

    Row newRow = createRow(table->schema, command->values);
    addRow(table, newRow);

    printf("Row inserted into table %s.\n", command->tableName);
}

// Function to execute SELECT command
void executeSelect(const SelectCommand *command) {
    // Find the table
    Table *table = findTable(command->tableName);
    if (table == NULL) {
        printf("Table %s not found.\n", command->tableName);
        return;
    }

    // Display all rows
    for (int i = 0; i < table->numRows; i++) {
        for (int j = 0; j < table->schema.numColumns; j++) {
            printf("%s\t", (char *)table->rows[i].values[j]);
        }
        printf("\n");
    }
}