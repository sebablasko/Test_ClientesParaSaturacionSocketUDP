# Test_ClientesParaSaturacionSocketUDP
Prueba para determinar el número de clientes que se necesitan para conseguir un escenario de saturación en un socket UDP, es decir, busca el ratio entre: #clientes vs. #threadsServidor(que procesan en un puerto). Busca responder la pregunta: Para sockets UDP con 1 .. N Threads corriendo en el servidor, ¿Cuantos clientes necesito para lograr un escenario de saturación?
