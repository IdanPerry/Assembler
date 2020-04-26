#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "instruction.h"
#include "code_table.h"
#include "register.h"

/**
 * File:
 * instruction.c handles instruction statements syntax related issues.
 */

/* accumulated number of opcodes in groups */
#define GROUP1 5
#define GROUP2 14

char *instructions[OPP_CODE_NUM] = {"mov", "cmp", "add", "sub",
                                    "lea", "clr", "not", "inc", "dec", "jmp",
                                    "bne", "red", "prn", "jsr", "rts", "stop"};

/* 
 * Private function:
 * read_operands analyzes and validates the instruction operands.
 *
 * Parameter char *, the operands to be analyzed.
 * Parameter operands_group, indicating how many operands should be expected.
 * Returns char *, the validated operand if no errors were found, otherwise
 *                 returns NULL. 
 */
static char *read_operands(char *, operands_group);

/* 
 * Private function:
 * validate_operand is a helper function for read_operands function.
 * it analyzes and validates a single operand.
 * 
 * Parameter char *, the operand to be analyzed.
 * Returns char *, the operands seperated with a comma (if its two operands)
 *                 if no errors were found. otherwise returns NULL.
 */
static char *validate_operand(char *);

/* 
 * Private function:
 * validate_comma is a helper function for read_operands function.
 * it handles comma related errors.
 * 
 * Parameter char *, the operands to be analyzed.
 * Returns char *, the operands seperated with a comma (if its two operands)
 *                 and trimed out of white space, if no errors were found.
 *                 otherwise returns NULL.
 */
static char *validate_comma(char *);

char *analyze_instruction(char *token, inst_attribute attribute) {
    static operation_code code;
    static operands_group group;
    char *answer;     /* could be a code name or operands */

    switch (attribute) {
    /* validate code name */
    case OP_CODE:
        for(code = 0; code < OPP_CODE_NUM; code++) {
            if(!strcmp(token, instructions[code])) {
                /* tag the code with the corresponding group */
                if(code < GROUP1)
                    group = TWO_OPS;

                else if(code >= GROUP1 && code < GROUP2)
                    group = ONE_OP;

                else
                    group = NO_OPS;

                break;
            }
        }

        /* reached end of loop - none of the codes matches the token */
        if(code == OPP_CODE_NUM) {
            code = -1;  /* error value */
            MESSAGE_1("is unknown instruction statement");
            return NULL;
        }

        answer = instructions[code];
        break;

    /* analyze code operands */
    case OPERANDS:
        answer = read_operands(token, group);

        break;
    } /* end switch */

    return answer;
}

