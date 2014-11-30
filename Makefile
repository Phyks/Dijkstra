CC=gcc -g -Wall -Wextra

SOURCES=main.c graph.c
OBJECTS=$(SOURCES:.c=.o)
EXECTUABLE=a.out

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)
