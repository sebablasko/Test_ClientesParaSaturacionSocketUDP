#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include "../ssocket/ssocket.h"

//Definiciones
#define BUF_SIZE 10
#define MAX_PACKS 100000
#define FIRST_PORT 1820

//Variables
int first_pack = 0;
struct timeval dateInicio, dateFin;
pthread_mutex_t lock;
int mostrarInfo = 0;
int NTHREADS = 1;

//Metodo para Threads
/*
* Recibe el valor del socket para enviar información
*/
llamadaHilo(int socket_fd){
	char buf[BUF_SIZE];
	int lectura;

	if(mostrarInfo) printf("Socket Operativo: %d\n", socket_fd);

	int i;
	int paquetesParaAtender = MAX_PACKS/NTHREADS;

	for(i = 0; i < paquetesParaAtender; i++) {
		//lectura = recv(socket_fd, buf, BUF_SIZE, 0);
		lectura = read(socket_fd, buf, BUF_SIZE);
		if(lectura <= 0) {
			fprintf(stderr, "Error en el read del socket (%d)\n", lectura);
			exit(1);
		}
		if(first_pack==0) { 
			pthread_mutex_lock(&lock);
			if(first_pack == 0) {
				if(mostrarInfo)	printf("got first pack\n");
				first_pack = 1;
				//Medir Inicio
				gettimeofday(&dateInicio, NULL);
			}
			pthread_mutex_unlock(&lock);
		}
	}
}

int main(int argc, char **argv){
	//Verificar Parametros Entrada
	if(argc <2){
		fprintf(stderr,"Syntax Error: Esperado: ./mtServer NTHREADS\n");
		exit(1);
	}

	//Recuperar Valores
	NTHREADS = atoi(argv[1]);
	if(mostrarInfo)	printf("Diseño: \t%d: Threads \n", NTHREADS);

	//Definir Variables
	int socket_fd;
	pthread_t pids[NTHREADS];
	int port = FIRST_PORT;
	double segundos;
	char ports[10];
	int i;

	if(mostrarInfo)	printf("Puertos Activados: \n");
	sprintf(ports, "%d", port);
	if(mostrarInfo)	printf("\t\t %s\n ", ports);
	socket_fd = udp_bind(ports);
	if(socket_fd < 0) {
		fprintf(stderr, "Error de bind al tomar el puerto\n");
		exit(1);
	}

	pthread_mutex_init(&lock, NULL);
	if(mostrarInfo)	printf("Usando %d threads, %d packs\n", NTHREADS, MAX_PACKS);

	//Lanzar Threads
	for(i=0; i < NTHREADS; i++) {
		pthread_create(&pids[i], NULL, llamadaHilo, socket_fd);
	}

	//Esperar Threads
	for(i=0; i < NTHREADS; i++) 
		pthread_join(pids[i], NULL);

	//Medir Fin
	gettimeofday(&dateFin, NULL);

	//Cerrar Sockets
	close(socket_fd);

	segundos=(dateFin.tv_sec*1.0+dateFin.tv_usec/1000000.)-(dateInicio.tv_sec*1.0+dateInicio.tv_usec/1000000.);
	if(mostrarInfo){
		printf("Tiempo Total = %g\n", segundos);
		printf("QPS = %g\n", MAX_PACKS*1.0/segundos);
	}else{
		printf("%g, \n", segundos);
	}
	exit(0);
}