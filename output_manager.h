#ifndef OUTPUT_MANAGER_H
#define OUTPUT_MANAGER_H

#include "symbols_table.h"

#define OCT_DIGITS 5

/**
 * Function:
 * translate_to_octal translates a binary number to its octal form.
 *
 * Parameter char *, a string representation of the binary number.
 * Returns int, the number in its octal form.
 */
int *translate_to_octal(char *);

/**
 * Function:
 * create_object_file loops through the code table and prints
 * every word (memory block) in the object file to be created.
 * additionaly it prints the data and instruction counters.
 *
 * Parameter code *, a pointer to the code table head.
 * Parameter int, the data counter (DC).
 * Parameter int, the instruction counter (IC).
 * Returns nothing.
 */
void create_object_file(code *, int, int);

/**
 * Function:
 * create_entry_file loops through the symbols table, searches
 * for labels defined as entries and prints its names and
 * addresses to the entry file to be created. if no entry is
 * found, the entry file will not be created.
 *
 * Parameter symbol *, a pointer to the symbols table head.
 * Returns nothing.
 */
void create_entry_file(symbol *);

/**
 * Function:
 * create_extern_file loops through the code table, searches
 * for labels defined as externals and prints its names and
 * addresses to the extern file to be created. if no extern
 * is found, the extern file will not be created.
 *
 * Parameter code *, a pointer to the code table head.
 * Returns nothing.
 */
void create_extern_file(code *);

#endif
