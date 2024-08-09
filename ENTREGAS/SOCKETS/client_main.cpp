#include "client_cliente.h"

int main(int argc, char* argv[]) {

    if (argc != 3) {
        std::cerr << "Error: Cantidad incorrecta de parametros" << std::endl;
        return 1;
    }

    const std::string hostname = argv[1];
    const std::string servname = argv[2];

    Socket socket(hostname.c_str(), servname.c_str());
    Cliente cliente(std::move(socket));

    if (cliente.iniciar_juego() == 1) {
        std::cerr << "Fallo" << std::endl;
        return 0;
    }

    if (cliente.jugar() == 1) {
        std::cerr << "Error: Accion incorrecta" << std::endl;
        return 0;
    }

    return 0;
}
