#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <iostream>
#include <string>
#include <vector>

#include "client_protocolo.h"

class Cliente {
private:
    ProtocoloCliente protocolo;

public:
    explicit Cliente(Socket&& socket);

    int iniciar_juego();
    int jugar();

    int enviar_seleccionar_escenario(const std::string& accion);
    int enviar_accion(const std::string& accion);

    int recibir_codigo();
    Ubicacion recibir_ubicacion();

    void imprimir_ubicacion(const Ubicacion& ubicacion);
};

#endif
