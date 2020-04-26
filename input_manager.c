#include <stdio.h>
#include <string.h>
#include "symbols_table.h"
#include "data_table.h"
#include "code_table.h"
#include "misc.h"
#include "input_manager.h"
#include "scan_one.h"
#include "scan_two.h"
#include "code.h"
#include "data.h"
#include "output_manager.h"

/**
 * File:
 * input_manager.c handles and manages the assembly input files.
 */

int file_line = 0;
extern char *file_name;

/*
 * Private function:
 * run_scan calls one of the two scans of the files.
 *
 * Parameter int, the number of scan (one or two).
 * Parameter FILE *, a pointer to the assembly file.
 * Parameter symbol *, a pointer to the symbol table head.
 * Parameter data *, a pointer to the data table head.
 * Parameter code *, a pointer to the code table head.
 * Parameter int *, a pointer to the data counter (DC).
 * Parameter int *, a pointer to the instruction counter (IC).
 * Returns TRUE if errors were found, otherwise returns FALSE.
 */
boolean run_scan(int, FILE *, symbol *, data *, code *, int *, int *);

void get_input(FILE *file_ptr) {
	symbol *symbol_head = NULL;
	data *data_head = NULL;
	code *code_head = NULL;
	int IC = INIT_ADDRESS;
    int DC = 0;

	/* initialize symbol table, data table and code table heads */
	create_symbol_node(&symbol_head);
	create_data_node(&data_head);
	create_code_node(&code_head, TRUE);

	/* 1st scan */
	if(run_scan(SCAN_ONE, file_ptr, symbol_head, data_head, code_head, &DC, &IC)) {
        printf("\nCouldn't create files - %s contain errors.\n", file_name);

		free_symbol_table(&symbol_head);
		free_data_table(&data_head);
		free_code_table(&code_head);

        return;
    }

    rewind(file_ptr);
    file_line = 0,

	/* update symbol table */
    update_address(symbol_head, IC);

    /* 2nd scan */
    if(!run_scan(SCAN_TWO, file_ptr, symbol_head, data_head, code_head, &DC, &IC)) {
		if(IC > INIT_ADDRESS) {
			complete_label_code(code_head, symbol_head);
			complete_data_code(data_head, code_head);
			create_object_file(code_head, DC, IC-INIT_ADDRESS);
			create_entry_file(symbol_head);
			create_extern_file(code_head);
		} else
			printf("\nFile %s contain only comments.\n", file_name);
        
    } else {
		printf("\nCouldn't create files - %s contain errors.\n", file_name);
		return;
	}

	free_symbol_table(&symbol_head);
	free_data_table(&data_head);
	free_code_table(&code_head);
}

boolean run_scan(int scan, FILE *file, symbol *sym, data *data, code *code, int *DC, int *IC) {
	char statement[STATEMENT_MAX_LENGTH];
    boolean error = FALSE;
    int j = 0;

    while((statement[j++] = fgetc(file)) != EOF) {
		if(j == STATEMENT_MAX_LENGTH) {
			file_line++;
			error = TRUE;
			MESSAGE_2("Statement exceeds maximum length");
			j = 0;
			continue;
		}

        if(statement[j-1] == '\n') {
            file_line++;
            statement[j-1] = '\0';

            switch(scan) {
            /* read file content - 1st scan */
            case SCAN_ONE:
                /* once error set to TRUE - it stays in this condition */
                if(first_scan(statement, sym, data, code, DC, IC))
                    error = TRUE;

                break;

            /* read file content - 2nd scan */
            case SCAN_TWO:
                if(second_scan(statement, sym, code))
                    error = TRUE;

                break;
            }

            j = 0;
        }
    }

    return error;
}
