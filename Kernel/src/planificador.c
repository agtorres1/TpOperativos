#include "planificador.h"


t_queue* colaNew;
t_list* listaReady;
t_list* listaExe;
t_list* listaBlock;
t_list* listaExit;

pthread_mutex_t mutexNew;
pthread_mutex_t mutexReady;
pthread_mutex_t mutexBlock;
pthread_mutex_t mutexExe;
pthread_mutex_t mutexExit;

sem_t contadorNew;
sem_t contadorReady;
sem_t contadorExe;
sem_t contadorBlock;
//sem_t contadorProcesosEnMemoria;
sem_t multiprogramacion;
pthread_mutex_t multiprocesamiento;
sem_t largoPlazo;


void agregarAReady(pcb_t* pcb){ // Tendria mas sentido que log_kernel no sea pasado por parametro
	//log_trace(log_kernel,"Entre en agregar a ready");

	time_t a = time(NULL); //momento en el que entra un proceso, sirve para el HRRN
	pcb->horaDeIngresoAReady = a;
	pthread_mutex_lock(&mutexReady); //para el execute a ready y de blocked a ready y de new a ready

	tabla_segmentos* tseg;
	list_add(listaReady, pcb); //agrega a la listaReady

	log_info(log_kernel, "[READY] Entra el proceso de PID: %d a la cola.", pcb->PID);
	send_INICIAR_ESTRUCTURA_MEMORIA(conexion_memoria, "Inicializa las estructuras"); //mandamos mensaje a memoria que incie sus estructuras
	if(!recv_TABLA_SEGMENTOS(conexion_memoria, &tseg)){ //recibimos la direccion de la tabla de segmento
		log_info(log_kernel, "No se recibio tabla de segmentos para el proceso de PID: %d", pcb->PID);
	}
	pcb->TSegmento = *tseg;

	//printf("PROCESOS EN READY: %d \n", list_size(colaReady));
	log_debug(log_kernel,"[----------------PROCESOS EN READY: %d --------------------]\n", list_size(listaReady)); //max 4

	pthread_mutex_unlock(&mutexReady);
	sem_post(&contadorReady);
}

//estado Ready
void hiloNew_Ready(){

	while(1){
		sem_wait(&largoPlazo); //ver si sacarlo o no
		pcb_t* proceso = sacarDeNew();

		//proceso->estimacionAnterior = estimacion_inicial;
		//proceso->estimacionActual = estimacion_inicial;	//"estimacio_inicial" va a ser una variable que vamos a obtener del cfg
		sem_wait(&multiprogramacion); // poner sem_post en el sacar_ready
		//log_error(log_kernel,"[===========] agregue a ready desde hilo new ready");
		agregarAReady(proceso);

		proceso->estimacion_rafaga_anterior = proceso->estimacion_prox_rafaga;
	proceso->rafaga_anterior_real =proceso-> horaDeIngresoAExe - proceso-> horaDeSalidaDeExe;
	proceso->estimacion_prox_rafaga = (hrrn_alfa* proceso->rafaga_anterior_real)+ ((1+hrrn_alfa)* proceso-> estimacion_rafaga_anterior);

	time_t final_ready = time(NULL);
	proceso-> hrrn = (proceso->estimacion_prox_rafaga + (final_ready - proceso->horaDeIngresoAReady))/ proceso->estimacion_prox_rafaga;

	}

}
int tamanioDeColaReady(){

	int tamanio;

	pthread_mutex_lock(&mutexReady);
	tamanio = list_size(listaReady);
	pthread_mutex_unlock(&mutexReady);

	return tamanio;
}
pcb_t* obtener_siguiente_ready(){

//	log_trace(log_kernel,"Entre en obtener sig de ready");
	sem_wait(&contadorReady);

	pcb_t* procesoPlanificado = NULL;

	if (tamanioDeColaReady() > 0){

		// Aca dentro un SWITCH para los distintos algoritmos q llama a una funcion para cada uno
		switch(obtener_algoritmo_planificacion(algoritmo_planificacion)){
			//	log_trace(log_kernel,"EL ALGORITMO DE PLANIF ES: %d",algoritmo_config);
				case FIFO:
					procesoPlanificado = obtener_siguiente_FIFO();
				break;
				case HRRN:
					time_t b= time(NULL);
					//map(listaReady,b);
					procesoPlanificado = obtener_siguiente_HRRN(b); //calcular el hrrn segun el tiempo final y obtiene el de mayor RR
				break;
			  }


	}

	// Devuelve NULL si no hay nada en ready
	// Caso contrario devuelve el que tiene mas prioridad segun el algoritmo que se este empleando
	return procesoPlanificado;
}

void* hiloReady_Execute(){

	while(1){

		pthread_mutex_lock(&multiprocesamiento);
		pcb_t* pcb_siguiente = obtener_siguiente_ready();

}

}
pcb_t* obtener_siguiente_FIFO(){
	pcb_t* procesoPlanificado;

	pthread_mutex_lock(&mutexReady);
	procesoPlanificado = list_remove(listaReady, 0);
    pthread_mutex_unlock(&mutexReady);

	return procesoPlanificado;
}
 void calcularHRRN(t_list* listaReady, time_t tiempo_actual){
	pcb_t* pcb_proceso;
	int t = list_size(listaReady);
	for(int i=0;i<t;i++){
	pcb_proceso = list_get(listaReady,i);
	pcb_proceso->hrrn = (pcb_proceso->estimacion_prox_rafaga + (tiempo_actual - pcb_proceso->horaDeIngresoAReady))/ pcb_proceso->estimacion_prox_rafaga;
	}
	//list_find( mayor pcb_proceso-> hrrn)
}

pcb_t* obtener_siguiente_HRRN(time_t tiempo_actual){
	pcb_t* proceso_planificado= NULL;
	pthread_mutex_lock(&mutexReady);
	calcularHRRN(listaReady,tiempo_actual);
	//proceso_planificado = list_find(listaReady, )
    pthread_mutex_unlock(&mutexReady);

	return proceso_planificado;
}

void devuelve_id_menor_rr(t_list* listaReady, time_t tiempo_actual){
	// sacamos la cantidad de elemtnos de la lista
	// while (<= cant_elem_list)
	// tiempoActual - horaIngresoReady
	//
	// calculo hrrn pcbnuevo
	// calculo hrrn pcbviejo
	// if (pcbnuevo.hrrn<pcbmenor.hrrn)
	// guarda la posicion de la lista.
	//return posicion
	// list_remove(listaReady,posicon)
}



//procesoPlanificado->rafaga_anterior_real =procesoPlanificado-> horaDeIngresoAExe - procesoPlanificado-> horaDeSalidaDeExe;
	//procesoPlanificado->estimacion_prox_rafaga = (hrrn_alfa* procesoPlanificado->rafaga_anterior_real)+ ((1+hrrn_alfa)* procesoPlanificado-> estimacion_rafaga_anterior);
	//procesoPlanificado->estimacion_rafaga_anterior = procesoPlanificado->estimacion_prox_rafaga;
	//time_t final_ready = time(NULL);
	//procesoPlanificado-> hrrn = (procesoPlanificado->estimacion_prox_rafaga + (final_ready - procesoPlanificado->horaDeIngresoAReady))/ procesoPlanificado->estimacion_prox_rafaga;


