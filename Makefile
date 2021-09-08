CC=gcc
CFLAGS=-Wall -Werror -g
APP_NAME=lsmim

runTests: lsmimTest lsmim
	echo "running tests\n";
	./$(APP_NAME)Test

lsmimTest:
	echo "building tests\n";
	$(CC) $(CFLAGS) $(APP_NAME)Test.c -o $(APP_NAME)Test -lcriterion

lsmim: runTests
	echo "building app\n";
	$(CC) $(CFLAGS) $(APP_NAME).c -o $(APP_NAME)
