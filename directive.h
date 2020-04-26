#ifndef DIRECTIVE_H
#define DIRECTIVE_H

#include "misc.h"

/* dir_attribute indicates whether its a directive type
   (.string, .extern etc.) or the data itself. */
typedef enum {TYPE, DAT} dir_attribute;
typedef enum {DATA, STRING, ENTRY, EXTERN} directive_type;

/**
 * Function:
 * analyze_directive analyzes and validates all directive statements,
 * including the directive name and its data itself.
 *
 * Parameter char *, the name of the directive or the .data/.string
 *                   (depends on the dir_attribute parameter) to be analyzed.
 * Parameter dir_attribute, a typedef enum indicates whether the string to
 *                          analyzed is a directive name or the data itself.
 * Returns char *, an information regarding the string which recieved when the
 *                 function was called, if no errors were found. otherwise
 *                 it returns NULL. 
 */
char *analyze_directive(char *, dir_attribute);

#endif
