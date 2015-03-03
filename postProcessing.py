def promedio(l):
	return reduce(lambda q,p: float(p)+float(q), l)/len(l)

import sys
filename = sys.argv[1]
archivo = open(filename, 'r')

finales = []

for line in archivo:
	parciales = []

	datos = line.split(";")
	nthreads = int(datos[0])
	nclients = len(datos)-1

	parciales.append(nthreads)
	for i in range(nclients):
		muestras  = datos[i+1].split(",")
		muestras = filter(lambda x: x!=" ", muestras)
		if(len(muestras)>1):
			parciales.append(promedio(muestras))

	finales.append(parciales)


salida = open("Results.csv", "w+")

salida.write(";")
for j in range(nclients-1):
	salida.write(str(j+1)+";")
salida.write("\n")

for l in finales:
	for i in l:
		salida.write(str(i)+";")
	salida.write("\n")