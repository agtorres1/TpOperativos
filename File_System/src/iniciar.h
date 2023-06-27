#ifndef INICIAR_H
#define INICIAR_H

#include <stdint.h> // pasarlo a todos los archivos
#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/string.h>
#include <commons/config.h>
#include <readline/readline.h>
#include <sockets.h>
#include <signal.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>

typedef struct{
	char* ip_memoria;
	char* puerto_memoria;
	char* puerto_escucha;
	char* superbloque; //path
	char* bitmap; //path
	char* bloques; //path
	char* fcb; //path
	uint32_t retardo_acceso_bloque;
} t_config_fs;

typedef struct {
    uint32_t block_size;
    uint32_t blocks;
} t_superbloque;

extern t_superbloque* superbloque;
extern t_config_fs *c;
uint32_t cargar_superbloque();

#endif