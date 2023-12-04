//
// Created by 정해민 on 2023/12/04.
//

#include "parser.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Helper function to trim whitespace
char *trimWhitespace(char *str) {
    while(isspace((unsigned char)*str)) str++;
    if(*str == 0) return str;

    char *end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    *(end+1) = 0;

    return str;
}

bool parseCreateTable(const char *sql, CreateTableCommand *command) {
    // Initialize command
    memset(command, 0, sizeof(CreateTableCommand));

    // Start parsing after "CREATE TABLE"
    const char *start = strstr(sql, "CREATE TABLE");
    if (!start) return false;
    start += strlen("CREATE TABLE");

    // Extract table name
    char *end = strchr(start, '(');
    if (!end) return false;

    size_t tableNameLength = end - start;
    char tableName[tableNameLength + 1];
    strncpy(tableName, start, tableNameLength);
    tableName[tableNameLength] = '\0';
    command->tableName = strdup(trimWhitespace(tableName));

    // Process column list
    start = end + 1;
    end = strchr(start, ')');
    if (!end) return false;

    char columns[end - start + 1];
    strncpy(columns, start, end - start);
    columns[end - start] = '\0';

    // Tokenize and store column names
    int capacity = 4; // Initial capacity for column names
    command->columnNames = malloc(sizeof(char *) * capacity);
    command->numColumns = 0;
    command->idColumnIndex = -1;

    char *columnToken = strtok(columns, ",");
    while (columnToken != NULL) {
        // Check if we need to expand the array
        if (command->numColumns >= capacity) {
            capacity *= 2; // Double the capacity
            char **temp = realloc(command->columnNames, sizeof(char *) * capacity);
            if (temp == NULL) {
                // Handle memory allocation failure
                perror("Failed to allocate memory for column names");
                // Cleanup previously allocated memory (not shown here)
                return false; // Or handle it according to your error handling strategy
            }
            command->columnNames = temp;
        }

        command->columnNames[command->numColumns] = strdup(trimWhitespace(columnToken));
        if (command->columnNames[command->numColumns] == NULL) {
            // Handle memory allocation failure for strdup
            perror("Failed to duplicate column name");
            // Cleanup (not shown here)
            return false; // Or handle it according to your error handling strategy
        }

        // Check for ID column
        if (strcmp(command->columnNames[command->numColumns], "ID") == 0) {
            command->idColumnIndex = command->numColumns;
        }
        command->numColumns++;
        columnToken = strtok(NULL, ",");
    }

    return true;
}

bool parseInsertInto(const char *sql, InsertCommand *command) {
    // Initialize command
    memset(command, 0, sizeof(InsertCommand));

    // Start parsing after "INSERT INTO"
    const char *start = strstr(sql, "INSERT INTO");
    if (!start) return false;
    start += strlen("INSERT INTO");

    // Extract table name
    char *end = strchr(start, '(');
    if (!end) return false;

    size_t tableNameLength = end - start;
    char tableName[tableNameLength + 1];
    strncpy(tableName, start, tableNameLength);
    tableName[tableNameLength] = '\0';
    command->tableName = strdup(trimWhitespace(tableName));

    // Process value list
    start = end + 1;
    end = strchr(start, ')');
    if (!end) return false;

    char values[end - start + 1];
    strncpy(values, start, end - start);
    values[end - start] = '\0';

    // Tokenize and store values
    const int maxValues = 10; // Arbitrary limit
    command->values = malloc(sizeof(char *) * maxValues);
    command->numValues = 0;

    char *valueToken = strtok(values, ",");
    while (valueToken != NULL && command->numValues < maxValues) {
        command->values[command->numValues++] = strdup(trimWhitespace(valueToken));
        valueToken = strtok(NULL, ",");
    }

    return true;
}
bool parseSelect(const char *sql, SelectCommand *command) {
    // Initialize command
    memset(command, 0, sizeof(SelectCommand));

    // Start parsing after "SELECT * FROM"
    const char *start = strstr(sql, "SELECT * FROM");
    if (!start) return false;
    start += strlen("SELECT * FROM");

    // Extract table name
    char *trimmedStart = trimWhitespace((char *)start);
    command->tableName = strdup(trimmedStart);

    return true;
}

ParsedSQLCommand parseSQL(const char *sql) {
    ParsedSQLCommand command;
    memset(&command, 0, sizeof(ParsedSQLCommand));  // Initialize the command

    if (strncmp(sql, "CREATE TABLE", 12) == 0) {
        if (parseCreateTable(sql, &command.createTableCommand)) {
            command.type = SQL_CREATE_TABLE;
        } else {
            command.type = SQL_UNKNOWN;
        }
    } else if (strncmp(sql, "INSERT INTO", 11) == 0) {
        if (parseInsertInto(sql, &command.insertCommand)) {
            command.type = SQL_INSERT;
        } else {
            command.type = SQL_UNKNOWN;
        }
    } else if (strncmp(sql, "SELECT * FROM", 13) == 0) {
        if (parseSelect(sql, &command.selectCommand)) {
            command.type = SQL_SELECT;
        } else {
            command.type = SQL_UNKNOWN;
        }
    } else {
        command.type = SQL_UNKNOWN;
    }

    return command;
}