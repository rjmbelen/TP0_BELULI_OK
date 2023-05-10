#include "client.h"

typedef struct {
    uint32_t dni;
    uint8_t edad;
    uint32_t pasaporte;
    char nombre[14];
} t_persona;



int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/
t_persona p1;
p1.dni=35478921;
p1.edad=25;
p1.pasaporte=3547899322;
strcpy(p1.nombre,"John Doe");
 printf("PRUEBA IMPRIMIR VALOR DE UN STRUCT dni %d \n", p1.dni);



int un_nro; // Variable entera
int *int_ptr; // Puntero a entero
un_nro = 2;	


int_ptr = &un_nro;
printf("int_ptr=%p  la posición de memoria de la variable un_nro \n",int_ptr); // Por pantalla se visualizará la posición de memoria de la variable un_nro, dado que eso apunta nuestro puntero.
printf("*int_ptr=%d  valor 2 \n",*int_ptr); // Por pantalla se visualizará "lo que tiene dentro" nuestro puntero, es decir, el valor 2
printf("&int_ptr=%p direccion de memoria del puntero\n",&int_ptr); // Por pantalla se visualizará la dirección de memoria de nuestro puntero
	/* ---------------- LOGGING ---------------- */

	t_log* logger = iniciar_logger();
	if(logger == NULL){
		perror("ERROR: en la creacion del logger");
		exit(-1);
	}

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	char * palabra = "CLIENTE";
	log_info(logger,"Hola soy un Log");
	log_info(logger, "¡ BIENVENIDOS AL LOG DE %s !", palabra);

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */
	t_config*  config = iniciar_config();

	if(config == NULL){
		log_error (logger,"ERROR = en la creacion del archivo config.");
		exit(-2);
	}

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	char* valor = config_get_string_value(config,"CLAVE");
	char* ip = config_get_string_value(config,"IP");
	char* puerto = config_get_string_value(config,"PUERTO");

	// Loggeamos el valor de config
	//log_info(logger,"La clave es:%s",valor);
	//log_info(logger, "El puerto es:%s", puerto);
	//log_info(logger,"La IP es:%s" ,ip);

	log_info(logger,
			"\nLa clave es:%s \n" 
			"El puerto es:%s \n"
			"La IP es:%s \n",
			valor, puerto,ip);   //mas pro, todo en 1!

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	int conexion = crear_conexion(ip, puerto); //IP Y PUERTO DEL SERVIDOR QUE SACO DE CONFIG

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor,conexion);
	log_info(logger,"Se envio el siguiente mensaje al Servidor:%s",
			valor); 
	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("tp0.log","LOG_CLIENTE_TP0",true,LOG_LEVEL_INFO);
	//nuevo_logger = NULL; TEST PARA VERIFICAR ERROR EN CREACION DE LOG
	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("../cliente.config");
	// t_config* nuevo_config = config_create("../cliente.config123232332"); TEST PARA VERIFICAR ERRO CREACION CONFIG

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");
	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while(strcmp(leido,"")){
		log_info(logger,leido);
		free(leido);
		leido = readline("> ");
	}
	free(leido);

	// ¡No te olvides de liberar las lineas antes de regresar!

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido = readline("CONSOLA_PAQUETE > ");
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	while(strcmp(leido,"")){
		agregar_a_paquete(paquete,leido, strlen(leido)+1);
		free(leido);
		leido = readline("CONSOLA_PAQUETE > ");
	}
	
	//no te olvideses de liberar las líneas y el paquete antes de regresar!
	free(leido);
	enviar_paquete(paquete, conexion);
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	//
	if(logger !=NULL){
		log_destroy(logger);
	}

	if(config != NULL){
		config_destroy(config);
	}

	liberar_conexion(conexion);

}
