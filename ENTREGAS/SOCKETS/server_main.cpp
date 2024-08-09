#include "server_servidor.h"

int main(int argc, char* argv[]) {

    if (argc != 3) {
        std::cerr << "Error: Cantidad incorrecta de parametros" << std::endl;
        return 1;
    }

    const std::string servname = argv[1];
    const std::string archivo = argv[2];

    Socket socket(servname.c_str());
    Socket peer = socket.accept();
    Servidor servidor(std::move(peer), archivo);

    if (servidor.iniciar_juego() == 1) {
        return 0;
    }

    servidor.actualizar_juego();
    return 0;
}
