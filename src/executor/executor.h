//
// Created by 정해민 on 2023/12/04.
//

#ifndef STSQL_PROJECT_EXECUTOR_H
#define STSQL_PROJECT_EXECUTOR_H


#include "../parser/parser.h"

// Function prototypes for executing SQL commands
void executeCreateTable(const CreateTableCommand *command);
void executeInsertInto(const InsertCommand *command);
void executeSelect(const SelectCommand *command);

#endif //STSQL_PROJECT_EXECUTOR_H
