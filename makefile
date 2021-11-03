CC=gcc
CFLAGS = -g 
SOURCES = driver.c parser.c scanner.c
OBJECTS = driver.o parser.o scanner.o
DEP = parser.h scanner.h token.h
TARGET = frontEnd
$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS)-o $@ $^
driver.o : driver.c $(DEP)
	$(CC) $(CFLAGS) -c -o $@ $< 
parser.o : parser.c $(DEP)
	$(CC) $(CFLAGS) -c -o $@ $<
scanner.o : scanner.c $(DEP)
	$(CC) $(CFLAGS) -c -o $@ $<
clean:
	rm -f *.o 
