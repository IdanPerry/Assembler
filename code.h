#ifndef CODE_H
#define CODE_H

#include "symbols_table.h"
#include "code_table.h"

#define MAX_OP 2
#define MESSAGE_3(str) \
	printf("ERROR at file %s, line %d: %s "\
	str ".\n", file_name, file_line, code_str)

enum {ONE_WORD = 1, TWO_WORDS = 2, THREE_WORDS = 3};

/**
 * Function:
 * calc_memory_word checks for each operand - what addressing
 * mode does it fit. when the necessary information is achieved,
 * the IC is beeing updated and the words related to these
 * operands and the opcode are inserted into the code table.
 *
 * Parameter int *, a pointer to the IC (instruction counter).
 * Parameter code *, a pointer to the head of the code table.
 * Parameter char *, a string representation of the opcode.
 * Parameter char *, an array of the operands.
 * Returns int, the updated IC.
 */
int calc_memory_word(int *, code *, char *, char *, symbol *);

/**
 * Function:
 * complete_label_code completes the code table with the
 * missing labels addresses.
 *
 * Parameter code *, a pointer to the code table head.
 * Parameter symbol *, a pointer to the symbols table head.
 * Returns nothing.
 */
void complete_label_code(code *, symbol *);

#endif
