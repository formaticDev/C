CC=gcc
CFLAGS= -Wall -ansi -pedantic -I. -g
DEPS = assembler.h
RM= rm
OBJ= bitwise.o lists.o functions.o output.o first_pass.o second_pass.o main.o

%.o: %.C $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

assembler: $(OBJ) 
	$(CC) $(CFLAGS)  $(OBJ)  -o assembler  -lm
	
clean:
	$(RM) $(OBJ)
