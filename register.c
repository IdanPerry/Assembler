#include <stdio.h>
#include <string.h>
#include "register.h"

/**
 * File:
 * register.c implemets registers activity.
 */

char *registers[REG_NUM] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

char *is_register(char *operand) {
    int i;

	/* search to find if one of the 8 registers match */
    for(i = 0; i < REG_NUM; i++) {
        if(!strcmp(operand, registers[i]))
            return registers[i];
    }

    return NULL;
}

int get_register(char *operand) {
    int i;

    for(i = 0; i < REG_NUM; i++) {
        if(!strcmp(operand, registers[i]))
            break;
    }

    if(i == REG_NUM) {
        puts("WARNING: No register was found");
		return -1;
	}

    return i;
}
