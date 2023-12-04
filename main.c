#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/parser/parser.h"
#include "src/executor/executor.h"

void processSQLCommand(const char *sql) {
    ParsedSQLCommand parsedCommand = parseSQL(sql);

    switch (parsedCommand.type) {
        case SQL_CREATE_TABLE:
            executeCreateTable(&parsedCommand.createTableCommand);
            break;
        case SQL_INSERT:
            executeInsertInto(&parsedCommand.insertCommand);
            break;
        case SQL_SELECT:
            executeSelect(&parsedCommand.selectCommand);
            break;
            // Handle other command types...
        default:
            printf("Unknown or unsupported SQL command.\n");
            break;
    }
}

int main() {
    char sql[1024]; // Buffer to store user input

    printf("Welcome to MyDBMS!\n");
    while (1) {
        printf("mydbms> "); // Prompt for input
        if (fgets(sql, 1024, stdin) == NULL) {
            printf("\nExiting MyDBMS.\n");
            break; // Exit the loop if input is NULL (e.g., Ctrl+D)
        }

        if (strcmp(sql, "exit\n") == 0) {
            printf("Exiting MyDBMS.\n");
            break; // Exit the loop if user types 'exit'
        }

        processSQLCommand(sql);
    }

    return 0;
}
