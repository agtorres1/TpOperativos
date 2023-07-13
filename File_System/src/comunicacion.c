#include "comunicacion.h"
extern t_log* logger;
extern t_config_fs *c;
extern int fd_fs;
//extern


void procesar_peticiones(int cliente_socket){
	op_code cop;
	while(cliente_socket!=-1){
		if(recv(cliente_socket,&cop,sizeof(op_code),0) == 0){
			log_info(logger,"DISCONECT!");
				return;  }


				switch(cop){
				// DEFINIR: ver como pasar los datos nombre archivo y tamanio porque despues lo usamos en config_set_value que sólo usa punteros

				//caso de F_OPEN primero hay que ver si ya existe el archivo
				case EXISTE_ARCHIVO:
					extra_code estado;
					char*nombre_archivo;
					uint32_t tamanio; // DEFINIR ver si cambiarlo a char* o castear
					if(!recv_EXISTE_ARCHIVO(cliente_socket,&nombre_archivo)) log_error(logger, "Fallo recibiendo f_open de kernel");

					log_info(logger,"abrir archivo %s",nombre_archivo);
					bool resultado = existe_y_abrir(nombre_archivo);
					if(!resultado){
						estado = INCORRECTO;
						send_OK_CODE(cliente_socket, estado);
						//crear el arcchivo.
						recv_CREAR_ARCHIVO(cliente_socket, &nombre_archivo, &tamanio); //para mi que no deberia recibir nada

						if(crear_archivo(nombre_archivo,tamanio)){
							estado = CORRECTO;
							send_OK_CODE(cliente_socket,estado);
						}
					}
					else{
						estado = CORRECTO;
						send_OK_CODE(cliente_socket, estado);
					}

				break;


				case F_WRITE:
					char* nombre_archivow;
				    uint32_t dfw; // la DF
					uint32_t cbw; // cant bytes
					char*contenido;
					uint32_t punterow = 0;
					uint32_t estadok;

					recv_F_WRITE(cliente_socket,&nombre_archivow,&dfw,&cbw);

					log_info(logger,"Escribir Archivo: <%s> - Puntero: <%d> - Memoria <%d> - Tamanio: <%d>",nombre_archivow,punterow,dfw,cbw);

					//solicito a memoria lo que hay en la direccion logica mandada
					send_READ2(fd_memoria, dfw);
					//aca recibo el contenido que le pediu a memoria
					recv_contenido_leido(fd_memoria,&contenido);
					//recibo un puntero?

					if(escribir_contenido(contenido,punterow)){ //escribir_contenido INCOMPLETA
						estadok = EXITOSO; //escribio bien
						send_OK_CODE(cliente_socket,estadok);
					}else {
						estadok= FALLIDO; //escribio mal
						send_OK_CODE(cliente_socket,estadok);
					}

					   break;
				case F_READ:
					uint32_t estado_memoria;
					uint32_t estado_kernel;
					char* nombre_archivor;
					uint32_t df;
					uint32_t cb; //cant bytes
					uint32_t puntero=0;
					recv_F_READ(cliente_socket,&nombre_archivor,&df,&cb);

					log_info(logger,"Leer: Archivo: %s - Puntero: %d  - Memoria: <%d>  - Tamanio: <%d>",nombre_archivor,puntero,df,cb);
					char*contenidor = buscar_contenido(puntero,cb); //TODO
					//le mando a memoria lo que tiene que escribir
					send_WRITE(fd_memoria,df,contenidor);
					recv_OK_CODE(fd_memoria,&estado_memoria);
					if(estado_memoria == EXITOSO){
						estado_kernel = EXITOSO;
						send_OK_CODE(cliente_socket,estado_kernel);
					}else{
						estado_kernel = FALLIDO;
						send_OK_CODE(cliente_socket,estado_kernel);
					}

						break;

				case F_TRUNCATE:
					char* nombre_archivo_truncate; //TODO DUDAS como es una variable deberia ser una general en vez de hacer varias para cada case
					uint32_t tamanio_truncate;
					recv_F_TRUNCATE(cliente_socket, &nombre_archivo, &tamanio);

					//uso funcion concat para obtener el path y asi usar c
					//TODO DUDAS no se como llegar al FCB

					// var pasada por parametro: el nuevo tamanio
					// a partir de ahí ver si es más chico o más grande y hacer lo necesario:
						//TODO TRUNCATE: Actualizar el tamanio del archivo en FCB
						// Si es mas chico que el de antes:
							//Marcar libres los bloques (no hace falta borrar el contenido)
						// Si es más grande que el de antes:
							//TODO Asignar nuevos bloques (ver que significa)
						break;

				}

}
}


int generar_conexion_con_memoria(){
	int conexion = crear_conexion(logger,"Memoria",c->ip_memoria, c->puerto_memoria);
	uint8_t handshake =1;
	uint8_t result;
	send_handshake(conexion,handshake);
	recv_handshake(conexion,&result);
	if(result == 1) log_info(logger,"todo ok capo");
	return conexion;
}


void conexion_kernel(){
	int fd_kernel = esperar_cliente(logger, fd_fs);
		if(fd_kernel!=-1){
			procesar_peticiones(fd_kernel);
		}
}












