#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "code_table.h"
#include "output_manager.h"

/**
 * File:
 * output_manager.c handles and manages the machine code
 * output files.
 */

#define BIN_DIGITS 3
#define BASE 2

extern char *file_name;

/*
 * Private function:
 * translate_to_decimal translates a binary number to its decimal form.
 *
 * Paramater char *, a string representation of the binary number.
 * Return int, the number in its decimal form.
 */
int translate_to_decimal(char *);

void create_object_file(code *head, int DC, int IC) {
    FILE *file_ptr;
    code *current = head;
    int *octal_num;
    char *obj_name = (char *)calloc(strlen(file_name) + 1, sizeof(char));
    int i;

	/* exclude .as file ending */
    strncpy(obj_name, file_name, strlen(file_name) - 2); 
    strcat(obj_name, "ob");

    file_ptr = fopen(obj_name, "w");
    if(!file_ptr) {
        printf("ERROR: Couldn't create %s object file.", obj_name);
        exit(1);
    }

	fprintf(file_ptr, "%4d %d\n", IC, DC);

    while(current -> next) {
		/* print code address with leading zero */
        fprintf(file_ptr, "0%d ", current -> address);
        octal_num = translate_to_octal(get_word(current -> w));

		/* print machine code in octal form */
        for(i = 0; i < OCT_DIGITS; i++)
            fprintf(file_ptr, "%d", octal_num[i]);

        fprintf(file_ptr, "\n");
        current = current -> next;
    }

    free(obj_name);
	fclose(file_ptr);

	printf("%s object file was successfuly created.\n", file_name);
}

void create_entry_file(symbol *head) {
	FILE *file_ptr;
    symbol *symbol_ptr;
	char *ent_name = (char *)calloc(strlen(file_name) + 2, sizeof(char));
	
	strncpy(ent_name, file_name, strlen(file_name) - 2); 
    strcat(ent_name, "ent");

    /* search for entries in the symbols table.
	   if none was found, the entry file will not be created. */
	if((symbol_ptr = get_next_entry(head))) {
    	file_ptr = fopen(ent_name, "w");

    	if(!file_ptr) {
        	printf("ERROR: Couldn't create %s entrie file.", ent_name);
        	exit(1);
   		 }

		/* print entry details and keep searching for entries */
		while(symbol_ptr) {
       		fprintf(file_ptr, "%s %d\n", symbol_ptr -> label, symbol_ptr -> address);
       		symbol_ptr = get_next_entry(symbol_ptr -> next);
		}

		fclose(file_ptr);
		printf("%s entry file was successfuly created.\n", file_name);
	}

	free(ent_name);
}

void create_extern_file(code *head) {
	FILE *file_ptr;
    code *code_ptr;
	char *ext_name = (char *)calloc(strlen(file_name) + 2, sizeof(char));

	strncpy(ext_name, file_name, strlen(file_name) - 2); 
    strcat(ext_name, "ext");

	/* search for externals in the code table 
	   if none was found, the extern file will not be created */
    if((code_ptr = get_next_extern(head))) {
	    file_ptr = fopen(ext_name, "w");

    	if(!file_ptr) {
        	printf("ERROR: Couldn't create %s extern file.", ext_name);
        	exit(1);
    	}

		/* print extern details and keep searching for externals */
		while(code_ptr) {
       		fprintf(file_ptr, "%s %d\n", code_ptr -> label, code_ptr -> address);
       		code_ptr = get_next_extern(code_ptr -> next);
		}

		fclose(file_ptr);
		printf("%s extern file was successfuly created.\n", file_name);
	}

	free(ext_name);
}

int *translate_to_octal(char *num) {
    char triplets[OCT_DIGITS][BIN_DIGITS + 1];	/* devide the binary to
													5 sections of 3 bits */
    static int octal_num[OCT_DIGITS];
    int i;

    for(i = 0; i < OCT_DIGITS; i++) {
		/* copy 3 bits at a time */
		strncpy(triplets[i], num + i*BIN_DIGITS, BIN_DIGITS);
        octal_num[i] = translate_to_decimal(triplets[i]);
    }

    return octal_num;
}

int translate_to_decimal(char *binary) {
    int i;
    int dec_num = 0;

    for(i = 0; i < BIN_DIGITS; i++) {
        if(binary[i] == '1')
            dec_num += pow(BASE, BASE - i);
    }

    return dec_num;
}
