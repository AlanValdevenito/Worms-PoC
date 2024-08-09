#include "server_servidor.h"

#include <utility>

#define CODIGO_ERROR 1

#define SELECT 1
#define DIR 3
#define MOVE 4
#define JUMP 5

Servidor::Servidor(Socket&& socket, const std::string& archivo):
        protocolo(std::move(socket)), juego(archivo) {}

int Servidor::iniciar_juego() {
    bool was_closed = false;
    Select select = recibir_seleccionar_escenario(&was_closed);

    int codigo = this->juego.seleccionar_escenario(select.escenario);
    enviar_codigo(codigo);

    if (codigo == CODIGO_ERROR) {
        return 1;
    }

    Ubicacion ubicacion_gusano = get_ubicacion();
    enviar_ubicacion(ubicacion_gusano);
    imprimir_ubicacion(ubicacion_gusano);
    return 0;
}

void Servidor::actualizar_juego() {
    bool was_closed = false;

    Accion accion;
    Ubicacion ubicacion_gusano;

    while (not was_closed) {
        accion = recibir_accion(&was_closed);

        if (was_closed) {
            break;
        }

        ejecutar_accion(accion);

        ubicacion_gusano = get_ubicacion();
        enviar_ubicacion(ubicacion_gusano);
        imprimir_ubicacion(ubicacion_gusano);
    }
}

Select Servidor::recibir_seleccionar_escenario(bool* was_closed) {
    return this->protocolo.esperar_seleccionar_escenario(was_closed);
}

Accion Servidor::recibir_accion(bool* was_closed) {
    return this->protocolo.esperar_accion(was_closed);
}

int Servidor::ejecutar_accion(const Accion& accion) {

    if (accion.tipo == DIR) {
        return this->juego.cambiar_direccion(accion.direccion);

    } else if (accion.tipo == MOVE) {
        return this->juego.mover();

    } else if (accion.tipo == JUMP) {
        return this->juego.saltar(accion.direccion);
    }

    return 1;
}

void Servidor::enviar_codigo(const int& codigo) { this->protocolo.enviar_codigo(codigo); }

void Servidor::enviar_ubicacion(const Ubicacion& ubicacion) {
    this->protocolo.enviar_ubicacion(ubicacion);
}

Ubicacion Servidor::get_ubicacion() {
    Ubicacion ubicacion;

    ubicacion.x = this->juego.get_x();
    ubicacion.y = this->juego.get_y();

    return ubicacion;
}

void Servidor::imprimir_ubicacion(const Ubicacion& ubicacion) {

    std::string posicionY = std::to_string(ubicacion.y);
    std::string posicionX = std::to_string(ubicacion.x);

    std::cout << posicionY + " " + posicionX << std::endl;
}
