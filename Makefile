CC=gcc
CFLAGS=-Wall -Werror -g

lsmim: libccan.a
	$(CC) $(CFLAGS) lsmim.c -o lsmim

libccan.a:
	cd ccan/; make; cd ..;
