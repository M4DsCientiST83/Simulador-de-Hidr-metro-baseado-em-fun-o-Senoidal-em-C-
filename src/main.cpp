#include "Control.hpp"
#include "DisplaySocket.hpp"
#include <iostream>
#include <filesystem>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#endif

namespace fs = std::filesystem;

int main() 
{
    #ifdef _WIN32
        // Inicializa Winsock
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
            std::cerr << "WSAStartup failed\n";
            return 1;
        }
    #endif

    // Configuração de caminho
    fs::path caminhoBase = fs::current_path().parent_path();
    fs::path configPath = caminhoBase / "data" / "config.json";


    // Criar socket TCP
    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        std::cerr << "Erro ao criar socket\n";
        return 1;
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(5000);

    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        std::cerr << "Erro no bind\n";
        return 1;
    }

    if (listen(server_fd, 1) == SOCKET_ERROR) {
        std::cerr << "Erro no listen\n";
        return 1;
    }

    std::cout << "Aguardando conexão Java na porta 5000...\n";
    SOCKET client_fd = accept(server_fd, nullptr, nullptr);
    if (client_fd == INVALID_SOCKET) {
        std::cerr << "Erro no accept\n";
        return 1;
    }
    std::cout << "Cliente Java conectado!\n";

    // Substitui o display por DisplaySocket


    // Injetar displaySocket no hidrometro (adaptar Hidrometro p/ aceitar Display* se necessário)
    // Por enquanto, se você quiser, pode trocar diretamente o Display interno por este DisplaySocket

    // Loop de simulação enviando dados para Java
    DisplaySocket displaySocket(client_fd);
    Control control(configPath.string(), &displaySocket);
    control.executar();

    closesocket(client_fd);
    closesocket(server_fd);
    WSACleanup();
    return 0;
}
