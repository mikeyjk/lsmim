CC=gcc
CFLAGS=-Wall -Werror -g
INCL=ccan/

lsmim: libccan.a
	$(CC) $(CFLAGS) lsmim.c ccan/libccan.a -I$(INCL) -o lsmim

libccan.a:
	cd ccan/; make; cd ..;
