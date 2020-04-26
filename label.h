#ifndef LABEL_H
#define LABEL_H

#include "symbols_table.h"

#define LABLE_MAX_LENGTH 31

#define MESSAGE_4(str) \
	printf("ERROR at file %s, line %d: %s "\
	str ".\n", file_name, file_line, label)

/**
 * Function:
 * analyze_label searches analyzes and validates a label name.
 *
 * Parameter char *, the label name to be analyzed.
 * Parameter symbol *, a pointer to the symbol table head. the function
 *                     checks the label hasn't been defined yet.
 * Returns char *, the label name if no errors were found, otherwise returns NULL.
 */
char *analyze_label(char *, symbol *);

#endif

