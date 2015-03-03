#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include "../ssocket/ssocket.h"

//Definiciones
#define BUF_SIZE 10
#define MAX_PACKS 1000000
#define PORT_NUM 1820

//Variables
int NTHREADS = 1;
int first_pack = 0;
struct timeval dateInicio, dateFin;
pthread_mutex_t lock;
char buf[BUF_SIZE];
char* IP_DEST;
int mostrarInfo = 0;
double segundos;

//Metodo para hilos
llamadaHilo(int socket_fd){
	int lectura;

	if(mostrarInfo)
		printf("Socket Operativo: %d\n", socket_fd);

	int i;
	int paquetesPorEnviar = MAX_PACKS;

	for(i = 0; i < paquetesPorEnviar; i++){
		if(write(socket_fd, buf, BUF_SIZE) != BUF_SIZE) {
			gettimeofday(&dateFin, NULL);
			segundos = (dateFin.tv_sec+dateFin.tv_usec/1000000.)-(dateInicio.tv_sec*1.0+dateInicio.tv_usec/1000000.);
			fprintf(stderr, "Falla el write al servidor, envio %d paquetes\n", i);
			fprintf(stderr, "total time = %g\n", segundos);
			break;
		}
		if(first_pack==0) { 
			pthread_mutex_lock(&lock);
			if(first_pack == 0) {
				if(mostrarInfo) printf("got first pack\n");
				first_pack = 1;
				//Medir Inicio
				gettimeofday(&dateInicio, NULL);
			}
			pthread_mutex_unlock(&lock);
		}
	}
}

main(int argc, char **argv) {

	if(argc < 2){
		fprintf(stderr, "Syntax Error: Esperado: ./client IP_DEST\n");
		exit(1);
	}

	IP_DEST = argv[1];

	//Variables
	int socket_fd;
	pthread_t pids;
	char port_number[10];
	int i;

	/* Llenar de datos el buffer a enviar */
	for(i=0; i < BUF_SIZE; i++)
		buf[i] = 'a'+i;

	if(mostrarInfo)	printf("Puertos Activados: \n");
	sprintf(port_number, "%d", PORT_NUM);
	if(mostrarInfo)	printf("\t\t %s\n ", port_number);
	socket_fd = udp_connect(IP_DEST, port_number);
	if(socket_fd < 0) {
		fprintf(stderr, "connection refused\n");
		exit(1);
	}

	pthread_mutex_init(&lock, NULL);

	//Lanzar Threads
	pthread_create(&pids, NULL, llamadaHilo, socket_fd);

	//Esperar Threads
	pthread_join(pids, NULL);

	//Medir Fin
	gettimeofday(&dateFin, NULL);

	segundos=(dateFin.tv_sec*1.0+dateFin.tv_usec/1000000.)-(dateInicio.tv_sec*1.0+dateInicio.tv_usec/1000000.);
	if(mostrarInfo){
		printf("Tiempo Total = %g\n", segundos);
		printf("QPS = %g\n", MAX_PACKS*1.0/segundos);
	}else{
		printf("%g \n", segundos);
	}
	exit(0);
}