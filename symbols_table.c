#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "symbols_table.h"

/**
 * File:
 * symbols_table.c implements a linked list that stores the
 * labels and its related memory address.
 */

void create_symbol_node(symbol **head) {
    symbol *temp = (symbol *)calloc(1, sizeof(symbol));
    symbol *current;

    if(!temp) {
        printf("ERROR: memory allocation failed.\n");
        exit(1);
    }

    temp -> next = NULL;

	/* insert first symbol to head */
    if(!(*head))
        *head = temp;

	/* insert the symbol at the end of list */
    else {
        current = *head;

        while((current -> next))
            current = current -> next;

        current -> next = temp;
    }
}

void free_symbol_table(symbol **head) {
    symbol *current;

    while(*head) {
        current = *head;
        *head = current -> next;
        free(current -> label);
        free(current);
    }
}

symbol *search_symbol(symbol *head, char *label) {
    symbol *current = head;

	while(current -> next) {
		if(!strcmp((current -> label), label))
			return current;

		current = current -> next;
	}

	return NULL;
}

symbol *get_next_entry(symbol *head) {
    symbol *current = head;

	while(current -> next) {
        if(current -> entry)
            return current;

        current = current -> next;
	}

	return NULL;
}

void update_symbol_table(symbol *head ,char *label, statement st, boolean ext, int address) {
    symbol *current = head;
    char *new_label = (char *)calloc(strlen(label) + 1, sizeof(char));

    if(!new_label) {
        printf("ERROR: memory allocation failed.\n");
        exit(1);
    }

    strcpy(new_label, label);

	/* reach last symbol */
	while(current -> next)
		current = current -> next;

	current -> label = new_label;
	current -> st = st;
	current -> external = ext;
	current -> entry = FALSE;
    current -> address = address;

	create_symbol_node(&head);
}

void update_address(symbol *head, int address) {
    symbol *current = head;

	/* update all directive non-external addresses */
    while(current -> next) {
        if(current -> st == DIRECTIVE && !current -> external)
            current -> address += address;

        current = current -> next;
    }
}

void print_symbol_table(symbol *head) {
    symbol *current = head;

    puts("SYMBOLS TABLE");
    puts("-------------");

    while(current -> next) {
        printf("%-4s : %-11s : %-12s : %d\n",
        current -> label, current -> st ? "Directive":"Instruction",
        current -> external ? "External":"Non-external",current -> address);

        current = current -> next;
    }

    putchar('\n');
}
