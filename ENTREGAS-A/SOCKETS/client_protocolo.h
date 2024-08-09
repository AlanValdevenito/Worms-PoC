#ifndef PROTOCOLO_H
#define PROTOCOLO_H

#include <cstring>
#include <string>
#include <utility>
#include <vector>

#include "common_socket.h"

struct Ubicacion {
    // cppcheck-suppress unusedStructMember
    int x;
    // cppcheck-suppress unusedStructMember
    int y;
};

class ProtocoloCliente {
private:
    Socket skt;

public:
    explicit ProtocoloCliente(Socket&& socket);

    int esperar_codigo();
    Ubicacion esperar_ubicacion();

    void seleccionar_escenario(const std::string& escenario);
    void cambiar_direccion_gusano(const int& direccion);
    void mover_gusano();
    void saltar_gusano(const int& tipo);
};

#endif
