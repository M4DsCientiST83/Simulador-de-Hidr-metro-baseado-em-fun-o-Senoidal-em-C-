#include "Control.cpp"
#include "DisplaySocket.hpp"
#include <iostream>
#include <filesystem>
#include <thread>   // Incluído para usar threads
#include <vector>   // Incluído para gerenciar as threads
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

namespace fs = std::filesystem;

// Função que será executada por cada thread para gerenciar uma simulação de cliente
void run_simulation(SOCKET client_fd, std::string configPath) {
    auto thread_id = std::this_thread::get_id(); // Pega o ID da thread para facilitar a depuração
    std::cout << "Thread " << thread_id << ": Iniciando simulacao para o cliente socket " << client_fd << std::endl;

    try {
        // 1. Inicializa um objeto da classe DisplaySocket para este cliente específico
        DisplaySocket displaySocket(client_fd);
        std::cout << "Thread " << thread_id << ": Objeto DisplaySocket criado." << std::endl;


        // 2. Inicializa um objeto da classe Control que vai executar todo o algoritmo para este cliente
        Control control(configPath, &displaySocket);
        std::cout << "Thread " << thread_id << ": Objeto Control criado. Iniciando execucao..." << std::endl;
        control.executar();
    } catch (const std::exception& e) {
        std::cerr << "ERRO na thread " << thread_id << ": " << e.what() << std::endl;
    }

    // 3. Encerra a conexão com este cliente
    std::cout << "Thread " << thread_id << ": Cliente desconectado." << std::endl;
    closesocket(client_fd);
}

int main() 
{
    // Inicializa Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 
    {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    // Configura o caminho para o arquivo de configuração
    fs::path configPath = fs::current_path().parent_path() / "data" / "config.json";
    if (!fs::exists(configPath)) {
        std::cerr << "Arquivo de configuracao nao encontrado em: " << configPath.string() << std::endl;
        // Tenta um caminho alternativo, comum no VSCode
        configPath = fs::current_path() / "data" / "config.json";
        if (!fs::exists(configPath)) {
             std::cerr << "Arquivo de configuracao tambem nao encontrado em: " << configPath.string() << std::endl;
             WSACleanup();
             return 1;
        }
    }
    std::cout << "Usando arquivo de configuracao: " << configPath.string() << std::endl;


    // Cria socket TCP
    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET) 
    {
        std::cerr << "Erro ao criar socket\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(5000);

    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        std::cerr << "Erro no bind\n";
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    if (listen(server_fd, 5) == SOCKET_ERROR) { // Aumentado para um backlog de 5
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

        if (client_fd == INVALID_SOCKET) 
        {
            std::cerr << "Erro no accept. Encerrando servidor.\n";
            break; // Sai do loop se houver um erro no accept
        }

        std::cout << "Cliente Java conectado! Socket: " << client_fd << std::endl;
        connected_clients++;
        std::cout << "Total de clientes conectados: " << connected_clients << "/" << MAX_CLIENTS << std::endl;
        
        // Cria e inicia uma nova thread para o cliente recém-conectado
        client_threads.emplace_back(run_simulation, client_fd, configPath.string());
        client_threads.back().detach(); // Permite que a thread rode de forma independente
    }

    std::cout << "Numero maximo de clientes atingido. O servidor nao aceitara novas conexoes." << std::endl;
    // O programa principal pode terminar aqui ou esperar por um sinal para desligar.
    // Para este exemplo, ele vai simplesmente encerrar após aceitar 5 clientes.

    // Encerra o socket do servidor
    closesocket(server_fd);
    WSACleanup();

    // Como as threads foram desanexadas (detach), o programa principal pode terminar
    // enquanto as simulações continuam rodando em suas próprias threads.
    // É importante notar que em uma aplicação real, seria necessário um mecanismo
    // mais robusto para gerenciar o ciclo de vida das threads.
    
    // Para manter o servidor "vivo" enquanto as threads rodam, 
    // podemos adicionar um loop de espera aqui.
    std::cout << "Pressione Enter para encerrar o servidor e todas as conexoes..." << std::endl;
    std::cin.get();


    return 0;
}