#ifndef GENERADOR_VA_MAIN_H
#define GENERADOR_VA_MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <commons/log.h>
#include <commons/config.h>
#include <string.h>
#include <limits.h>
#include "../include/generador_va.h"

#define HV INT_MAX

///////////////////////////////////////////////

typedef struct eventos
{
    int tiempo_proxima_llegada;
    int *tiempo_proxima_salida;
}t_eventos_futuros;

typedef struct estadisticas
{
    double *sumatoria_tiempos_de_llegada;
    double *sumatoria_tiempo_de_salida;
    double *sumatoria_tiempos_atencion;
    double *sumatoria_tiempo_ocioso;
    double *intervalo_tiempo_ocioso;
    double *total_personas;
    double *arrepentidos_por_cola;
}t_estadisticas;

typedef struct resultados
{
    double *promedio_permenancia;
    double *promedio_espera;
    double *porcentaje_tiempo_ocioso;
    double *porcentaje_arrepentimiento;
}t_resultados;

///////////////////////////////////////////////

void iniciar_logger();
void cargar_confguracion(int*);
void realizar_simulacion(int);
t_resultados *inicializar_resultados();
void cargar_confguracion_ia();
void cargar_confguracion_ta();
void cargar_configuracion_fdp();

int obtener_puesto_menor_TPS(int*);
int seleccionar_puesto(int*);
unsigned long obtener_intervalo_entre_arribos();
void decidir_arrepentimiento(int, double*, bool*);
void inicializar_vector_nulo(void**);
void set_condiciones_iniciales(t_estadisticas*, t_eventos_futuros *);
unsigned int obtener_tiempo_atencion(int);
double fdp(int);
void procesar_llegada(t_eventos_futuros *,t_estadisticas *);
void procesar_salida(t_eventos_futuros *,t_estadisticas *,int);
void calcular_resultados(t_resultados*, t_estadisticas *);

double calcular_promedio_permanencia(int, t_estadisticas *, int);
double calcular_promedio_espera(int, t_estadisticas *, int);
double calcular_porcentaje_de_tiempo_ocioso(t_estadisticas *, int);
double calcular_porcentaje_arrepentimiento(t_estadisticas *, int);

void imprimir_resultados(t_resultados*);
void cargar_configuracion_fdp();

void liberar_memoria(t_resultados*,t_estadisticas*,t_eventos_futuros*);
void liberar_resultados(t_resultados *);
void liberar_estadisticas(t_estadisticas *);
void liberar_eventos_futuros(t_eventos_futuros *);
void liberar_memoria_globales();

///////////////////////////////////////////////

t_log *un_logger;
t_log *logger_trace;
bool hay_tracing_activo;
int cantidad_puestos;
int t;
int *num_elem;

t_dist_intervalo *distribucion_ia;
t_dist_intervalo *distribucion_ta;

///////////////////////////////////////////////

#endif //GENERADOR_VA_MAIN_H
