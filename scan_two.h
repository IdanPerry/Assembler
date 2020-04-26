#ifndef SCAN_TWO_H
#define SCAN_TWO_H

#include "data_table.h"
#include "symbols_table.h"
#include "code_table.h"

/**
 * Function:
 * second_scan scans a single statement in an assembly file. it skips most
 * statements and analyzes only .entry and .extern directives.
 *
 * Parameter char *, the statement to be analyze.
 * Parameter symbol *, a pointer to the symbol table head.
 * Parameter code *, a pointer to the code table head.
 * Returns TRUE if errors were found, otherwise returns FALSE.
 */ 
boolean second_scan(char *, symbol *, code *);

#endif
