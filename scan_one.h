#ifndef SCAN_ONE_H
#define SCAN_ONE_H

#include "data_table.h"
#include "symbols_table.h"
#include "code_table.h"

/**
 * Function:
 * first_scan scans a single statement in an assembly file. it analyzes
 * the statement, searches for errors relating labels, instructions,
 * and directives. it builds the symbol table, data table and code table.
 *
 * Parameter char *, the statement to be analyze.
 * Parameter symbol *, a pointer to the symbol table head.
 * Parameter data *, a pointer to the data table head.
 * Parameter code *, a pointer to the code table head.
 * Parameter int *, a pointer to the data counter (DC).
 * Parameter int *, a pointer to the instruction counter (IC).
 * Returns TRUE if errors were found, otherwise returns FALSE.
 */ 
boolean first_scan(char *, symbol *, data *, code *, int *, int *);

#endif
