#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "directive.h"

/**
 * File:
 * directive.c handles directive statements syntax related issues.
 */

#define ERROR -1

char *directives[DIREC_NUM] = {"data", "string", "entry", "extern"};

/* 
 * Private function:
 * read_data analyzes and validates .data type directive statements.
 *
 * Parameter char *, the data to be analyzed.
 * Returns char *, the data seperated with commas and trimed out of white
 *                 space if no errors were found. otherwise returns NULL.
 */
static char *read_data(char *);

/* 
 * Private function:
 * read_string analyzes and validates .string type directive statements.
 *
 * Parameter char *, the string to be analyzed.
 * Returns char *, the string without the quotation marks if no errors were
 *                 found. otherwise returns NULL.
 */
static char *read_string(char *);

/* 
 * Private function:
 * analyze_commas analyzes comma related errors in .data type statements.
 *
 * Parameter char *, the data to be analyzed.
 * Returns char *, the data seperated with commas and trimed out of white
 *                 space if no errors were found. otherwise returns NULL.
 */
static char *analyze_commas(char *);

char *analyze_directive(char *token, dir_attribute attribute) {
	static directive_type dir_type;     /* data, string, entry or extern */
	char *answer;                       /* answer could be a directive string
                                            type or analyzed string/data */

	switch (attribute) {
	/* analyze TYPE of directive */
	case TYPE:
	    for(dir_type = 0; dir_type < DIREC_NUM; dir_type++) {
            if(!strcmp(token, directives[dir_type]))
                break;
	    }

	    if(dir_type == DIREC_NUM) {
            dir_type = ERROR;
			MESSAGE_1("is unknown directive statement");
			return NULL;
	    }

		answer = directives[dir_type];
		break;

	/* analyze directive data */
	case DAT:
		switch (dir_type) {
		case DATA:
			if(!(token = read_data(token)))
				return NULL;

			break;
		case STRING:
			if(!(token = read_string(token)))
                return NULL;

			break;
		case ENTRY:
		    /* do nothing, .entry is being analyzed only at 2nd scan */
			break;

		case EXTERN:
		    /* the token should be a label, thus label.c should validate it */
		    token = "read_label";
			break;
		} /* end switch */

		answer = token;
		break;

	} /* end switch */

	return answer;
}

char *read_data(char *data) {
	char *delims = " \t,-+";
	int i;
	int j;

	/* search invalid characters */
	for(i = 0; data[i] != '\0'; i++) {
        if(isdigit(data[i]))
            continue;

		for(j = 0; delims[j] != '\0'; j++) {
			if(data[i] == delims[j])
                break;
		}

		/* none of the permitted chars were found */
		if(delims[j] == '\0') {
			MESSAGE_2("Data includes invalid characters");
			return NULL;
		}
	}

    /* check plus/minus sign with no consecutive digit */
	for(i = 0; data[i] != '\0'; i++) {
		if((data[i] == '-' || data[i] == '+') && !isdigit(data[i+1]) ) {
			MESSAGE_2("Missing digit after plus/minus sign");
			return NULL;
		}
	}

	return analyze_commas(data);
}

char *read_string(char *string) {
	char *new_str = NULL;
	int i = 0;
	int j;		/* mark the 1st quotation mark position */

	/* skip white space */
	while(string[i] == ' ' || string[i] == '\t')
		i++;

	if(string[i++] != '"') {
		MESSAGE_2("Missing opening quotation marks");
		return NULL;

	} else
		j = i-1;

    /* search for closing quotation marks */
	/* reach end of string */
	while(string[i] != '\0')
		i++;

	/* loop backwords and search quotation marks */
	while(--i > j) {
		if(string[i] != '"' && string[i] != ' ' && string[i] != '\t') {
        	MESSAGE_2("Missing closing quotation marks");
        	return NULL;

		} else if(string[i] == '"')
			break;		/* found closing quotation marks */
	}

	/* string includs only opening quotation marks and white space */
	if(i == j) {
        MESSAGE_2("Missing closing quotation marks");
        return NULL;
	}

	/* trim quotation marks */
	new_str = (char *)calloc(i-j, sizeof(char));
	strncpy(new_str, string+j+1, i-j-1);

	if(strlen(new_str) == 0) {
		MESSAGE_2("Empty string");
		free(new_str);
		return NULL;
	}

	return new_str;
}

char *analyze_commas(char *string) {
    int i;
    int j;

    for(i = 0; string[i] != '\0'; i++) {
        /* digit then white space follows */
        if(isdigit(string[i]) && (string[i+1] == ' ' || string[i+1] == '\t')) {
            j = i+1;

            /* skip white space */
            while(string[j] == ' ' || string[j] == '\t')
                j++;

            if(string[j] != ',' && string[j] != '\0') {
                MESSAGE_2("Missing comma");
                return NULL;
            }
        }
    }

    /* trim white space */
    for(i = 0, j = 0; string[i] != '\0'; i++) {
        if(string[i] != ' ' && string[i] != '\t')
            string[j++] = string[i];
    }

    string[j] = '\0';


	/* check error for comma as first or last data char */
	if(string[0] == ',' || string[strlen(string)-1] == ',') {
		MESSAGE_2("Illegal comma position");
		return NULL;
	}

	for(i = 1; string[i] != '\0'; i++) {
		/* check multiple consecutive commas */
		if(string[i-1] == ',' && string[i] == ',') {
			MESSAGE_2("Illegal consecutive commas");
			return NULL;
		}
	}

	return string;
}
