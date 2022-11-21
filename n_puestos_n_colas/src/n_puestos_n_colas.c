#include "../include/n_puestos_n_colas.h"
//TODO: Optimizar MAKEFILE y ver el tema de multiples fdp para c/puesto
//////////////////////////////////////////////

int main(int argc, char* argv[])
{
    int tiempo_finalizacion = 0;

    iniciar_logger();
    cargar_confguracion(&tiempo_finalizacion);
    cargar_configuracion_fdp();

    realizar_simulacion(tiempo_finalizacion);

    return EXIT_SUCCESS;
}

void procesar_llegada(t_eventos_futuros *eventos_futuros,t_estadisticas *estadisticas)
{
    bool se_queda = false;
    int puesto_elegido;
    int tiempo_atencion;

    for(int j = 0; j<cantidad_puestos; j++)
        estadisticas->sumatoria_tiempos_de_llegada[j] += (eventos_futuros->tiempo_proxima_llegada - t) * num_elem[j];

    t = eventos_futuros->tiempo_proxima_llegada;

    eventos_futuros->tiempo_proxima_llegada = t + (int) obtener_intervalo_entre_arribos();
    puesto_elegido = seleccionar_puesto(eventos_futuros->tiempo_proxima_salida);

    decidir_arrepentimiento(num_elem[puesto_elegido],&estadisticas->arrepentidos_por_cola[puesto_elegido],&se_queda);

    if(se_queda)
    {
        num_elem[puesto_elegido]++;
        estadisticas->total_personas[puesto_elegido]++;

        if(num_elem[puesto_elegido] == 1)
        {
            estadisticas->sumatoria_tiempo_ocioso[puesto_elegido] += t - estadisticas->intervalo_tiempo_ocioso[puesto_elegido];
            tiempo_atencion = obtener_tiempo_atencion(puesto_elegido);
            eventos_futuros->tiempo_proxima_salida[puesto_elegido] = t + tiempo_atencion;
            estadisticas->sumatoria_tiempos_atencion[puesto_elegido] += tiempo_atencion;
        }
    }
}

double fdp(int x)
{
    return 0.375 - (0.09375 * ((x-4)^2));
}

unsigned int obtener_tiempo_atencion(int puesto_elegido)
{
    //TODO: Resolver como procesar n fdp
    return generar_valor_dist_uniforme(distribucion_ta);
    //return metodo_del_rechazo(distribucion_ta, 0.375, fdp);
}

unsigned long obtener_intervalo_entre_arribos()
{
    return generar_valor_dist_uniforme(distribucion_ia);
}

void procesar_salida(t_eventos_futuros *eventos_futuros, t_estadisticas *estadisticas, int indice_puesto)
{
    int tiempo_atencion;
    int a = 0;
    int b = 0;
    for(int j = 0; j<cantidad_puestos; j++)
    {
        a = eventos_futuros->tiempo_proxima_salida[indice_puesto];
        b = a - t;
        estadisticas->sumatoria_tiempo_de_salida[j] = estadisticas->sumatoria_tiempo_de_salida[j] + (b* num_elem[j]);
    }

    t = eventos_futuros->tiempo_proxima_salida[indice_puesto];
    num_elem[indice_puesto]--;

    if(num_elem[indice_puesto] > 0)
    {
        tiempo_atencion = obtener_tiempo_atencion(indice_puesto);
        eventos_futuros->tiempo_proxima_salida[indice_puesto] = t + tiempo_atencion;
        estadisticas->sumatoria_tiempos_atencion[indice_puesto] += tiempo_atencion;
    }
    else
    {
        estadisticas->intervalo_tiempo_ocioso[indice_puesto] = t;
        eventos_futuros->tiempo_proxima_salida[indice_puesto] = HV;
    }
}

void realizar_simulacion(int tiempo_finalizacion)
{
    int indice_puesto;

    t_resultados *resultados = inicializar_resultados();
    t_eventos_futuros *eventos_futuros = malloc(sizeof(t_eventos_futuros));
    t_estadisticas *estadisticas = malloc(sizeof(t_estadisticas));

    set_condiciones_iniciales(estadisticas,eventos_futuros);

    while(true)
    {
        indice_puesto = obtener_puesto_menor_TPS(eventos_futuros->tiempo_proxima_salida);

        if(eventos_futuros->tiempo_proxima_llegada <= eventos_futuros->tiempo_proxima_salida[indice_puesto])
        {
            //ES UNA LLEGADA
            procesar_llegada(eventos_futuros,estadisticas);
            log_info(un_logger,"ENTRADA | Tiempo: %d",t);
        }
        else
        {
            //ES UNA SALIDA
            procesar_salida(eventos_futuros, estadisticas, indice_puesto);
            log_info(un_logger,"SALIDA | Tiempo: %d",t);
        }

        if(t >= tiempo_finalizacion)
        {
            calcular_resultados(resultados,estadisticas);
            imprimir_resultados(resultados);
            break;
        }
    }
    liberar_memoria(resultados,estadisticas,eventos_futuros);
}

