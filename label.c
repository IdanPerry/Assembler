#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "label.h"
#include "misc.h"

/**
 * File:
 * label.c handles label syntax related issues.
 */

extern char *directives[DIREC_NUM];
extern char *instructions[OPP_CODE_NUM];

char *analyze_label(char *label, symbol *sym_table_head) {
	int i = 0;

	/* exclude ':' */
	if(label[strlen(label)-1] == ':')
        label[strlen(label)-1] = '\0';

	if(strlen(label) > LABLE_MAX_LENGTH) {
		MESSAGE_4("label length is not valid. "
					"labels should be up to 31 characters long");
		return NULL;
	}

	if(!isalpha(label[i++])) {
		MESSAGE_4("first character is not valid. "
					"labels must start with a letter");
		return NULL;
	}

	while(label[i++] != '\0') {
		if(!isalpha(label[i-1]) && !isdigit(label[i-1])) {
			MESSAGE_4("has illegal character. "
						"labels must have letters and digits only");
			return NULL;
		}
	}

	/* check if lable is same as instruction or directive name */
	for(i = 0; i < OPP_CODE_NUM; i++) {
		if(!strcmp(label, instructions[i]) || (i < DIREC_NUM && !strcmp(label, directives[i]))) {
			MESSAGE_4("can't be used as a label");
			return NULL;
		}
	}

	/* search identical label in symbol table */
    if(search_symbol(sym_table_head, label)) {
        MESSAGE_4("is already defined");
        return NULL;
    }

	return label;
}
