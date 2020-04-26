#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "code_table.h"
#include "directive.h"
#include "data.h"

/**
 * File:
 * data.c handles data related inssues.
 */

int update_DC(int *DC, char *data, int dir_type) {
    int i = 0;

    /* .string */
    if(dir_type == STRING)
        *DC = *DC + strlen(data)+1;

    /* .data, each integer is followed by a comma except the 1st integer */
    else {
        (*DC)++;    /* 1st int */

        while(data[i] != '\0') {
            if(data[i++] == ',')
                (*DC)++;
        }
    }

    return *DC;
}

void complete_data_code(data *data_head, code *code_head) {
    data *data_ptr = data_head;

    while(data_ptr -> next) {
        insert_data(code_head, data_ptr -> val);
        data_ptr = data_ptr -> next;
    }
}