void liberar_memoria(t_resultados *resultados, t_estadisticas *estadisticas,t_eventos_futuros *eventos_futuros)
{
    liberar_resultados(resultados);
    liberar_estadisticas(estadisticas);
    liberar_eventos_futuros(eventos_futuros);
    liberar_memoria_globales();
}

void liberar_resultados(t_resultados *resultados)
{
    free(resultados->porcentaje_arrepentimiento);
    free(resultados->porcentaje_tiempo_ocioso);
    free(resultados->promedio_espera);
    free(resultados->promedio_permenancia);
    free(resultados);
}

void liberar_estadisticas(t_estadisticas *estadisticas)
{
    free(estadisticas->arrepentidos_por_cola);
    free(estadisticas->intervalo_tiempo_ocioso);
    free(estadisticas->sumatoria_tiempo_de_salida);
    free(estadisticas->sumatoria_tiempo_ocioso);
    free(estadisticas->sumatoria_tiempos_atencion);
    free(estadisticas->sumatoria_tiempos_de_llegada);
    free(estadisticas->total_personas);
    free(estadisticas);
}

void liberar_eventos_futuros(t_eventos_futuros *eventos_futuros)
{
    free(eventos_futuros->tiempo_proxima_salida);
    free(eventos_futuros);
}

void liberar_memoria_globales()
{
    free(distribucion_ia);
    free(distribucion_ta);
    log_destroy(un_logger);
}

void calcular_resultados(t_resultados *resultados, t_estadisticas *estadisticas)
{
    int tiempo_en_sistema;

    for(int k=0; k<cantidad_puestos; k++)
    {
        tiempo_en_sistema = estadisticas->sumatoria_tiempo_de_salida[k] + estadisticas->sumatoria_tiempos_de_llegada[k];

        resultados->promedio_permenancia[k] = calcular_promedio_permanencia(tiempo_en_sistema,estadisticas,k);
        resultados->promedio_espera[k] = calcular_promedio_espera(tiempo_en_sistema,estadisticas,k);
        resultados->porcentaje_tiempo_ocioso[k] = calcular_porcentaje_de_tiempo_ocioso(estadisticas,k);
        resultados->porcentaje_arrepentimiento[k] = calcular_porcentaje_arrepentimiento(estadisticas,k);
    }
}

void imprimir_resultados(t_resultados* resultados)
{
    for(int p=0; p<cantidad_puestos; p++)
    {
        log_info(un_logger,"------------- Resultados puesto %d -------------",p);
        log_info(un_logger,"Promedio de permanencia en el sistema: %lf",resultados->promedio_permenancia[p]);
        log_info(un_logger,"Promedio de espera en cola: %lf",resultados->promedio_espera[p]);
        log_info(un_logger,"Porcentaje de tiempo ocioso: %lf",resultados->porcentaje_tiempo_ocioso[p]);
        log_info(un_logger,"Porcentaje de arrepentimiento: %lf",resultados->porcentaje_arrepentimiento[p]);
    }
}

double calcular_promedio_permanencia(int tiempo_en_sistema, t_estadisticas *estadisticas, int k)
{
    if(estadisticas->total_personas[k] == 0)
        return -1;
    else
        return tiempo_en_sistema / estadisticas->total_personas[k];
}

double calcular_promedio_espera(int tiempo_en_sistema, t_estadisticas *estadisticas, int k)
{
    if(estadisticas->total_personas[k] == 0)
        return -1;
    else
        return (tiempo_en_sistema - estadisticas->sumatoria_tiempos_atencion[k]) / estadisticas->total_personas[k];
}

double calcular_porcentaje_de_tiempo_ocioso(t_estadisticas *estadisticas, int k)
{
    return (estadisticas->sumatoria_tiempo_ocioso[k] * 100) / t;
}

double calcular_porcentaje_arrepentimiento(t_estadisticas *estadisticas, int k)
{
    int total_personas_con_arrep = estadisticas->total_personas[k] + estadisticas->arrepentidos_por_cola[k];
    if(total_personas_con_arrep == 0)
        return -1;
    else
        return (estadisticas->arrepentidos_por_cola[k] / total_personas_con_arrep) * 100;
}

