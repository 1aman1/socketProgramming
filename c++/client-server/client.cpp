#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cstring>
#include <unistd.h>

int main()
{
    int clientSocket;
    struct sockaddr_in serverAddress;
    char buffer[1024];

    // Create client socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0)
    {
        std::cerr << "Error creating socket.\n";
        return 1;
    }

    // Set up server address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);

    // Convert server address from string to network address structure
    struct hostent *server = gethostbyname("localhost");
    if (server == NULL)
    {
        std::cerr << "Error resolving hostname.\n";
        return 1;
    }
    memcpy(&serverAddress.sin_addr.s_addr, server->h_addr, server->h_length);

    // Connect to server
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        std::cerr << "Error connecting to server.\n";
        return 1;
    }

    // Send data to server
    const char *message = "Hello from client!";
    send(clientSocket, message, strlen(message), 0);
    std::cout << "Message sent to server.\n";

    // Receive response from server
    memset(buffer, 0, sizeof(buffer));
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << "Received from server: " << buffer << "\n";

    // Close socket
    close(clientSocket);

    return 0;
}
