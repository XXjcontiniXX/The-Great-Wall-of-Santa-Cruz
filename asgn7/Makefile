SOURCES  = $(wildcard *.c)
OBJECTS  = $(SOURCES:%.c=%.o)

CC       = clang
CFLAGS   = -Wall -Wpedantic -Werror -Wextra -Ofast

.PHONY: all clean

all: encode decode

test: node.o test.o pq.o code.o io.o# get rid of this jesus please
	$(CC) -o $@ $^

encode: $(OBJECTS)
	$(CC) -o $@ $^

decode: $(OBJECTS)
	$(CC) -o $@ $^


%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o

spotless:
	rm -f encode decode *.o

format:
	clang-format -i -style=file *.[ch]
