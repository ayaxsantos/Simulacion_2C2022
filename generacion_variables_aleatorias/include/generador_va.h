#ifndef GENERADOR_VA_MAIN_H
#define GENERADOR_VA_MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <commons/log.h>
#include <commons/config.h>
#include <time.h>

///////////////////////////////////////////////

void generar_valores(unsigned long long);
void iniciar_logger();
void loggear_funcion(int, int);
void cargar_confguracion(unsigned int*, unsigned int*);
int* inicializar_resultados(int);

///////////////////////////////////////////////

t_log *un_logger;

///////////////////////////////////////////////

#endif //GENERADOR_VA_MAIN_H
