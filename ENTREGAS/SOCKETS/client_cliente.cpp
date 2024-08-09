#include "client_cliente.h"

#include <utility>

#define CODIGO_ERROR 1

#define SELECT "select"
#define DIR "dir"
#define MOVE "move"
#define JUMP "jump"

Cliente::Cliente(Socket&& socket): protocolo(std::move(socket)) {}

int Cliente::iniciar_juego() {
    std::string select;
    std::cin >> select;

    if (enviar_seleccionar_escenario(select) == 1) {
        return 1;
    }

    int codigo = recibir_codigo();

    if (codigo == CODIGO_ERROR) {
        return 1;
    }

    Ubicacion ubicacion_gusano = recibir_ubicacion();
    imprimir_ubicacion(ubicacion_gusano);
    return 0;
}

int Cliente::jugar() {
    std::string accion;
    Ubicacion ubicacion_gusano;

    while (std::cin >> accion) {

        if ((accion == "") || (accion[0] == '#')) {
            std::getline(std::cin, accion);
            continue;
        }

        if (enviar_accion(accion) == 1) {
            return 1;
        }

        ubicacion_gusano = recibir_ubicacion();
        imprimir_ubicacion(ubicacion_gusano);
    }

    return 0;
}

int Cliente::enviar_seleccionar_escenario(const std::string& accion) {

    if (accion == SELECT) {
        std::string escenario;
        std::cin >> escenario;

        protocolo.seleccionar_escenario(escenario);
        return 0;
    }

    return 1;
}

int Cliente::enviar_accion(const std::string& accion) {

    if (accion == DIR) {
        int direccion;
        std::cin >> direccion;

        protocolo.cambiar_direccion_gusano(direccion);
        return 0;

    } else if (accion == MOVE) {
        protocolo.mover_gusano();
        return 0;

    } else if (accion == JUMP) {
        int tipo;
        std::cin >> tipo;

        protocolo.saltar_gusano(tipo);
        return 0;
    }

    return 1;
}

int Cliente::recibir_codigo() { return protocolo.esperar_codigo(); }

Ubicacion Cliente::recibir_ubicacion() { return protocolo.esperar_ubicacion(); }

void Cliente::imprimir_ubicacion(const Ubicacion& ubicacion) {

    std::string posicionY = std::to_string(ubicacion.y);
    std::string posicionX = std::to_string(ubicacion.x);

    std::cout << posicionY + " " + posicionX << std::endl;
}
