#ifndef DATA_TABLE_H
#define DATA_TABLE_H

typedef struct data_node {
    int val;
    struct data_node *next;
} data;

/**
 * Function:
 * update_data_table adds new nodes to the data table, using the
 * create_data_node function. each node holds a single number or
 * char (in ASCII code), depends on the directive type (.data or .string).
 *
 * Parameter data *, a pointer to the data table head.
 * Parameter char *, the directive data or string to be added.
 * Parameter int, the directive type: STRING or DATA (these were
 *                defined as enum directive_type in directive.h file).
 * Returns nothing. 
 */
void update_data_table(data *, char *, int);

/**
 * Function:
 * create_data_node allocates a memory for a new data
 * and places the new node at the end of the data list.
 *
 * Parameter data **, a pointer address to the head of the data list.
 * Returns nothing.
 */
void create_data_node(data **);

/**
 * Function:
 * free_data_table frees all allocated memory used for the list.
 *
 * Parameter data **, a pointer address to the head of the data list.
 * Returns nothing.
 */
void free_data_table(data **);

/**
 * Function:
 * print_data_table prints all data in the list.
 * used for debug only.
 *
 * Parameter data *, a pointer to the head of the data list.
 * Returns nothing.
 */
void print_data_table(data *);

#endif
