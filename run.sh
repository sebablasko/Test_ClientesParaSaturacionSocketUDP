#!/bin/bash

# Variables
#	serverThreads: número de Threads que correrán en la instancia del servidor para procesar los paquetes enviados por el cliente
#	clients: número de instancias del programa cliente que se ejecutarán para saturar el servidor
#	repetitions: cantidad de repeticiones de la prueba
#
# salida=threads_sockets_relation.csv
#
# make all
#
# for serverThreads in {1..4}
# do
# 	linea=$serverThreads";"
# 	for clients in {1..5}
# 	do
# 		echo $serverThreads" threads en servidor"
# 		echo $clients" clientes corriendo"
#
# 		repetitions=3
# 		for ((i=1 ; $i<=$repetitions ; i++))
# 		{
# 			./server $serverThreads >> aux &
# 			pid=$!
# 			sleep 1
#
# 			for ((j=1 ; $j<=$clients ; j++))
# 			{
# 				./client 127.0.0.1 > /dev/null &
# 			}
# 			wait $pid
# 			linea="$linea$(cat aux)"
# 			rm aux
# 		}
# 		linea="$linea;"
# 	done
# 	echo "$linea" >> $salida
# done
#
# make clean
#
# python postProcessing.py $salida





echo "Compilando..."
make all
echo "Done"

MAX_PACKS=1000000
num_port=1820
repetitions=50
total_num_threads_per_socket="1 2 4 6 8 10 12 14 16 18 20 22 24 30 36 42 48 56 60"
total_clients="1 2 3 4 5 6 7 8 9 10 11 12"

echo "Iniciando Prueba..."
salida=threads_sockets_relation.csv

for num_threads_per_socket in $total_num_threads_per_socket
do
	linea=$num_threads_per_socket"; "

	for num_clients in $total_clients
	do
		echo "ServerThreads: "$num_threads_per_socket", Clientes: "$num_clients

		for ((i=1 ; $i<=$repetitions ; i++))
		{
			echo "Repeticion $i"

			./serverTesis --packets $MAX_PACKS --threads $num_threads_per_socket --port $num_port >> aux &
			sleep 1

			for ((j=1; $j<=$num_clients; j++))
			{
				./clientTesis --packets "$(($MAX_PACKS*10))" --ip 127.0.0.1 --port $num_port > /dev/null &
			}

			wait $(pgrep 'serverTesis')

			linea="$linea$(cat aux)"
			rm aux
		}
		linea="$linea;"
	done

	echo "$linea" >> $salida
	echo ""
done

make clean

python postProcessing.py $salida $total_clients | sed 's/\./,/g' > "fix_"$salida
