#ifndef DATA_H
#define DATA_H

#include "data_table.h"
#include "code_table.h"

/**
 * Function:
 * update_DC updated the data counter according to the directive
 * type statement.
 *
 * Parameter int *, a pointer to the data counter.
 * Parameter char *, a strign representation of the data.
 * Parameter int, the directive type (.string or .data).
 * Returns int, the updated data counter.
 */
int update_DC(int *, char *, int);

/**
 * Function:
 * complete_data_code completes the code table with the
 * missing directive data at the end of the table.
 *
 * Parameter data *, a pointer to the data table head.
 * Parameter code *, a pointer to the code table head.
 * Returns nothing.
 */
void complete_data_code(data *, code *);

#endif
