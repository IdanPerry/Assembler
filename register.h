#ifndef REGISTER_H
#define REGISTER_H

#define REG_NUM 8

typedef enum {R0, R1, R2, R3, R4, R5, R6, R7} reg;

/**
 * Function:
 * is_register recieves a code operand string and returns
 * TRUE if it's one of the 8 registers.
 *
 * Parameter char *, an operand string to be analyzed.
 * Returns TRUE if the operand string represents a valid register.
 *         otherwise returns FALSE.
 */
char *is_register(char *);

/**
 * Function:
 * get_register recieves a string representing a register,
 * and returns the register number.
 *
 * Parameter char *, a register represented as string.
 * Returns the register number.
 */
int get_register(char *);

#endif
