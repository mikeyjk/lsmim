lsmim: libccan.a
	gcc -Wall -Werror -g lsmim.c ccan/libccan.a -Iccan/ -o lsmim

libccan.a:
	cd ccan/; make; cd ..
