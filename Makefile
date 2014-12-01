CC?=gcc -g -Wall -Wextra
CFLAGS?=-std=c99 -Wall

SOURCES=graph.c main.c dijkstra.c queue.c
EXECUTABLE=a.out

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES)

clean:
	rm -rf $(EXECUTABLE)
