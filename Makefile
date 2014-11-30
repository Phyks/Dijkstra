CC?=gcc -g -Wall -Wextra
CFLAGS?=-std=c99

SOURCES=graph.c main.c
EXECUTABLE=a.out

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES)

clean:
	rm -rf $(EXECUTABLE)
