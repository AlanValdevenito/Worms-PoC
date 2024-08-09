#include "server_juego.h"

#include <sstream>

#define SUELO_PARED 'X'
#define GUSANO 'G'

#define IZQUIERDA -1
#define DERECHA 1

#define SALTAR_ADELANTE 0
#define SALTAR_ATRAS 1

Juego::Juego(const std::string& archivo):
        archivo(archivo), filas(0), columnas(0), gusano({0, 0, 0}) {}

int Juego::seleccionar_escenario(const std::string& escenario) {

    if (!validar_escenario(escenario)) {
        return 1;
    }

    guardar_escenario();
    return 0;
}

int Juego::cambiar_direccion(const int& nueva_direccion) {

    if (nueva_direccion == 0) {
        this->gusano.direccion = IZQUIERDA;
    } else if (nueva_direccion == 1) {
        this->gusano.direccion = DERECHA;
    }

    return 0;
}

int Juego::mover() {

    if (validar_ubicacion(this->gusano.x, this->gusano.y + this->gusano.direccion)) {
        this->gusano.y = this->gusano.y + this->gusano.direccion;
        simular_gravedad();
    }

    return 0;
}

int Juego::saltar(const int& tipo) {

    if (tipo == SALTAR_ADELANTE) {

        if (validar_ubicacion(this->gusano.x - 1, this->gusano.y)) {
            this->gusano.x--;
        } else {
            return 0;
        }

        if (validar_ubicacion(this->gusano.x, this->gusano.y + this->gusano.direccion)) {
            this->gusano.y = this->gusano.y + this->gusano.direccion;
        } else {
            simular_gravedad();
            return 0;
        }

        if (validar_ubicacion(this->gusano.x, this->gusano.y + this->gusano.direccion)) {
            this->gusano.y = this->gusano.y + this->gusano.direccion;
        } else {
            simular_gravedad();
            return 0;
        }

    } else if (tipo == SALTAR_ATRAS) {

        if (validar_ubicacion(this->gusano.x - 1, this->gusano.y)) {
            this->gusano.x--;
        } else {
            return 0;
        }

        if (validar_ubicacion(this->gusano.x - 1, this->gusano.y)) {
            this->gusano.x--;
        } else {
            simular_gravedad();
            return 0;
        }

        if (validar_ubicacion(this->gusano.x, this->gusano.y - this->gusano.direccion)) {
            this->gusano.y = this->gusano.y - this->gusano.direccion;
        } else {
            simular_gravedad();
            return 0;
        }
    }

    simular_gravedad();
    return 0;
}

void Juego::simular_gravedad() {

    while (validar_ubicacion(this->gusano.x + 1, this->gusano.y)) {
        this->gusano.x++;
    }
}

bool Juego::validar_escenario(const std::string& escenario_pedido) {
    std::string linea;
    std::string escenario;

    while (std::getline(this->archivo, linea)) {
        std::istringstream iss(linea);
        if ((iss >> this->columnas >> this->filas >> escenario) &&
            (escenario == escenario_pedido)) {
            return true;
        }
    }

    return false;
}

void Juego::guardar_escenario() {
    this->escenario.resize(this->filas, std::vector<char>(this->columnas));
    std::string linea;

    for (int i = 0; i < this->filas; i++) {
        std::getline(this->archivo, linea);
        for (int j = 0; j < this->columnas; j++) {
            this->escenario[i][j] = linea[j];

            if (this->escenario[i][j] == GUSANO) {
                this->gusano.x = i;
                this->gusano.y = j;
            }
        }
    }

    this->gusano.direccion = IZQUIERDA;
    this->archivo.close();
}

bool Juego::validar_ubicacion(int x, int y) { return (this->escenario[x][y] != SUELO_PARED); }

int Juego::get_x() { return this->gusano.x; }

int Juego::get_y() { return this->gusano.y; }
