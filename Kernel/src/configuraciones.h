#ifndef SRC_CONFIGURACIONES_H_
#define SRC_CONFIGURACIONES_H_
#include "inttypes.h"
#include <stdlib.h>
#include <stdio.h>
#include <commons/config.h>
#include <commons/collections/list.h>

typedef struct{
	char* nombre;
	int instancia;
}recurso_sistema;

//typedef enum{
//	FIFO,
//	HRRN
//}algoritmo_planificacion;

extern char* ip;
extern char* puerto_escucha;
extern char* ip_cpu;
extern char* puerto_cpu;
extern char* ip_memoria;
extern char* puerto_memoria;
extern char* ip_fileSystem;
extern char* puerto_fileSystem;
extern char* algoritmo_planificacion;
extern uint32_t estimacion_inicial;
extern float hrrn_alfa;
extern uint32_t grado_max_multiprogramacion;
extern t_list* lista_recursos;


void validar_alfa(float);
t_list* obtener_recursos(t_config*, char*, char*);
#endif /* SRC_CONFIGURACIONES_H_ */
