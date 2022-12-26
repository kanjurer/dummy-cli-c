COMPILER = gcc
CSTD     = --std=c18
INCLUDES = -Ilib
LIBFLAGS = -lm -lpthread


#main executable
main: main.o history.o
	$(COMPILER) $(CSTD) main.o history.o -o main


#object files
main.o: main.c main.h
	$(COMPILER) $(CSTD) -c main.c -o main.o

history.o: history.c history.h
	$(COMPILER) $(CSTD) -c history.c -o history.o

#clean
clean:
	rm -f main.o history.o main main
