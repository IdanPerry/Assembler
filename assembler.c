#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input_manager.h"

/**
 * =================================================================*
 *                                                                  *
 * Program: Assembler                                               *
 * Author: Idan Perry                                               *
 * Date: 1-3-2020                                                  *
 *                                                                  *
 * This program translates assembly language to machine language.   *
 *                                                                  *
 * =================================================================*
 */

/**
 * File:
 * Assembler.c file includes the main function, which takes as
 * parameters the assembly files provided as arguments in the
 * command line.
 */

char *file_name;

int main(int argc, char *argv[]) {
	FILE *file_ptr;
	int i;		/* argv iterator */

	if(argc == 1) {
		printf("ERROR: Missing assembly files.\n");
		exit(1);
	}

	for(i = 1; i < argc; i++) {
		file_name = (char *)calloc(strlen(argv[i]) + 4, sizeof(char));
		strcpy(file_name, argv[i]);
		strcat(file_name, ".as");
		file_ptr = fopen(file_name, "r");	/* open one file at a time */

		if(!file_ptr) {
			printf("ERROR: Couldn't open file %s\n", file_name);
			
		} else {
			get_input(file_ptr);
			fclose(file_ptr);
		}

		free(file_name);
	}

	return 0;
}
