CFLAGS = -Wall -DDEBUG
SRC_C = ./src/generador_va.c
SRC_H = ./include/generador_va.h
BIN = ./bin

LIBS = commons

####################################################

filename = generador_va.out

####################################################

.PHONY: all
all: clean
	mkdir $(BIN)
	gcc -g $(CFLAGS) $(SRC_C) -o $(addprefix $(BIN)/,$(filename)) $(LIBS:%=-l%)

.PHONY: all
clean:
	-rm -rfv obj bin

####################################################
