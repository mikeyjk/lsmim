CC=gcc
CFLAGS=-Wall -Werror -g
OUT_DIR=bin
APP_NAME=lsmim
LIBS=criterion

lsmim: runTests
	$(info    ----> building app)
	$(CC) $(CFLAGS) $(APP_NAME).c -o $(OUT_DIR)/$(APP_NAME)

runTests: lsmimTest
	$(info    ----> running tests)
	./$(OUT_DIR)/$(APP_NAME)Test

lsmimTest:
	$(info		----> building tests)
	$(CC) $(CFLAGS) $(APP_NAME)Test.c -o $(OUT_DIR)/$(APP_NAME)Test -l$(LIBS)

clean:
	rm $(OUT_DIR)/$(APP_NAME)Test
	rm $(OUT_DIR)/$(APP_NAME)
