CFLAGS = -g -Wall -DDEBUG
SRC_C = ./src/n_puestos_n_colas.c
SRC_H = ./include/n_puestos_n_colas.h
BIN = ./bin
OBJS = ./bin/generador_va.o

LIBS = commons

####################################################

TARGET = n_puestos_n_colas.out

####################################################

.PHONY: all
all: $(OBJS)
	gcc $(CFLAGS) $(SRC_C) -o $(addprefix $(BIN)/,$(TARGET)) $(OBJS) $(LIBS:%=-l%)

./bin/generador_va.o: ./src/generador_va.c ./include/generador_va.h | generate_bin
	gcc -c ./src/generador_va.c -o ./bin/generador_va.o

generate_bin:
	mkdir -p $(BIN)

.PHONY: clean
clean:
	-rm -rfv obj $(BIN)

####################################################
