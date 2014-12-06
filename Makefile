CC=gcc -g -Wall -Wextra
CFLAGS=-std=c99 -Wall `xml2-config --cflags --libs`
SHELL=/bin/zsh

SOURCES=graph.c main.c dijkstra.c queue.c states.c utils.c
OBJECTS=$(SOURCES:%.c=%.o)
EXECUTABLE=dijkstra

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE)

%.o: $(@:%.o=%.c)
	$(CC) $(CFLAGS) -c $(@:%.o=%.c)

clean:
	rm -rf $(EXECUTABLE)
	rm $(SOURCES:%.c=%.o)

test: $(EXECUTABLE)
	@for i in tests/* ; do \
		echo "$$i :" ; \
		echo "Got :" ; \
		./$(EXECUTABLE) $$i ; \
		echo "Expected :" ; \
		cat tests_outputs/$${i#tests/} ; \
		if (diff <(./$(EXECUTABLE) $$i) <(cat tests_outputs/$${i#tests/}) > /dev/null) \
		then \
			printf '\033[1;32;40m' ; \
			echo "[Passed]" ; \
			printf '\033[0m' ; \
		else \
			printf '\033[1;31;40m' ; \
			echo "[Failed]" ; \
			printf '\033[0m' ; \
		fi ; \
		echo "" ; \
	done
