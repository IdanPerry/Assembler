#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "code.h"
#include "instruction.h"
#include "register.h"

/**
 * File:
 * code.c handles code (words/memory blocks) related issues
 * and links between the code table and other parts of the program.
 */

extern char *instructions[OPP_CODE_NUM];

/*
 * Private function:
 * update_IC updates the instruction counter according to the
 * operands addressing mode, which effects the number of words
 * inserted into the code table.
 *
 * Parameter int *, a pointer to the instruction counter.
 * Parameter addressing_mode *, an array of the source and destination
 *                              operands addressing modes.
 */
static int update_IC (int *, addressing_mode *);

int calc_memory_word (int *IC, code *code_head, char *code_str, char *operands_str, symbol *sym_head) {
    addressing_mode mode[MAX_OP] = {-1, -1};
    operation_code op_cpde;
    char *label[MAX_OP] = {NULL, NULL};
    int operands[MAX_OP] = {0, 0};
    char *source_op;
    char *destination_op;

    for(op_cpde = 0; op_cpde < OPP_CODE_NUM; op_cpde++) {
        if(!strcmp(code_str, instructions[op_cpde]))
            break;
    }

    if(!strcmp (code_str, "rts") || !strcmp (code_str, "stop")) {
        mode[DES] = 0;
        mode[SOUR] = 0;

    } else {
        destination_op = strtok(operands_str, ",");
        source_op = strtok(NULL, ",");

   	 	/* DESTINATION OPERAND */
        /* prompt addressing mode */
        if(destination_op[0] == '#') {
            mode[DES] = PROMPT;
            operands[DES] = atoi(destination_op + 1);

            if(strcmp(code_str, "cmp") && strcmp(code_str, "prn"))
                MESSAGE_3("doesn't recognize prompt addressing "
                           "mode for destination operand");

        /* indirect register addressing mode */
        } else if(destination_op[0] == '*') {
            mode[DES] = INDIRECT_REG;
            operands[DES] = get_register(destination_op + 1);

        /* direct register addressing mode */
        } else if((is_register(destination_op))) {
            mode[DES] = DIRECT_REG;
            operands[DES] = get_register(destination_op);

            if(!strcmp(code_str, "jmp") || !strcmp(code_str, "bne") || !strcmp(code_str, "jsr"))
                MESSAGE_3("doesn't recognize direct register "
                           "addressing mode for destination operand");

        /* direct addressing mode: undefined label
            will be determined during 2nd scan */
        } else {
            mode[DES] = DIRECT;
            operands[DES] = 0;     /* temporary assignment */
            label[DES] = destination_op;
        }


    	/* SOURCE OPERAND */
        if(source_op) {
            /* prompt addressing mode */
            if(source_op[0] == '#') {
                mode[SOUR] = PROMPT;
                operands[SOUR] = atoi (source_op + 1);

                if(!strcmp(code_str, "lea"))
                    MESSAGE_3("doesn't recognize prompt "
                               "addressing mode for source operand");

            /* indirect register addressing mode */
            } else if(source_op[0] == '*') {
                mode[SOUR] = INDIRECT_REG;
                operands[SOUR] = get_register(source_op + 1);

                if(!strcmp(code_str, "lea"))
                    MESSAGE_3 ("doesn't recognize indirect register "
                               "addressing mode for source operand");

            /* direct register addressing mode */
            } else if(is_register (source_op)) {
                mode[SOUR] = DIRECT_REG;
                operands[SOUR] = get_register(source_op);

                if(!strcmp(code_str, "lea"))
                    MESSAGE_3("doesn't recognize direct register "
                               "addressing mode for source operand");

            /* direct addressing mode: undefined label
                will be determined during 2nd scan */
            } else {
                mode[SOUR] = DIRECT;
                operands[SOUR] = 0;      /* temporary assignment */
                label[SOUR] = source_op;
            }

        } else
            mode[SOUR] = 0;
    }

    update_IC(IC, mode);
    insert_first_word(code_head, mode, op_cpde);
    insert_words(code_head, mode, operands, label);

    return *IC;
}

void complete_label_code(code *code_head, symbol *sym_head) {
    /* scan code table and return the first label */
    code *code_ptr = search_code(code_head);
    symbol *label;

	/* update the code table and keep searching for labels */
    while(code_ptr -> next) {
        if(code_ptr -> label && (label = search_symbol(sym_head, code_ptr -> label)))
            code_ptr -> w.nw.num = label -> address;

        code_ptr = code_ptr -> next;
    }
}

int update_IC(int *IC, addressing_mode *mode) {
	/* instruction without operands */
    if(!mode[SOUR] && !mode[DES])
        *IC += ONE_WORD;

	/* only destination operand */
    else if(!mode[SOUR] && mode[DES])
        *IC += TWO_WORDS;

	/* both operands are registers */
    else if((mode[SOUR] == INDIRECT_REG || mode[SOUR] == DIRECT_REG)
             && (mode[DES] == INDIRECT_REG || mode[DES] == DIRECT_REG))
        *IC += TWO_WORDS;

	/* two operands which not both of them are registers */
    else
        *IC += THREE_WORDS;

    return *IC;
}
