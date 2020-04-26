assembler: assembler.o input_manager.o scan_one.o label.o instruction.o directive.o symbols_table.o data_table.o register.o code_table.o code.o data.o scan_two.o output_manager.o
	gcc -g -ansi -Wall -pedantic assembler.o input_manager.o scan_one.o label.o instruction.o directive.o symbols_table.o register.o data_table.o code_table.o code.o data.o scan_two.o output_manager.o -o assembler -lm

assembler.o: assembler.c
	gcc -c -ansi -Wall -pedantic assembler.c -o assembler.o

input_manager.o: input_manager.c input_manager.h
	gcc -c -ansi -Wall -pedantic input_manager.c -o input_manager.o

scan_one.o: scan_one.c scan_one.h misc.h
	gcc -c -ansi -Wall -pedantic scan_one.c -o scan_one.o

label.o: label.c label.h
	gcc -c -ansi -Wall -pedantic label.c -o label.o

instruction.o: instruction.c instruction.h
	gcc -c -ansi -Wall -pedantic instruction.c -o instruction.o

directive.o: directive.c directive.h
	gcc -c -ansi -Wall -pedantic directive.c -o directive.o

symbols_table.o: symbols_table.c symbols_table.h
	gcc -c -ansi -Wall -pedantic symbols_table.c -o symbols_table.o

data_table.o: data_table.c data_table.h
	gcc -c -ansi -Wall -pedantic data_table.c -o data_table.o

data.o: data.c data.h
	gcc -c -ansi -Wall -pedantic data.c -o data.o

register.o: register.c register.h
	gcc -c -ansi -Wall -pedantic register.c -o register.o

code_table.o: code_table.c code_table.h
	gcc -c -ansi -Wall -pedantic code_table.c -o code_table.o

code.o: code.c code.h
	gcc -c -ansi -Wall -pedantic code.c -o code.o

scan_two.o: scan_two.c scan_two.h
	gcc -c -ansi -Wall -pedantic scan_two.c -o scan_two.o

output_manager.o: output_manager.c output_manager.h
	gcc -c -ansi -Wall -pedantic output_manager.c -o output_manager.o -lm
