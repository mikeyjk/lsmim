CC=gcc
CFLAGS=-Wall -Werror -g
OUT_DIR=bin
APP_NAME=lsmim
INCLUDE_PATH ?= criterion/include
LIB_PATH ?= criterion/lib
LIBS=criterion

lsmim: runTests
	$(info    ----> building app)
	$(CC) $(CFLAGS) $(APP_NAME).c -o $(OUT_DIR)/$(APP_NAME)

runTests: lsmimTest
	$(info    ----> running tests)
	./$(OUT_DIR)/$(APP_NAME)Test

lsmimTest:
	$(info		----> building tests)
	$(CC) $(CFLAGS) $(APP_NAME)Test.c -o $(OUT_DIR)/$(APP_NAME)Test -I$(INCLUDE_PATH) -L$(LIB_PATH) -l$(LIBS)

#$(CC) $(CFLAGS) $(APP_NAME)Test.c -o $(OUT_DIR)/$(APP_NAME)Test -I$(INCLUDE_PATH) -L$(LIB_PATH)/$(LIBS)
clean:
	rm $(OUT_DIR)/$(APP_NAME)Test
	rm $(OUT_DIR)/$(APP_NAME)
