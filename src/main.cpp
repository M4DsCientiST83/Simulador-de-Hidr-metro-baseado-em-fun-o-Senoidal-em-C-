#include "Control.cpp"
#include "DisplaySocket.hpp"
#include <iostream>
#include <filesystem>
#include <thread>
#include <vector>
#include <string>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h> // Header necessário para GetModuleFileNameA
#pragma comment(lib, "ws2_32.lib")
#endif

namespace fs = std::filesystem;

// Função que será executada por cada thread (sem alterações)
void run_simulation(SOCKET client_fd, std::string configPath) {
    auto thread_id = std::this_thread::get_id();
    std::cout << "Thread " << thread_id << ": Iniciando simulacao para o cliente socket " << client_fd << std::endl;

    try {
        DisplaySocket displaySocket(client_fd);
        std::cout << "Thread " << thread_id << ": Objeto DisplaySocket criado." << std::endl;

        Control control(configPath, &displaySocket);
        std::cout << "Thread " << thread_id << ": Objeto Control criado. Iniciando execucao..." << std::endl;
        control.executar();
    } catch (const std::exception& e) {
        std::cerr << "ERRO na thread " << thread_id << ": " << e.what() << std::endl;
    }

    std::cout << "Thread " << thread_id << ": Cliente desconectado." << std::endl;
    closesocket(client_fd);
}

int main() 
{
    // Inicializa Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    // --- LÓGICA DE CAMINHO ROBUSTA ---
    // Encontra o caminho do executável para localizar o config.json de forma segura.
    fs::path configPath;
    #ifdef _WIN32
        char exePath[MAX_PATH];
        GetModuleFileNameA(NULL, exePath, MAX_PATH);
        // O caminho do executável é: ...\Projeto_Hidro\build\Debug\simulador.exe
        // Precisamos voltar 3 níveis para chegar à raiz do projeto.
        configPath = fs::path(exePath).parent_path().parent_path().parent_path() / "data" / "config.json";
    #else
        // Fallback para outros sistemas (pode precisar de ajuste)
        configPath = fs::current_path() / "data" / "config.json";
    #endif

    if (!fs::exists(configPath)) {
        std::cerr << "Arquivo de configuracao nao encontrado no caminho esperado: " << configPath.string() << std::endl;
        WSACleanup();
        system("pause"); // Pausa para que o erro possa ser lido
        return 1;
    }
    std::cout << "Usando arquivo de configuracao: " << configPath.string() << std::endl;
    // --- FIM DA LÓGICA DE CAMINHO ---


    // Cria socket TCP
    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) {
        std::cerr << "Erro ao criar socket\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(5000);

    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        std::cerr << "Erro no bind. Verifique se o programa ja nao esta rodando.\n";
        closesocket(server_fd);
        WSACleanup();
        system("pause");
        return 1;
    }

    if (listen(server_fd, 5) == SOCKET_ERROR) {
        std::cerr << "Erro no listen\n";
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    std::cout << "Servidor iniciado. Aguardando conexoes na porta 5000...\n";
    
    std::vector<std::thread> client_threads;
    int connected_clients = 0;
    const int MAX_CLIENTS = 5;

    // Loop principal para aceitar múltiplas conexões
    while (connected_clients < MAX_CLIENTS) 
    {
        SOCKET client_fd = accept(server_fd, nullptr, nullptr);

        if (client_fd == INVALID_SOCKET) {
            std::cerr << "Erro no accept. Encerrando servidor.\n";
            break; 
        }

        std::cout << "Cliente Java conectado! Socket: " << client_fd << std::endl;
        connected_clients++;
        std::cout << "Total de clientes conectados: " << connected_clients << "/" << MAX_CLIENTS << std::endl;
        
        client_threads.emplace_back(run_simulation, client_fd, configPath.string());
        client_threads.back().detach(); 
    }

    std::cout << "Numero maximo de clientes atingido ou servidor encerrado." << std::endl;
    
    // Mantém o programa principal vivo para que as threads não sejam terminadas abruptamente.
    std::cout << "Pressione Enter para encerrar o servidor..." << std::endl;
    std::cin.get();

    closesocket(server_fd);
    WSACleanup();

    return 0;
}