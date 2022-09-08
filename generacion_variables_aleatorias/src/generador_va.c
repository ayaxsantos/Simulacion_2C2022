#include "../include/generador_va.h"

//////////////////////////////////////////////

int main(int argc, char* argv[])
{
    unsigned long long cantidad_de_iteraciones = atoi(argv[1]);
    iniciar_logger();
    generar_valores(cantidad_de_iteraciones);
    log_destroy(un_logger);
    return EXIT_SUCCESS;
}

void generar_valores(unsigned long long cantidad_de_iteraciones)
{
    double valor_aleatorio;
    int valor_generado;
    unsigned int supremo = 0, infimo = 0;
    cargar_confguracion(&supremo,&infimo);
    int *resultados = inicializar_resultados(supremo);

    srand(time(NULL));
    loggear_funcion(supremo, infimo);
    log_info(un_logger, "Comenzando generacion...");

    //Se require mucho performance en este bucle!!
    for (unsigned long long i = 0; i < cantidad_de_iteraciones; i++)
    {
        // x = a + (b – a) R
        valor_aleatorio = (double) rand() / RAND_MAX;
        valor_generado = infimo + ((supremo - infimo) * valor_aleatorio);
        resultados[valor_generado] += 1;
    }

    for (int i = 20; i < supremo; i++)
    {
        log_info(un_logger, "Valor generado: %i | Cantidad de veces: %d", i, resultados[i]);
    }

    free(resultados);
}

void iniciar_logger()
{
    t_log_level nivel_log = LOG_LEVEL_INFO;
    un_logger = log_create("./generador_va.log","generador_va",true,nivel_log);

    if(un_logger == NULL)
    {
        write(0,"ERROR -> No se pudo crear el log \n",40);
        exit(EXIT_FAILURE);
    }
    log_info(un_logger,"--- GENERADOR DE V.A. | DISTRIBUCION UNIFORME | METODO FUNCION INVERSA ---");
}

void loggear_funcion(int maximo, int minimo)
{
    char *numerador = "        1 ";
    char *barra_cociente = "f(x) = ----";

    if(maximo > 1000)
    {
        numerador = "         1 ";
        barra_cociente = "f(x) = ------";
    }

    log_info(un_logger,"La funcion ingresada es: \n");
    log_info(un_logger,"%s",numerador);
    log_info(un_logger,"%s",barra_cociente);
    log_info(un_logger,"        %d \n",maximo-minimo);
}

void cargar_confguracion(unsigned int *supremo, unsigned int *infimo)
{
    t_config *una_config = config_create("./valores.config");

    *supremo = config_get_int_value(una_config, "SUPREMO");
    *infimo = config_get_int_value(una_config, "INFIMO");

    config_destroy(una_config);
}

int* inicializar_resultados(int supremo)
{
    int *resultados = malloc(sizeof(int) * (supremo+1));

    for (int i = 0; i <= supremo; i++)
        resultados[i] = 0;

    return resultados;
}

//////////////////////////////////////////////
