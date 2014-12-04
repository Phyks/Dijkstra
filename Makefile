CC=gcc -g -Wall -Wextra
CFLAGS=-std=c99 -Wall

SOURCES=graph.c main.c dijkstra.c queue.c states.c utils.c
EXECUTABLE=Debug

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES)

clean:
	rm -rf $(EXECUTABLE)

test: $(EXECUTABLE)
	@for i in tests/* ; do \
		echo "$$i :" ; \
		echo "Got :" ; \
		./$(EXECUTABLE) $$i ; \
		echo "Expected :" ; \
		cat tests_outputs/$${i#tests/}; \
		echo "" ; \
	done
