run: pre_assembler.o assembler.o first_pass.o second_pass.o text_correctness.o errors.o
	gcc -g -Wall -ansi -pedantic pre_assembler.o assembler.o first_pass.o second_pass.o text_correctness.o errors.o -o run
pre_assembler.o: pre_assembler.c pre_assembler.h
	gcc -c -Wall -ansi -pedantic pre_assembler.c -o pre_assembler.o
assembler.o: assembler.c assembler.h
	gcc -c -Wall -ansi -pedantic assembler.c -o assembler.o
first_pass.o: first_pass.c first_pass.h
	gcc -c -Wall -ansi -pedantic first_pass.c -o first_pass.o
second_pass.o: second_pass.c second_pass.h
	gcc -c -Wall -ansi -pedantic second_pass.c -o second_pass.o
text_correctness.o: text_correctness.c text_correctness.h
	gcc -c -Wall -ansi -pedantic text_correctness.c -o text_correctness.o
errors.o: errors.c errors.h
	gcc -c -Wall -ansi -pedantic errors.c -o errors.o
