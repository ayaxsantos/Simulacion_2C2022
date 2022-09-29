#ifndef N_PUESTOS_N_COLAS_GENERADOR_VA_H
#define N_PUESTOS_N_COLAS_GENERADOR_VA_H

#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct dist_intervalo
{
    unsigned int supremo;
    unsigned int infimo;
}t_dist_intervalo;

//////////////////////////////////////////////

unsigned long generar_valor_dist_uniforme(t_dist_intervalo*);
unsigned long metodo_del_rechazo(t_dist_intervalo*, double, double (*fdp)(int));

//////////////////////////////////////////////

#endif //N_PUESTOS_N_COLAS_GENERADOR_VA_H
