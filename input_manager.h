#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#define STATEMENT_MAX_LENGTH 81

enum {SCAN_ONE = 1, SCAN_TWO = 2};

/**
 * Function:
 * get_input analyzes one assembly file input. in addition,
 * it initializes all necessary data structures to translate
 * the assembly language.
 *
 * Parameter File *, a pointer to the assembly file. 
 * Returns nothing.
 */
void get_input(FILE *);

#endif
