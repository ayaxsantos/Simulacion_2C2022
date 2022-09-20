#include "../include/generador_va.h"

//////////////////////////////////////////////

unsigned long generar_valor_dist_uniforme(unsigned long supremo, unsigned long infimo)
{
    double valor_aleatorio;
    unsigned long valor_generado;

    srand(time(NULL));

    // x = a + (b – a) R
    valor_aleatorio = (double) rand() / RAND_MAX;
    valor_generado = infimo + ((supremo - infimo) * valor_aleatorio);

    return valor_generado;
}

//////////////////////////////////////////////
