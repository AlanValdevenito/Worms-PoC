# Worms PoC

Antes de comenzar el desarrollo del proyecto grupal [Worms](https://github.com/AlanValdevenito/Worms) para la materia [Taller de Programacion I,](https://taller-de-programacion.github.io/bienvenido.html) se realizaron dos pruebas de concepto (PoC) individuales en C++ con el objetivo de experimentar y comprender en profundidad las tecnologías centrales del trabajo final:
1. Multithreading
2. Sockets y networking

De esta forma nos familiarizamos con estas tecnologias antes de embarcarnos a desarrollar el TP real con el objetivo de tomar mejores decisiones luego.

## TP1: Sockets

Se puede acceder al [codigo](https://github.com/AlanValdevenito/Taller-De-Programacion-I/tree/main/ENTREGAS/SOCKETS) y al [enunciado](https://github.com/AlanValdevenito/Worms-PoC/blob/main/ENUNCIADOS/TP1-SOCKETS.pdf) de esta PoC.

El objetivo de esta PoC era familiarizarse con:
1. Encapsulación de sockets en C++
2. Diseño de un protocolo binario
3. Separación en capas (network / protocol / game / UI)
4. Uso correcto de [`RAII`](https://en.cppreference.com/w/cpp/language/raii.html) y la librería estándar [`STL`](https://en.cppreference.com/w/cpp/standard_library.html) de `C++`
5. Comunicación cliente-servidor bloqueante

En esta PoC el servidor atiende un único cliente.

Se trabajó con separación estricta de responsabilidades:
1. `Socket`: Encapsulación del file descriptor
2. `Protocolo`: Serialización/deserialización
3. `Game`: Lógica del gusano
4. `Client`/`Server`: Orquestación

Cumple con las restricciones técnicas:
1. `C++17`
2. `POSIX 2008`
3. Sin variables globales, funciones globales y goto
4. Sockets bloqueantes
5. Debe haber una clase Socket tanto el socket aceptador como el socket usado para la comunicación.
6. Debe haber una clase Protocolo que encapsule la serialización y deserialización de los mensajes entre el cliente y el servidor. 
7. Uso de `RAII`
8. Uso de `STL`

### Descripción funcional

Se implementa un cliente que simula comandos de un jugador desde `stdin`:
1. `select <scenario>`
2. `dir <direction>`
3. `move`
4. `jump <type>`

El servidor:
1. Carga escenarios desde archivo
2. Emula la lógica del gusano (posición entera x,y)
3. Valida colisiones
4. Simula gravedad
5. Devuelve siempre la posición final calculada
6. El cliente imprime la posición recibida

### Protocolo binario

Se implementó un protocolo binario explícito con serialización manual.

Cliente → Servidor
1. `01 <L> <N>`: Seleccionar y crear una nueva partida con el escenario pedido. El campo <L> determina la longitud del nombre <N>; <L> es un entero de 16 bits sin signo en big endian y <N> es una secuencia de bytes sin el \0.
2. `03 <D>`:  Cambiar de dirección en la que mira el gusano. El campo <D> es un entero de 8 bits sin signo. Vale 00 si la nueva dirección es a la izquierda o 01 si es a la derecha.
3. `04`: Mover el gusano
4. `05 <T>`: Realizar un salto. El campo <T> es un entero de 8 bits sin signo. Vale 00 si el salto es hacia adelante o 01 si el salto es hacia atrás.

Servidor → Cliente
1. `00 <X> <Y>`: Código de éxito al seleccionar y crear una nueva partida, seguido de la posición inicial del gusano. Tanto <X> como <Y> son enteros de 32 bits sin signo en big endian.
2. `01`: Código de error al seleccionar y crear una nueva partida (el escenario no fue encontrado).
3. `<X> <Y>`: Respuesta a todos los casos. El servidor responde con la posición final del gusano. Tanto <X> como <Y> son enteros de 32 bits sin signo en big endian.

### Ejecución

Cliente: `./client <ip/hostname server> <puerto/servicename>`

Servidor: `./server <puerto/servicename> <escenarios>`

El archivo `<escenarios>` tiene la definición de los escenarios que el cliente podrá elegir y crear una nueva
partida con él.

## TP2: Threads

Se puede acceder al [codigo](https://github.com/AlanValdevenito/Taller-De-Programacion-I/tree/main/ENTREGAS/THREADS) y al [enunciado](https://github.com/AlanValdevenito/Worms-PoC/blob/main/ENUNCIADOS/TP2-THREADS.pdf) de esta PoC.

El objetivo de esta PoC era familiarizarse con:
1. Multithreading en C++
2. Race conditions
3. Sincronización con mutex (`std::mutex`) y condition variables (`std::condition_variable`)
4. Uso de monitores
5. Thread-safe queues
6. RAII aplicado a threads

El foco no fue la lógica del juego sino el modelo concurrente del servidor.

### Descripción funcional

Se implementa un servidor tipo lobby:
1. Múltiples clientes pueden conectarse
2. Cada cliente puede enviar mensajes de chat
3. El servidor realiza broadcast a todos
4. Se notifica la cantidad de jugadores conectados

Cada vez que un jugador se una o se retire del lobby, el servidor le envia un mensaje a todos los clientes conectados con la cantidad total de jugadores en el lobby. Además, cada jugador podrá enviar mensajes de chat: el servidor deberá obtenerlos y enviarlos a todos los clientes conectados, sean jugadores o espectadores. Es un broadcast. Tanto el servidor como los clientes deberán imprimirlos.

Mensajes posibles:
1. `Jugadores <N>, esperando al resto de tus amigos...`
2. `<chatmsg>`

### Modelo de concurrencia

El servidor crea:
1. Dos threads por cliente:
a. Receiver thread
b. Sender thread
2. Un thread aceptador (lobby)
3. Thread principal

Dado que los mensajes de chat de un jugador son enviados al resto de los clientes y que el servidor también envia mensajes a todos los clientes, esto impone una race condition sobre los sockets (múltiples threads quieren enviar usando el mismo socket). Por ello el servidor tiene una thread-safe queue por cada thread del cliente que se dedique a enviar mensajes hacia el cliente (outgoing queues).

Las race conditions sobre los sockets no son las únicas: como también se permite que los clientes se unan y retiren del lobby en distintos tiempos, hay que agregar o remover la queue de cada cliente de “la lista de queues” del lobby y como esta está compartida entre threads se protege con un monitor (hay múltiples threads recorriendo esa lista para hacer un broadcast).

En la solución implementada:
1. Se tene una thread-safe queue por cliente
1. El sender thread consume de su propia queue
3. Un monitor protege la estructura compartida de queues
4. Uso de:
a. `std::mutex`
b. s`td::condition_variable`
d. Colas bloqueantes

Se evita:
1. Uso de `sleep()` para sincronizar
2. Deadlocks
3. Race conditions

### Protocolo binario

Cliente → Servidor:
1. `0x05 <len> <chatmsg>`: El campo `0x05` es un byte con el número literal `0x05`, `<chatmsg length>` son 2 bytes sin signo en big endian con la longitud del mensaje y `<chatmsg>` es el mensaje sin el salto de línea sin el `\0` al final.

Servidor → Cliente:
1. `0x06 <player_cnt>`: El campo `0x06` es un byte con el número literal `0x06` y `<player cnt>` es un campo de 1 byte sin signo con la cantidad de jugadores actualmente unidos a la partida y esperando en el lobby.
2. `0x09 <len> <chatmsg>`: Es el mensaje de chat que un cliente envió al servidor y este lo está reenviando a todos los jugadores.

### Ejecución

Cliente: `./client <ip/hostname server> <puerto/servicename>`

Servidor: `./server <puerto/servicename>`

### Cierre del servidor

El servidor lee de la entrada estándar a la espera de leer la letra `q` que le indica que debe finalizar cerrando todos los sockets, queues y joinenando todos los threads sin enviar ningún mensaje adicional ni imprimir por salida estándar.

El servidor:
1. Lee `q` desde stdin
2. Cierra sockets
3. Finaliza queues
4. Hace `join()` de todos los threads
5. Libera recursos correctamente (`RAII`)
