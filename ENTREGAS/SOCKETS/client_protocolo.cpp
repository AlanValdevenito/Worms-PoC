#include "client_protocolo.h"

#include <arpa/inet.h>

#define SELECT 1
#define DIR 3
#define MOVE 4
#define JUMP 5

#define BYTES_CODIGO 1
#define BYTES_X 4
#define BYTES_Y 4

ProtocoloCliente::ProtocoloCliente(Socket&& socket): skt(std::move(socket)) {}

int ProtocoloCliente::esperar_codigo() {
    bool was_closed = false;
    char buffer_codigo[BYTES_CODIGO] = {0};
    skt.recvall(buffer_codigo, sizeof(buffer_codigo), &was_closed);

    uint8_t byteCodigo;
    memcpy(&byteCodigo, buffer_codigo, sizeof(byteCodigo));
    int codigo = static_cast<int>(byteCodigo);

    return codigo;
}

Ubicacion ProtocoloCliente::esperar_ubicacion() {
    bool was_closed = false;
    char buf[BYTES_X + BYTES_Y] = {0};
    skt.recvall(buf, sizeof(buf), &was_closed);

    Ubicacion ubicacion;

    uint32_t posicionY_net, posicionX_net;

    memcpy(&posicionY_net, buf, sizeof(posicionY_net));
    memcpy(&posicionX_net, buf + sizeof(posicionY_net), sizeof(posicionX_net));

    int posicionY = ntohl(posicionY_net);
    int posicionX = ntohl(posicionX_net);

    ubicacion.y = posicionY;
    ubicacion.x = posicionX;

    return ubicacion;
}

void ProtocoloCliente::seleccionar_escenario(const std::string& escenario) {
    bool was_closed = false;
    std::vector<uint8_t> bytes;

    uint8_t accion = static_cast<uint8_t>(SELECT);

    uint16_t longitud = static_cast<uint16_t>(escenario.size());
    uint16_t longitud_net = htons(longitud);

    bytes.resize(sizeof(accion) + sizeof(longitud_net) + escenario.size());
    memcpy(bytes.data(), &accion, sizeof(accion));
    memcpy(bytes.data() + sizeof(accion), &longitud_net, sizeof(longitud_net));
    memcpy(bytes.data() + sizeof(accion) + sizeof(longitud_net), escenario.c_str(),
           escenario.size());

    skt.sendall(bytes.data(), bytes.size(), &was_closed);
}

void ProtocoloCliente::cambiar_direccion_gusano(const int& direccion) {
    bool was_closed = false;
    std::vector<uint8_t> bytes;

    uint8_t accion = static_cast<uint8_t>(DIR);
    uint8_t dir = static_cast<uint8_t>(direccion);

    bytes.resize(sizeof(accion) + sizeof(dir));
    memcpy(bytes.data(), &accion, sizeof(accion));
    memcpy(bytes.data() + sizeof(accion), &dir, sizeof(dir));

    skt.sendall(bytes.data(), bytes.size(), &was_closed);
}

void ProtocoloCliente::mover_gusano() {
    bool was_closed = false;
    std::vector<uint8_t> bytes;

    uint8_t accion = static_cast<uint8_t>(MOVE);

    bytes.resize(sizeof(accion));
    memcpy(bytes.data(), &accion, sizeof(accion));

    skt.sendall(bytes.data(), bytes.size(), &was_closed);
}

void ProtocoloCliente::saltar_gusano(const int& tipo) {
    bool was_closed = false;
    std::vector<uint8_t> bytes;

    uint8_t accion = static_cast<uint8_t>(JUMP);
    uint8_t tip = static_cast<uint8_t>(tipo);

    bytes.resize(sizeof(accion) + sizeof(tip));
    memcpy(bytes.data(), &accion, sizeof(accion));
    memcpy(bytes.data() + sizeof(accion), &tip, sizeof(tip));

    skt.sendall(bytes.data(), bytes.size(), &was_closed);
}
