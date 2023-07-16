#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>

using namespace std;

int main()
{
    int serverSocket;
    struct sockaddr_in serverAddress, clientAddress;
    char buffer[1024];

    // Create server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
    {
        std::cerr << "Error creating socket.\n";
        return 1;
    }

    // Set up server address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to address
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        std::cerr << "Error binding socket.\n";
        return 1;
    }

    // Listen for incoming connections
    listen(serverSocket, 5);
    std::cout << "Server listening on port 8080...\n";

    // Accept client connection
    socklen_t clientAddressLength = sizeof(clientAddress);
    int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
    if (clientSocket < 0)
    {
        std::cerr << "Error accepting client connection.\n";
        return 1;
    }

    // Receive data from client
    memset(buffer, 0, sizeof(buffer));
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << "Received from client: " << buffer << "\n";

    // Send response to client
    const char *response = "Hello from server!";
    send(clientSocket, response, strlen(response), 0);
    std::cout << "Response sent to client.\n";

    // Close sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}
