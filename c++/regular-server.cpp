#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddress, clientAddress;
    int portNumber = 8080; // The port number you want to expose

    // Create a socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
    {
        std::cerr << "Error opening socket." << std::endl;
        return 1;
    }

    // Set up the server address structure
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(portNumber);

    // Bind the socket to the specified address and port
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        std::cerr << "Error binding socket." << std::endl;
        return 1;
    }

    // Start listening for incoming connections
    if (listen(serverSocket, 5) < 0)
    {
        std::cerr << "Error listening for connections." << std::endl;
        return 1;
    }

    std::cout << "Server listening on port " << portNumber << std::endl;

    // Accept incoming connections
    socklen_t clientAddressLength = sizeof(clientAddress);
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
    if (clientSocket < 0)
    {
        std::cerr << "Error accepting connection." << std::endl;
        return 1;
    }

    // Handle the incoming connection
    std::cout << "Accepted connection from: " << inet_ntoa(clientAddress.sin_addr) << std::endl;

    // Close the client socket
    close(clientSocket);

    // Close the server socket
    close(serverSocket);

    return 0;
}
