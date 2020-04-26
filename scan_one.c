#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "misc.h"
#include "label.h"
#include "directive.h"
#include "instruction.h"
#include "data.h"
#include "code.h"
#include "register.h"
#include "scan_one.h"

/**
 * File:
 * scan_one.c handles most assembly statements, except the .entry directives.
 * asymbol table, data table and code table are built here.
 */

boolean first_scan(char *input, symbol *symbol_head, data *data_head, code *code_head, int *DC, int *IC) {
    char *token;
    char *label = NULL;
    char *data = NULL;          /* directive data post validation */
    char *dir_type = NULL;      /* directive name (.data, .extern etc.) */
    char *code = NULL;          /* instruction code name */
    char *operands;             /* instruction code operands */
    int address;
    boolean is_label = FALSE;
    boolean external = FALSE;
    statement st;

    /* read first token and ignore empty statement */
    if(!(token = strtok(input, " \t")))
        return FALSE;

    /* ignore comment */
    if(token[0] == ';')
        return FALSE;

    /* check if first token is label */
    if(token[strlen(token) - 1] == ':') {
        is_label = TRUE;

        if(!(label = analyze_label(token, symbol_head)))
            return TRUE;     /* invalid label name or label is already defined */

        /* next token should be a directive or instruction statement */
        if(!(token = strtok(NULL, " \t"))) {
            MESSAGE_2("Missing directive or instruction");
            return TRUE;
        }
    }

    /* check if token is a directive type */
    if(token[0] == '.') {
        st = DIRECTIVE; /* tag for further data validation */

        if(!(dir_type = analyze_directive(token + 1, TYPE)))
            return TRUE;

        /* .entry is being handled only in 2nd scan */
        else if(!strcmp(dir_type, "entry"))
            return FALSE;

    /* check if token is an instruction type */
    } else {
        st = INSTRUCTION;  /* tag for further operands validation */

        if(!(code = analyze_instruction(token, OP_CODE)))
            return TRUE;
    }

    /* next token should be a directive data or instruction operands */
    token = strtok(NULL, "");

    /* analyze data/operands */
    switch(st) {
    case DIRECTIVE:
        if(!token) {
            MESSAGE_2("Missing directive data");
            return TRUE;
        }

        if(!(data = analyze_directive(token, DAT)))
            return TRUE;    /* invalid data structure */

        if(dir_type && !strcmp(dir_type, "extern")) {
            external = TRUE;
            address = 0;

            if(!strcmp(data, "read_label")) {
                is_label = TRUE;

                if(!(label = analyze_label(token, symbol_head)))
                    return TRUE; /* Invalid label name or label has been already defined */

                data = token;
            }

        } else if(!strcmp(dir_type, "string")) {
            address = *DC;
            update_DC(DC, data, STRING);
            update_data_table(data_head, data, STRING);
			free(data);

        } else {
			address = *DC;
            update_DC(DC, data, DATA);
            update_data_table(data_head, data, DATA);
		}

        /* insert label to symbol table */
        if(is_label)
            update_symbol_table(symbol_head, label, DIRECTIVE, external, address);

        break;

    case INSTRUCTION:
        if(!(operands = analyze_instruction(token, OPERANDS))) {
            if(strcmp(code, "rts") && strcmp(code, "stop"))
                return TRUE;
        }

        address = *IC;
        *IC = calc_memory_word(IC, code_head, code, operands, symbol_head);

        if(is_label)
            update_symbol_table(symbol_head, label, INSTRUCTION, external, address);

        break;
    }

    return FALSE;
}
