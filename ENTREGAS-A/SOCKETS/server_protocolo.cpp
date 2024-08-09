#include "server_protocolo.h"

#include <cstring>
#include <utility>

#include <arpa/inet.h>

#define SELECT 1
#define DIR 3
#define MOVE 4
#define JUMP 5

#define BYTES_ACCION 1
#define BYTES_L 2
#define BYTES_D 1
#define BYTES_T 1

ProtocoloServidor::ProtocoloServidor(Socket&& peer): peer(std::move(peer)) {}

Select ProtocoloServidor::esperar_seleccionar_escenario(bool* was_closed) {
    Select select;

    char buffer_accion[BYTES_ACCION] = {0};
    peer.recvall(buffer_accion, sizeof(buffer_accion), was_closed);

    uint8_t byteAccion;
    memcpy(&byteAccion, buffer_accion, sizeof(byteAccion));
    select.tipo = static_cast<int>(byteAccion);

    char buffer_longitud[BYTES_L] = {0};
    peer.recvall(buffer_longitud, sizeof(buffer_longitud), was_closed);

    uint16_t longitud_net;
    memcpy(&longitud_net, buffer_longitud, sizeof(longitud_net));
    int longitud = ntohs(longitud_net);

    std::vector<char> buffer_escenario(longitud, 0);
    peer.recvall(buffer_escenario.data(), buffer_escenario.size(), was_closed);
    select.escenario.assign(buffer_escenario.begin(), buffer_escenario.end());

    return select;
}

Accion ProtocoloServidor::esperar_accion(bool* was_closed) {
    Accion accion = {0, 0};

    char buffer_accion[BYTES_ACCION] = {0};
    peer.recvall(buffer_accion, sizeof(buffer_accion), was_closed);

    uint8_t byteAccion;
    memcpy(&byteAccion, buffer_accion, sizeof(byteAccion));
    accion.tipo = static_cast<int>(byteAccion);

    if (accion.tipo == DIR) {
        char buffer_direccion[BYTES_D] = {0};
        peer.recvall(buffer_direccion, sizeof(buffer_direccion), was_closed);
        int direccion = static_cast<int>(buffer_direccion[0]);

        accion.direccion = direccion;

    } else if (accion.tipo == JUMP) {
        char buffer_tipo[BYTES_T] = {0};
        peer.recvall(buffer_tipo, sizeof(buffer_tipo), was_closed);
        int tipo = static_cast<int>(buffer_tipo[0]);

        accion.direccion = tipo;
    }

    return accion;
}

void ProtocoloServidor::enviar_codigo(const int& codigo) {
    bool was_closed = false;
    std::vector<uint8_t> bytes;

    uint8_t byteCodigo = static_cast<uint8_t>(codigo);
    bytes.resize(sizeof(byteCodigo));
    memcpy(bytes.data(), &byteCodigo, sizeof(byteCodigo));

    peer.sendall(bytes.data(), bytes.size(), &was_closed);
}

void ProtocoloServidor::enviar_ubicacion(const Ubicacion& ubicacion) {
    bool was_closed = false;
    std::vector<uint8_t> bytes;

    uint32_t posicionY = static_cast<uint32_t>(ubicacion.y);
    uint32_t posicionY_net = htonl(posicionY);

    uint32_t posicionX = static_cast<uint32_t>(ubicacion.x);
    uint32_t posicionX_net = htonl(posicionX);

    bytes.resize(sizeof(posicionY_net) + sizeof(posicionX_net));
    memcpy(bytes.data(), &posicionY_net, sizeof(posicionY_net));
    memcpy(bytes.data() + sizeof(posicionY_net), &posicionX_net, sizeof(posicionX_net));

    peer.sendall(bytes.data(), bytes.size(), &was_closed);
}
