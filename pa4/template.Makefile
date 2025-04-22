CC     = gcc
CFLAGS = -g -std=c99 -Wall -Wvla -Werror -fsanitize=address,undefined

truthtable: truthtable.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf truthtable *.o *.a *.dylib *.dSYM
