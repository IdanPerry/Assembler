#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "misc.h"

/* inst_attribute indicates whether it's a code name or the operands */
typedef enum {OP_CODE, OPERANDS} inst_attribute;

/* operands_group indicates the number of operands that should be expected */
typedef enum {TWO_OPS = 2, ONE_OP = 1, NO_OPS = 0} operands_group;

/**
 * Function:
 * analyze_instruction analyzes and validates all instruction statements,
 * including the code name and the operands.
 *
 * Parameter char *, the name of the code or the operands (depends
 *                   on the inst_attribute parameter) to be analyzed.
 * Parameter inst_attribute, a typedef enum indicates whether the string to
 *                           be analyzed is a code name or the operands.
 * Returns char *, an information regarding the string which recieved when the
 *                 function was called, if no errors were found.
 *                 otherwise it returns NULL. 
 */
char *analyze_instruction(char *, inst_attribute);

#endif