char *read_operands(char *operands, operands_group group) {
    char *delims = " \t,-#*";
    char *op_arr[3] = {NULL, NULL, NULL};
    char *op_copy = NULL;
    int i;
    int j;

    if(operands) {
        /* check for invalid characters */
        for(i = 0; operands[i] != '\0'; i++) {
            if(isdigit(operands[i]) || isalpha(operands[i]))
                continue;

            for(j = 0; delims[j] != '\0'; j++) {
                if(operands[i] == delims[j])
                    break;
            }

            /* none of the permitted chars were found */
            if(delims[j] == '\0') {
                MESSAGE_2("Operands include invalid characters");
                return NULL;
            }
        }

        op_copy = (char *)calloc(strlen(operands) + 1, sizeof(char));

        if(!op_copy) {
        	printf("ERROR: memory allocation failed.\n");
        	exit(1);
        }

        strcpy(op_copy, operands);

		/* break down operands into array. the 3rd one should be NULL */
        if((op_arr[0] = strtok(op_copy, " \t,"))) {
            if((op_arr[1] = strtok(NULL, " \t,")))
                op_arr[2] = strtok(NULL, " \t");
        }
    }

    switch (group) {
    case TWO_OPS:
        /* first operand */
        if(!op_arr[0]) {
            MESSAGE_2("Missing instruction code operands");
            operands = NULL;

        } else if(!(op_arr[0] = validate_operand(op_arr[0])))
            operands = NULL;

        /* second operand */
        else if(!op_arr[1]) {
            MESSAGE_2("This instruction code requires two operands, one is missing");
            operands = NULL;

        } else if(!(op_arr[1] = validate_operand(op_arr[1])))
            operands = NULL;

        else if(op_arr[2]) {
            MESSAGE_2("Extraneous characters after 2nd operand");
            operands = NULL;
        }

        else if(!(operands = validate_comma(operands)))
            operands = NULL;

        else {
            strcpy(operands, op_arr[1]);  /* destination operand first */
            strcat(operands, ",");
            strcat(operands, op_arr[0]);  /* source operand second */
        }

        break;

    case ONE_OP:
        /* no operand */
        if(!op_arr[0]) {
            MESSAGE_2("Missing instruction code operand");
            operands = NULL;


        } else if(!(op_arr[0] = validate_operand(op_arr[0])))
            operands = NULL;

        else if(op_arr[1]) {
            MESSAGE_2("Extraneous characters after operand");
            operands = NULL;

        } else {
            /* no comma allowed */
            for(i = 0; op_copy[i] != '\0'; i++) {
                if(op_copy[i] == ',') {
                    MESSAGE_2("Invalid input - illegal comma");
                    operands = NULL;
                }
            }
        }

        if(operands)
            strcpy(operands, op_arr[0]);  /* destination operand */

        break;

    case NO_OPS:
        if(op_arr[0])
            MESSAGE_2("Extraneous characters after instruction code");

        operands = NULL;
        break;

    } /* end switch */

    free(op_copy);
    return operands;
}

char *validate_operand(char *operand) {
    /* number sign with no integer */
    if(operand[0] == '#') {
        if(operand[1] != '-' && !isdigit(operand[1])) {
            MESSAGE_2("Missing integer after number sign");
            return NULL;
        }

        if(operand[1] == '-' && !isdigit(operand[2])) {
            MESSAGE_2("Missing integer after minus sign");
            return NULL;
        }

        /* integer with no number sign */
    } else if(operand[0] == '-' || isdigit(operand[0])) {
        MESSAGE_2("Missing number sign before integer");
        return NULL;

        /* asterisk sign with no register */
    } else if(operand[0] == '*') {
        if(!is_register(operand+1)) {
            MESSAGE_2("Missing register after asterisk sign");
            return NULL;
        }
    }

    return operand;
}

char *validate_comma(char *string) {
	int i = 0;
	int j;

	/* check comma as first character */
	while(string[i] == ' ' || string[i] == '\t')	/* skip white space */
		i++;

	if(string[i] == ',') {
		MESSAGE_2("Misplaced comma");
		return NULL;
	}

	/* check invalid comma as last char */
	for(i = 0; string[i] != '\0'; i++); 	/* reach end of input */
	for(i--; (string[i] == ' ' || string[i] == '\t') && i >= 0; i--);
	if(string[i] == ',') {
		MESSAGE_2("Misplaced comma");
		return NULL;
	}

	/* check 2 consecutive commas */
	for(i = 0; string[i] != '\0'; i++) {
		if(string[i] == ',') {
			j = i;

			/* skip white space */
			i++;
			while(string[i] == ' ' || string[i] == '\t')
				i++;

			if(string[i] == ',' && i != j) {
				MESSAGE_2("Illegal consecutive commas");
				return NULL;
			}
		}
	}

	/* check missing commas */
	for(i = 0; string[i] != '\0'; i++) {
		if(string[i] == ' ' || string[i] == '\t') {
			if(i > 0 && string[i-1] != ',' && string[i-1] != ' ' && string[i-1] != '\t') {

				/* skip white space */
				while(string[i] == ' ' || string[i] == '\t')
					i++;

				if(string[i] != ',' && string[i] != '\0' ) {
					MESSAGE_2("Missing comma");
					return NULL;
				}
			}
		}
	}

    /* trim white space */
    for(i = 0, j = 0; string[i] != '\0'; i++) {
        if(string[i] != ' ' && string[i] != '\t')
            string[j++] = string[i];
    }

    string[j] = '\0';

	return string;
}
