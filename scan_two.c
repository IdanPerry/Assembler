#include <stdio.h>
#include <string.h>
#include "misc.h"
#include "scan_two.h"

/**
 * File:
 * scan_two.c handles .entry and .extern directive statements.
 */

boolean second_scan(char *input, symbol *symbol_head, code *code_head) {
    char *token;
    symbol *label;
    code *code_ptr;

    /* read first token and ignore empty statement */
    if(!(token = strtok(input, " \t")))
        return FALSE;

    /* ignore comment */
    if(token[0] == ';')
        return FALSE;

    /* skip label */
    if(token[strlen(token)-1] == ':')
        token = strtok(NULL, " \t");

    if(!strcmp(token, ".entry")) {
        /* next token should be a label */
        if(!(token = strtok(NULL, " \t"))) {
			MESSAGE_2("Missing entry data");
            return TRUE;
		}		

        /* the label has been defined in the symbols table */
        if((label = search_symbol(symbol_head, token)))
            label -> entry = TRUE;

        else {
            MESSAGE_1("is not a defined label in this file");
            return TRUE;
        }

    } else if(!strcmp(token, ".extern")) {
        /* next token is a verified label by scan_one */
        token = strtok(NULL, " \t");

        /* search externals and update the word in the code table */
        code_ptr = search_code(code_head);
        while(code_ptr) {
            if(!strcmp(code_ptr -> label, token))
                update_extern(code_ptr);

            code_ptr = search_code(code_ptr -> next);
        }
    }

    return FALSE;
}
