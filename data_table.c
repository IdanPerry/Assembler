#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "data_table.h"
#include "directive.h"

/**
 * File:
 * data_table.c implemets a linked list that stores the
 * directive data and strings (represented as ASCII code).
 */

#define PRINT 4

void create_data_node(data **head) {
    data *temp = (data *)malloc(sizeof(data));
    data *current;

    if(!temp) {
        printf("ERROR: memory allocation failed.\n");
        exit(1);
    }

    temp -> next = NULL;

	/* the list is empty */
    if(!(*head))
        *head = temp;

	/* insert the node at the end of the list */
    else {
        current = *head;
        while((current -> next))
            current = current -> next;

        current -> next = temp;
    }
}

void free_data_table(data **head) {
    data *p;

    while(*head) {
        p = *head;
        *head = p -> next;
        free(p);
    }
}

void update_data_table(data *head, char *string, int dir_type) {
    data *current = head;
    char *token;
    int dec_num;
    int i;

    /* reach last node */
    while(current -> next)
		current = current -> next;

    /* directive .string */
    if(dir_type == STRING) {
        for(i = 0; string[i] != '\0'; i++) {
            dec_num = string[i] + '\0';     /* convert to ASCII code */
            current -> val = dec_num;
            create_data_node(&head);
            current = current -> next;
        }

        current -> val = '\0';
        create_data_node(&head);
        current = current -> next;

    /* directive .data */
    } else {
        token = strtok(string, ",");

        while(token) {
           dec_num = atoi(token);
           token = strtok(NULL, ",");
           current -> val = dec_num;
           create_data_node(&head);
           current = current -> next;
        }
    }
}

void print_data_table(data *head) {
    data *current = head;
    int i = 1;

    puts("DATA TABLE");
    puts("----------");

    while(current -> next) {
        printf("%-6d%c", current -> val, i % PRINT ? ' ':'\n');
        i++;
        current = current -> next;
    }

    putchar('\n');
}
