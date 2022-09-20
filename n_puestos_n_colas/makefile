CFLAGS = -Wall -DDEBUG
SRC_C = ./src/n_puestos_n_colas.c
SRC_H = ./include/n_puestos_n_colas.h
BIN = ./bin

LIBS = commons

####################################################

filename = n_puestos_n_colas.out

####################################################

.PHONY: all
all: clean
	mkdir $(BIN)
	gcc -g $(CFLAGS) $(SRC_C) -o $(addprefix $(BIN)/,$(filename)) $(LIBS:%=-l%)

.PHONY: all
clean:
	-rm -rfv obj bin

####################################################
