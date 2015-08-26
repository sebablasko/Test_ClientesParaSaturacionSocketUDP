import sys
import numpy as np

# Reune los datos de un CSV con la estructura:
#   nThreads{0}; nClientes{0}_rep1, ..., nClientes{0}_repN; nClientes{1}_rep1, ..., nClientes{1}_repN;...; nClientes{M}_rep1, ..., nClientes{M}_repN;
#   nThreads{1}; nClientes{0}_rep1, ..., nClientes{0}_repN; nClientes{1}_rep1, ..., nClientes{1}_repN;...; nClientes{M}_rep1, ..., nClientes{M}_repN;
#   ...
#   nThreads{K}; nClientes{0}_rep1, ..., nClientes{0}_repN; nClientes{1}_rep1, ..., nClientes{1}_repN;...; nClientes{M}_rep1, ..., nClientes{M}_repN;
#
# Retornando un CSV con la estructura:
#              , 0               , 1               , ..., M
#   nThreads{0}, AVG_nClientes{0}, AVG_nClientes{1}, ..., AVG_nClientes{M}
#   nThreads{1}, AVG_nClientes{0}, AVG_nClientes{1}, ..., AVG_nClientes{M}
#   ...
#   nThreads{k}, AVG_nClientes{0}, AVG_nClientes{1}, ..., AVG_nClientes{M}


def parseRepetition(line):
	line = [v.strip() for v in line.split(",")]
	line = [float(v) for v in line if v != ""]
	#print line, np.average(line)
	return np.average(line)

if(len(sys.argv)<3):
	print "Error. Uso: ./postProcessing.py archivoAProcesar <Lista cantidad de Clientes lanzados>"
	exit()

filename = sys.argv[1]
#num_clientes = [int(v) for v in sys.argv[2:]]
num_clientes = sys.argv[2:]

archivo = open(filename, 'r')

resultadosFinales = []

resultadosFinales.append(['']+num_clientes)

for line in archivo:
	resultadosParciales = []

 	datos = line.split(";")
	resultadosParciales.append(datos[0])
	for i in range(1, len(datos)-1):
		resultadosParciales.append(parseRepetition(datos[i]))

	resultadosFinales.append(resultadosParciales)

# imprimir salida para csv
for line in resultadosFinales:
	for value in line:
		print value, ";",
	print ""
