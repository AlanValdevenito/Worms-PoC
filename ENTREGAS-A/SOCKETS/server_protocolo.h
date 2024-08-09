#ifndef PROTOCOLO_SERVIDOR_H
#define PROTOCOLO_SERVIDOR_H

#include <string>
#include <vector>

#include "common_socket.h"

struct Ubicacion {
    // cppcheck-suppress unusedStructMember
    int x;
    // cppcheck-suppress unusedStructMember
    int y;
};

struct Select {
    // cppcheck-suppress unusedStructMember
    int tipo;
    std::string escenario;
};

struct Accion {
    // cppcheck-suppress unusedStructMember
    int tipo;
    // cppcheck-suppress unusedStructMember
    int direccion;
};

class ProtocoloServidor {
private:
    Socket peer;

public:
    explicit ProtocoloServidor(Socket&& peer);

    Select esperar_seleccionar_escenario(bool* was_closed);
    Accion esperar_accion(bool* was_closed);

    void enviar_codigo(const int& codigo);
    void enviar_ubicacion(const Ubicacion& ubicacion);
};

#endif
