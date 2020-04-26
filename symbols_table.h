#ifndef SYMBOLS_TABLE_H
#define SYMBOLS_TABLE_H

#include "misc.h"

typedef struct symbol_node {
	char *label;
	int address;
	statement st;		/* instruction or directive */
	boolean external;
	boolean entry;
	struct symbol_node *next;
} symbol;

/**
 * Function:
 * create_symbol_node allocates a memory for a new symbol
 * and inserts the new node at the end of the symbols list.
 *
 * Parameter symbol **, a pointer address to the head of the symbols list.
 * Returns nothing.
 */
void create_symbol_node(symbol **);

/**
 * Function:
 * free_symbol_table frees all allocated memory used for the list.
 * it also frees the memory allocated for the label strings.
 *
 * Parameter symbol **, a pointer address to the head of the symbols list.
 * Returns nothing.
 */
void free_symbol_table(symbol **);

/**
 * Function:
 * print_symbol_table prints all labels in the list.
 * used for debug only.
 *
 * Parameter symbol *, a pointer to the head of the symbols list.
 * Returns nothing.
 */
void print_symbol_table(symbol *);

/**
 * Function:
 * search_symbol searches the list for a label name provided
 * through a parameter.
 *
 * Parameter symbol *, a pointer to the head of the symbols list.
 * Parameter char *, the label name to be searched.
 * Returns a pointer to the symbol with the label if found.
 * 		   otherwise returns NULL.
 */
symbol *search_symbol(symbol *, char *);

/**
 * Function:
 * update_symbol_table adds a new symbol to the symbol table,
 * using the create_symbol_node function.
 *
 * Parameter symbol *, a pointer to the head of the symbols list.
 * Parameter char *, the label name to be searched.
 * Parameter statement, whether the label related to directive
 *                      or instruction statement.
 * Parameter boolean, whether the label related to external or not.
 * Parameter int, the address of the label, which is the DC or IC.
 * Returns nothing.
 */
void update_symbol_table(symbol *,char *, statement, boolean, int);

/**
 * Function:
 * update_address updates all directive non externals addresses
 * in the symbol table at the end of the first scan and before the second.
 *
 * Parameter symbol *, a pointer to the head of the symbols list.
 * Parameter int, the address to add to the desired symbols.
 * Returns nothing.
 */
void update_address(symbol *, int);

/**
 * Function:
 * get_next_entry searches for the next symbol related to an entry,
 * using a pointer to the last entry that was searched. the first
 * search starts at the head of the list.
 *
 * Parameter symbol *, a pointer to the head of the symbols list.
 * Returns a pointer to the next entry symbol.
 */
symbol *get_next_entry(symbol *);

#endif
