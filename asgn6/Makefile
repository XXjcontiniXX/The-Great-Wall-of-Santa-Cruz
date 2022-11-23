CC = clang
CFLAGS = -Ofast -Wall -Werror -Wextra -Wpedantic

test: test.o bv.o bf.o city.o node.o ll.o ht.o parser.o
	$(CC) -o $@ $^ $(LFLAGS)

banhammer: bv.o bf.o city.o node.o ll.o ht.o parser.o banhammer.o
	$(CC) -o $@ $^ $(LFLAGS)

all: bv.o bf.o city.o node.o ll.o ht.o parser.o banhammer.o
	$(CC) -o $@ $^ $(LFLAGS)
%.o: %.c
	$(CC) $(CFLAGS) -c $<

spotless:
	rm -f banhammer *.o

clean:
	rm -f *.o 

format:
	clang-format -i -style=file *.[ch]
