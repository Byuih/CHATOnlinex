#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <iostream>
#include <string>

#pragma comment(lib, "WS2_32.lib")

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize winsock" << std::endl;
        return 1;
    }

    // Create a socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket" << std::endl;
        WSACleanup();
        return 1;
    }

    // Connect to the server
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP address of the server
    serverAddr.sin_port = htons(12345); // Port number of the server

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Failed to connect to server" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to server!" << std::endl;

    // Main loop to receive and send messages
    char buffer[4096];
    int bytesReceived;

    while (true) {
        // Send data to the server
        std::cout << "Client: ";
        std::string message;
        std::getline(std::cin, message);

        send(clientSocket, message.c_str(), message.size(), 0);

        // Receive data from the server
        bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            std::cerr << "Connection closed by server" << std::endl;
            break;
        }

        // Display received message
        std::cout << "Server: " << std::string(buffer, 0, bytesReceived) << std::endl;
    }

    // Cleanup
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}