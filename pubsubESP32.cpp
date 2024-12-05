#include <iostream>
#include <cstring>
#include <thread>
#include <arpa/inet.h>                 // Para socket e operações relacionadas
#include <unistd.h>                   // Para close()

#define SERVER_PORT_UDP 4000        // Porta para escutar mensagens UDP
#define SERVER_PORT_TCP 3000       // Porta do servidor TCP no ESP32
#define SERVER_IP "192.168.164.37" // IP do ESP32
#define BUFFER_SIZE 1024


int tcp() {
    int sock; 
    //int bytesReceived;
    struct sockaddr_in serverAddr;
    //char buffer[1024] = {0};

    // Criar socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Erro ao criar socket" << std::endl;
        return -1;
    }

    // Configurar o endereço do servidor
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT_TCP);
    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
        std::cerr << "Endereço inválido ou não suportado" << std::endl;
        return -1;
    }

    // Conectar ao servidor
    if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Erro ao conectar ao servidor" << std::endl;
        return -1;
    }

    std::cout << "Conectado ao servidor ESP32." << std::endl;

    while (true) {
        std::cout << "ESP32$ ";
        std::string command;
        std::getline(std::cin, command);

        if (command == "EXIT") {
            break;
        }

        // Enviar comando para o servidor
        send(sock, command.c_str(), command.size(), 0);

        // Receber resposta do servidor
        // bytesReceived = recv(sock, buffer, sizeof(buffer) - 1, 0);
        // if (bytesReceived > 0) {
        //     buffer[bytesReceived] = '\0';
        //     std::cout<< buffer;
        // } else {
        //     std::cerr << "Conexão perdida com o servidor." << std::endl;
        //     break;
        // }
    }

    // Fechar conexão
    close(sock);
    std::cout << "Cliente encerrado." << std::endl;
    return 0;
}

int udp() {
    int sock, bytesReceived;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[BUFFER_SIZE];
    socklen_t addrLen = sizeof(clientAddr);

    // Criar socket UDP
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        std::cerr << "Erro ao criar socket UDP" << std::endl;
        return -1;
    }

    // Configurar endereço do servidor
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(SERVER_PORT_UDP);

    // Associar o socket ao endereço
    if (bind(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Erro ao associar o socket à porta UDP" << std::endl;
        close(sock);
        return -1;
    }

    std::cout << "Servidor UDP aguardando mensagens na porta " << SERVER_PORT_UDP << "..." << std::endl;

    while (true) {
        // Receber mensagem do cliente UDP
        bytesReceived = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&clientAddr, &addrLen);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::cout << buffer;
            //std::cout << "\nESP32$ ";
        } else {
            std::cerr << "Erro ao receber mensagem UDP." << std::endl;
        }
    }

    // Fechar o socket
    close(sock);
    return 0;
}


int main(){
    std::thread threadtcp(tcp);
    std::thread threadudp(udp); 
    threadtcp.join();
    threadudp.join();
    return 0;
}