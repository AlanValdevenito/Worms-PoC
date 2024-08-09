#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <iostream>
#include <string>
#include <vector>

#include "server_juego.h"
#include "server_protocolo.h"

class Servidor {
private:
    ProtocoloServidor protocolo;
    Juego juego;

public:
    Servidor(Socket&& socket, const std::string& archivo);

    int iniciar_juego();
    void actualizar_juego();

    Select recibir_seleccionar_escenario(bool* was_closed);
    Accion recibir_accion(bool* was_closed);

    int ejecutar_accion(const Accion& accion);

    void enviar_codigo(const int& codigo);
    void enviar_ubicacion(const Ubicacion& ubicacion);

    Ubicacion get_ubicacion();
    void imprimir_ubicacion(const Ubicacion& ubicacion);
};

#endif
