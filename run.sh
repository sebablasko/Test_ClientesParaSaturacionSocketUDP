#!/bin/bash

# Variables
#	serverThreads: número de Threads que correrán en la instancia del servidor para procesar los paquetes enviados por el cliente
#	clients: número de instancias del programa cliente que se ejecutarán para saturar el servidor
#	repetitions: cantidad de repeticiones de la prueba

salida=threads_sockets_relation.csv

make all

for serverThreads in {1..4}
do
	linea=$serverThreads";"
	for clients in {1..5}
	do
		echo $serverThreads" threads en servidor"
		echo $clients" clientes corriendo"

		repetitions=3
		for ((i=1 ; $i<=$repetitions ; i++))
		{
			./server $serverThreads >> aux &
			pid=$!
			sleep 1

			for ((j=1 ; $j<=$clients ; j++))
			{
				./client 127.0.0.1 > /dev/null &
			}
			wait $pid
			linea="$linea$(cat aux)"
			rm aux
		}
		linea="$linea;"
	done
	echo "$linea" >> $salida
done

python postProcessing.py $salida