#include "../include/generador_va.h"

//////////////////////////////////////////////

static double obtener_valor_aleatorio();
static double inversa_dist_uniforme(t_dist_intervalo*, double);

//////////////////////////////////////////////

unsigned long generar_valor_dist_uniforme(t_dist_intervalo *intervalo)
{
    double valor_aleatorio;
    unsigned long valor_generado;

    srand(time(NULL));

    // x = a + (b – a) R
    valor_aleatorio = obtener_valor_aleatorio();
    valor_generado = inversa_dist_uniforme(intervalo,valor_aleatorio);

    return valor_generado;
}

unsigned long metodo_del_rechazo(t_dist_intervalo *intervalo, double valor_maximo, double(*fdp)(int))
{
    double valor_aleatorio_pri;
    double valor_aleatorio_seg;
    unsigned long valor_generado;
    double y;

    srand(time(NULL));
    while(true)
    {
        valor_aleatorio_pri =  obtener_valor_aleatorio();
        valor_aleatorio_seg = obtener_valor_aleatorio();

        valor_generado = inversa_dist_uniforme(intervalo,valor_aleatorio_pri);

        y = valor_maximo * valor_aleatorio_seg;

        if(y <= fdp(valor_generado))
            return valor_generado;
    }
}

static double inversa_dist_uniforme(t_dist_intervalo *intervalo, double valor_aleatorio)
{
    return intervalo->infimo + ((intervalo->supremo - intervalo->infimo) * valor_aleatorio);
}

static double obtener_valor_aleatorio()
{
    return (double) rand() / RAND_MAX;
}

//////////////////////////////////////////////
