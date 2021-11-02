CC=gcc
CFLAGS = -g 
SOURCES = scanner.c parser.c
OBJECTS = scanner.o parser.o
DEP = scanner.h token.h parser.h
TARGET = parser
$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS)-o $@ $^
parser.o : parser.c parser.h
	$(CC) $(CFLAGS) -c -o $@ $<
scanner.o : scanner.o scanner.h
	$(CC) $(CFLAGS) -c -o $@ $< 
clean:
	rm -f *.o 
