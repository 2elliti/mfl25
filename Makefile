ENABLE_DEBUG := 1

CC := gcc
CFLAGS :=
PWD := ${CURDIR}
SRC := $(PWD)/src
INCLUDES := $(PWD)/includes
BIN := $(PWD)/bin

ifeq ($(ENABLE_DEBUG),1)
CFLAGS += -g
endif


all:
	$(CC)	$(SRC)/main.c $(SRC)/parse.c -I$(INCLUDES) $(CFLAGS) -o $(BIN)/bin 

clean:
	rm $(BIN)/bin
