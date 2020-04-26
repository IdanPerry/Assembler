#ifndef CODE_TABLE_H
#define CODE_TABLE_H

#include "misc.h"

#define BITS 15

/* Each of the next 4 structs represents
	a 15 bits memory block ("word"). */

/* first_word holds the op code name */
typedef struct {
    unsigned int E:1;
    unsigned int R:1;
    unsigned int A:1;
    unsigned int dest:4;
    unsigned int source:4;
    unsigned int op_code:4;
} first_word;

/* number_word holds a prompt or direct
	addressing mode address */
typedef struct {
    unsigned int E:1;
    unsigned int R:1;
    unsigned int A:1;
    unsigned int num:12;
} number_word;

/* register_word holds a direct or non
	direct register addressing mode address*/
typedef struct {
    unsigned int E:1;
    unsigned int R:1;
    unsigned int A:1;
    unsigned int dest_reg:3;
    unsigned int source_reg:3;
    unsigned int unused:6;
} register_word;

/* directive_word holds a directive type data */
typedef struct {
    unsigned int num:15;
} directive_word;

/* a word could be one of the 4 listed above */
typedef union {
    first_word fw;
    number_word nw;
    register_word rw;
    directive_word dw;
} word;

/* single node in the code list */
typedef struct code_node {
    word w;
    char *label;
    int address;
    boolean external;
    struct code_node *next;
} code;

/* first word bits distribution */
typedef enum {E = 1, R = 1, A = 1, DEST = 4, SOURCE = 4, CODE = 4} fw_field_size;

/* addressing modes represented as powers of 2 */
typedef enum {PROMPT = 1 ,DIRECT = 2, INDIRECT_REG = 4, DIRECT_REG = 8} addressing_mode;

typedef enum {FIRST_W, NUM_W, REG_W, DIREC_W} word_type;

typedef enum {MOV, CMP, ADD, SUB, LEA, CLR, NOT, INC,
                DEC, JMP, BNE, RED, PRN, JSR, RTS, STOP} operation_code;

/**
 * Function:
 * insert_first_word updates the code table with the code of the opcode
 * and the code of the addressing mode operands.
 *
 * Parameter code *, a pointer to the code table head.
 * Parameter addressing_mode *, an array of the source and destination
 * 								operands addressing modes.
 * Parameter operation_code, an opcode represented by its number.
 * Returns nothing.
 */
void insert_first_word(code *, addressing_mode *, operation_code);

/**
 * Function:
 * insert_words updates the code table with additional words.
 *
 * Parameter code *, a pointer to the code table head.
 * Parameter addressing_mode *, an array of the source and destination
 * 								operands addressing modes.
 * Parameter int *, an array of the source and destination operands
 *                  codes/addresses.
 * Parameter char **, an array of source and destination labels operands.
 *                    if the operands are not labels, the array contains nulls.
 */
void insert_words(code *, addressing_mode *, int *, char **);

/**
 * Function:
 * create_code_node allocates a memory for a new word (memory block)
 * and inserts the new node at the end of the code table.
 * The head node gets an address initialized to 100, and each new node gets
 * an address increasing by one.
 *
 * Parameter code **, a pointer address to the head of the code table.
 * Parameter boolean, flags for an address reset: if TRUE - the address
 *                    in the table will be reset to 100.
 * Returns nothing.
 */
void create_code_node(code **, boolean);

/**
 * Function:
 * free_code_table frees all allocated memory used for the list.
 * it also frees the memory allocated for the label strings.
 *
 * Parameter code **, a pointer address to the head of the code table.
 * Returns nothing.
 */
void free_code_table(code **);

/**
 * Function:
 * print_code_table prints all the table words and its addresses.
 *
 * Paramater code *, a pointer to the head of the code table.
 * Returns nothing.
 */
void print_code_table(code *);

/**
 * Function:
 * search_code returns the first code which at least one the opernads
 * was defined as a label.
 *
 * Parameter code *, a pointer to the head of the code table.
 * returns the first code which at least one the opernads
 *         was defined as a label.
 */
code *search_code(code *);

/**
 * Function:
 * insert_data updates the code table with a directive data, and
 * returns a pointer to the next node for easy new data insert.
 *
 * Parameter code *, a pointer to the head of the code table.
 * Parameter int, the data to be insert.
 * Returns a pointer to the next node in the table.
 */
code *insert_data(code *, int);

/**
 * Function:
 * get_next_extern returns a code which defined as external. if a pointer
 * to the head of the table has been recieved, the first external code will
 * be returned. if the same pointer which was returned recently is recieved,
 * the next external code in line will be returned.
 *
 * Parameter code *, a pointer to the head of the code table.
 * Returns a pointer to a code which defined an external.
 */
code *get_next_extern(code *);

/**
 * Function:
 * update_extern updates a word to be meeting the external properties.
 *
 * Parameter code *, a pointer to the node which its word is to be
 *                   updated.
 * Returns nothing.
 */
void update_extern(code *);

/*
 * Function:
 * get_word returns a string representing of the word in binary form.
 *
 * Parameter word, the word to translate to binary form.
 * Returns a string representing of the word in binary form.
 */
char *get_word(word);

#endif
