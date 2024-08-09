#ifndef JUEGO_H
#define JUEGO_H

#include <fstream>
#include <string>
#include <vector>

struct Gusano {
    // cppcheck-suppress unusedStructMember
    int x;
    // cppcheck-suppress unusedStructMember
    int y;
    // cppcheck-suppress unusedStructMember
    int direccion;
};

class Juego {
private:
    std::ifstream archivo;

    int filas, columnas;
    std::vector<std::vector<char>> escenario;

    Gusano gusano;

public:
    explicit Juego(const std::string& archivo);

    int seleccionar_escenario(const std::string& escenario);
    int cambiar_direccion(const int& nueva_direccion);
    int mover();
    int saltar(const int& tipo);
    void simular_gravedad();

    bool validar_escenario(const std::string& escenario_pedido);
    void guardar_escenario();
    bool validar_ubicacion(int x, int y);

    int get_x();
    int get_y();
};

#endif
