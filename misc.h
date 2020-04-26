#ifndef MISC_H
#define MISC_H

#define DIREC_NUM 4
#define OPP_CODE_NUM 16
#define INIT_ADDRESS 100

#define MESSAGE_1(str) \
	printf("ERROR at file %s, line %d: %s "\
	str ".\n", file_name, file_line, token)

#define MESSAGE_2(str) \
	printf("ERROR at file %s, line %d: "\
	str ".\n", file_name, file_line)

enum add_mode {SOUR, DES};	/* source and destination addressing modes */
typedef enum {FALSE, TRUE} boolean;
typedef enum {INSTRUCTION, DIRECTIVE} statement;

extern char *file_name;
extern int file_line;

#endif
