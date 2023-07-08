#ifndef INSTRUCCIONES_H
#define INSTRUCCIONES_H

#include <stdint.h> // pasarlo a todos los archivos
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <readline/readline.h>
#include <sockets.h>

#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include "send_rcvs.h"
#include "estructuras_compartidas.h"
#include "configuraciones.h"
#include "ejecutar_instrucciones.h"

typedef struct{
	uint32_t PID;
	uint32_t PC;
	t_list* instrucciones;
	registros_cpu registros;
	t_list* TSegmento;
}pcb_cpu;

extern t_list* lista_pcb;


void cargar_instruccion_a_lista(int, op_code, t_list*, t_log*);
int verificacion_recibo_code_correctamente(int, t_log*, op_code*);


void cargar_instruccion1(int, char* , uint32_t, uint32_t ,uint32_t ,t_list* );
void cargar_instruccion2(int, char* , uint32_t, char* ,uint32_t ,t_list* );
void cargar_instruccion3(int, char* , char*, uint32_t ,uint32_t ,t_list* );

instruccion* fetch();
int decode_execute(int, pcb_cpu*, instruccion*, t_log*);



#endif
