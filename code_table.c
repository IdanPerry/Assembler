#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "code_table.h"
#include "output_manager.h"

/**
 * File:
 * code_table.c implemets a linked list that stores the
 * machine code translated from the assembly files.
 */

#define FIELDS 6  /* number of fields in first word struct */

/*
 * Private function:
 * get_field returns a bits section of the first word struct.
 *
 * Parameter word, the first word which contain the opcode.
 * Parameter int, the number of field in the first word struct.
 * Returns unsigned int, a bits section of the first word struct.
 */
static unsigned int get_field(word, int);

/*
 * Private function:
 * get_field_size return the number of bits used by
 * a word's struct field.
 *
 * Parameter int, the number of the field in the word's struct.
 * Returns the number of bits used by a word's struct field.
 */
static fw_field_size get_field_size(int);

void create_code_node(code **head, boolean reset_address) {
    code *temp = (code *)calloc(1, sizeof(code));
    code *current;
    static int address;

    if(!temp) {
        printf("ERROR: memory allocation failed.\n");
        exit(1);
    }

	if(reset_address)
		address = INIT_ADDRESS;

    temp -> next = NULL;
    temp -> address = address++;

    if(!(*head))
        *head = temp;

    else {
        current = *head;
        while((current -> next))
            current = current -> next;

        current -> next = temp;
    }
}

void free_code_table(code **head) {
    code *current;

    while(*head) {
        current = *head;
        *head = current -> next;
        free(current -> label);
        free(current);
    }
}

void insert_first_word(code *head, addressing_mode *mode, operation_code oc) {
    code *current = head;

    /* reach last node */
    while (current -> next)
        current = current -> next;

    current -> w.fw.E = 0;
    current -> w.fw.R = 0;
    current -> w.fw.A = 1;
    current -> w.fw.dest = mode[DES];
    current -> w.fw.source = mode[SOUR];
    current -> w.fw.op_code = oc;

    create_code_node(&head, FALSE);
}

void insert_words(code *head, addressing_mode *mode, int *operands, char **lbl) {
    code *current = head;
    char *label[2] = {NULL, NULL};

    if(lbl[0]) {
        if(!(label[0] = (char *)calloc(strlen(lbl[0]) + 1, sizeof(char)))) {
       		printf("ERROR: memory allocation failed.\n");
      	 	exit(1);

        } else
            strcpy(label[0], lbl[0]);
    }

    if(lbl[1]) {
        if(!(label[1] = (char *)calloc(strlen(lbl[1]) + 1, sizeof(char)))) {
        	printf("ERROR: memory allocation failed.\n");
        	exit(1);

        } else
            strcpy(label[1], lbl[1]);
    }

    /* reach last node */
    while (current -> next)
        current = current -> next;

    current -> label = NULL;        /* default */
    current -> external = FALSE;    /* default */

    switch(mode[SOUR]) {
	/* direct and indirect register modes are being treated the same */
    case DIRECT_REG:
    case INDIRECT_REG:
        current -> w.rw.unused = 0;
        current -> w.rw.source_reg = operands[0];
        current -> w.rw.dest_reg = 0;
        current -> w.rw.A = 1;
        current -> w.rw.R = 0;
        current -> w.rw.E = 0;

        /* insert only one word */
        if(mode[DES] == DIRECT_REG || mode[DES] == INDIRECT_REG) {
            current -> w.rw.dest_reg = operands[1];
            create_code_node(&head, FALSE);
            return;
        }

        create_code_node (&head, FALSE);
        current = current -> next;
        break;

	/* promp and direct modes are being treated the same */
    case PROMPT:
    case DIRECT:
        current -> w.nw.num = operands[0];
        current -> w.nw.A = mode[SOUR] == PROMPT ? 1 : 0;
        current -> w.nw.R = mode[SOUR] == DIRECT ? 1 : 0;
        current -> w.nw.E = mode[SOUR] == 0;

        if(mode[SOUR] == DIRECT)
            current -> label = label[SOUR];

        create_code_node(&head, FALSE);
        current = current -> next;
        break;
    }

    switch(mode[DES]) {
    case DIRECT_REG:
    case INDIRECT_REG:
        current -> w.rw.unused = 0;
        current -> w.rw.source_reg = 0;
        current -> w.rw.dest_reg = operands[1];
        current -> w.rw.A = 1;
        current -> w.rw.R = 0;
        current -> w.rw.E = 0;

        create_code_node(&head, FALSE);
        break;

    case PROMPT:
    case DIRECT:
        current -> w.nw.num = operands[1];
        current -> w.nw.A = mode[DES] == PROMPT ? 1 : 0;
        current -> w.nw.R = mode[DES] == DIRECT ? 1 : 0;
        current -> w.nw.E = mode[DES] == 0;

        if(mode[DES] == DIRECT)
            current -> label = label[DES];

        create_code_node (&head, FALSE);
        break;
    }
}

void update_extern(code *code_ptr) {
    code_ptr -> external = TRUE;
    code_ptr -> w.nw.E = 1;
    code_ptr -> w.nw.R = 0;
}

code *get_next_extern(code *head) {
    code *current = head;

    while(current->next) {
        if(current -> external)
            return current;

        current = current -> next;
    }

    return NULL;
}

code *insert_data(code *head, int data) {
    code *current = head;

    /* reach tail */
	while(current -> next)
        current = current -> next;

	/* insert directive data */
    current -> w.dw.num = data;
    create_code_node(&head, FALSE);

    return current -> next;
}

code *search_code(code *head) {
    code *current = head;

	while(current -> next) {
		if(current -> label)
			return current;

		current = current -> next;
	}

	return NULL;
}

void print_code_table(code *head) {
    code *current = head;

    puts("\nCODE TABLE");
    puts("----------");

    while(current -> next) {
        printf("0%d %s\n", current -> address, get_word(current -> w));
        current = current -> next;
    }
}

char *get_word(word w) {
    fw_field_size b_size;  /* size in bits */
    unsigned int mask;
    static char bin_number[BITS + 1];
    int i;
    int j = 0;

	/* loop through the struct fields */
    for(i = FIELDS; i > 0; i--) {
        b_size = get_field_size(i);
        mask = 1 << (b_size - 1);

		/* translate word to binary form */
        while(mask) {
            if((get_field(w, i) & mask) == 0)
                bin_number[j] = '0';
            else
                bin_number[j] = '1';

            mask >>= 1;
            j++;
        }
    }

    bin_number[BITS] = '\0';
    return bin_number;
}

unsigned int get_field(word w, int m) {
    switch(m) {
    case 1:
        return w.fw.E;
    case 2:
        return w.fw.R;
    case 3:
        return w.fw.A;
    case 4:
        return w.fw.dest;
    case 5:
        return w.fw.source;
    case 6:
        return w.fw.op_code;
    default:
        return 0;
    }
}

fw_field_size get_field_size(int k) {
    switch(k) {
    case 1:
        return A;
    case 2:
        return R;
    case 3:
        return E;
    case 4:
        return DEST;
    case 5:
        return SOURCE;
    case 6:
        return CODE;
    default:
        return 0;
    }
}