void set_condiciones_iniciales(t_estadisticas *estadisticas, t_eventos_futuros *eventos_futuros)
{
    //TODO: Refactorizar esto...
    inicializar_vector_nulo((void*) &estadisticas->sumatoria_tiempos_de_llegada);
    inicializar_vector_nulo((void*) &estadisticas->sumatoria_tiempo_de_salida);
    inicializar_vector_nulo((void*) &estadisticas->sumatoria_tiempos_atencion);
    inicializar_vector_nulo((void*) &estadisticas->total_personas);
    inicializar_vector_nulo((void*) &estadisticas->arrepentidos_por_cola);
    inicializar_vector_nulo((void*) &estadisticas->sumatoria_tiempo_ocioso);
    inicializar_vector_nulo((void*) &estadisticas->intervalo_tiempo_ocioso);

    eventos_futuros->tiempo_proxima_llegada = 0;
    eventos_futuros->tiempo_proxima_salida = malloc(cantidad_puestos * sizeof(int));
    for(int k=0; k<cantidad_puestos; k++)
        eventos_futuros->tiempo_proxima_salida[k] = HV;

    t = 0;
    inicializar_vector_nulo((void*) &num_elem);
}

t_resultados *inicializar_resultados()
{
    t_resultados *unos_resultados = malloc(sizeof(t_resultados));

    unos_resultados->porcentaje_arrepentimiento =
            malloc(sizeof(unos_resultados->porcentaje_arrepentimiento
    [0])*cantidad_puestos);
    unos_resultados->porcentaje_tiempo_ocioso =
            malloc(sizeof(unos_resultados->porcentaje_tiempo_ocioso[0])*cantidad_puestos);

    unos_resultados->promedio_espera = malloc(sizeof(unos_resultados->promedio_espera[0])*cantidad_puestos);
    unos_resultados->promedio_permenancia = malloc(sizeof(unos_resultados->promedio_permenancia[0])*cantidad_puestos);

    return unos_resultados;
}

void inicializar_vector_nulo(void **un_campo)
{
    int tamanio = cantidad_puestos * sizeof(double);
    *un_campo = malloc(tamanio);
    memset(*un_campo, 0, tamanio);
}

//Rutina de arrepentimiento
void decidir_arrepentimiento(int num_elem,double *arrepentidos_en_cola, bool* se_queda)
{
    int elem_sin_persona = num_elem - 1;
    srand(time(NULL));

    if(elem_sin_persona <= 5)
        *se_queda = true;

    else if(elem_sin_persona > 8)
    {
        (*arrepentidos_en_cola)++;
        *se_queda = false;
    }
    else
    {
        float valor_aleatorio = (float) rand()/RAND_MAX;
        if(valor_aleatorio >= 0.2)
        {
            (*arrepentidos_en_cola)++;
            *se_queda = false;
        }
        else
            *se_queda = true;
    }
}

int seleccionar_puesto(int unos_tps[] )
{
    int indice_buscado = 0;

    for(int i = 0; i < cantidad_puestos; i++)
    {
        if(num_elem[i] < num_elem[indice_buscado])
            indice_buscado = i;
    }
    return indice_buscado;
}

int obtener_puesto_menor_TPS(int unos_tps[])
{
    int indice_buscado = 0;
    for(int i = 0; i < cantidad_puestos;i++)
    {
        if(unos_tps[i] < unos_tps[indice_buscado])
        {
            indice_buscado = i;
        }
        else if(unos_tps[i] == HV)
        {
            indice_buscado = i;
            break;
        }
    }
    return indice_buscado;
}

void iniciar_logger()
{
    t_log_level nivel_log = LOG_LEVEL_INFO;
    un_logger = log_create("./n_puestos_n_colas.log","n_puestos_n_colas",true,nivel_log);

    if(un_logger == NULL)
    {
        write(0,"ERROR -> No se pudo crear el log \n",40);
        exit(EXIT_FAILURE);
    }
    log_info(un_logger,"--------------- Iniciando simulacion -----------------");
}

void cargar_confguracion(int *tiempo_finalizacion)
{
    t_config *una_config = config_create("./parametros.config");

    cantidad_puestos = config_get_int_value(una_config, "CANTIDAD_COLAS_Y_PUESTOS");
    *tiempo_finalizacion = config_get_int_value(una_config,"TIEMPO_SIMULACION");

    config_destroy(una_config);
}

void cargar_configuracion_fdp()
{
    cargar_confguracion_ia();
    cargar_confguracion_ta();
}

void cargar_confguracion_ia()
{
    t_config *una_config = config_create("./valores_ia.config");
    distribucion_ia = malloc(sizeof(t_dist_intervalo));

    distribucion_ia->supremo = config_get_int_value(una_config, "SUPREMO");
    distribucion_ia->infimo = config_get_int_value(una_config, "INFIMO");

    config_destroy(una_config);
}

void cargar_confguracion_ta()
{
    t_config *una_config = config_create("./valores_ta.config");
    distribucion_ta = malloc(sizeof(t_dist_intervalo));

    distribucion_ta->supremo = config_get_int_value(una_config, "SUPREMO");
    distribucion_ta->infimo = config_get_int_value(una_config, "INFIMO");

    config_destroy(una_config);
}

//////////////////////////////////////////////
