CC = clang
CFLAGS = -Ofast -Wall -Werror -Wextra -Wpedantic

bv: bv.o banhammer.o
	$(CC) -o $@ $^ $(LFLAGS)
test: str.o test.o bv.o bf.o city.o node.o
	$(CC) -o $@ $^ $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f bv *.o 

format:
	clang-format -i -style=file *.[ch]
