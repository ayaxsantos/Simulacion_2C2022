#ifndef N_PUESTOS_N_COLAS_GENERADOR_VA_H
#define N_PUESTOS_N_COLAS_GENERADOR_VA_H

#include <time.h>
#include <stdlib.h>

typedef struct dist_uniforme
{
    unsigned int supremo;
    unsigned int infimo;
}t_dist_uniforme;

//////////////////////////////////////////////

unsigned long generar_valor_dist_uniforme(unsigned long, unsigned long);

//////////////////////////////////////////////

#endif //N_PUESTOS_N_COLAS_GENERADOR_VA_H
