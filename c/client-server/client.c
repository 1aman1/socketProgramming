#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
#include<netdb.h>

void error( const char* msg) {
  perror(msg);
  exit(1);
}

int main( int argc, char* argv[] ) {
  //rguments must be supplied,,|| FIRST-filename || SECOND-portNo || 
  if( argc < 3 ) {
    fprintf(stderr, "port no not provided->proceeding to termination\n");
    exit(1);
  }

  int sockFd, portNo, n;
  char buffer[64];

  struct sockaddr_in SERVERaddr;
  struct hostent *server;

  portNo = atoi(argv[2]);

  if( (sockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    error("error opening socket\n");
  else
    printf("socket created successfully\n");

  
  if( (server = gethostbyname(argv[1])) == NULL )
    error("no such host\n");

  bzero(&SERVERaddr, sizeof(SERVERaddr)); // flushes
  SERVERaddr.sin_family = AF_INET;
  bcopy(  server->h_addr, & SERVERaddr.sin_addr.s_addr, server->h_length);
  SERVERaddr.sin_port = htons(portNo);

  if( connect(sockFd, (struct sockaddr *)& SERVERaddr, sizeof(SERVERaddr)) < 0)
    error("connection failed\n");

  while( 1 ) {
//printf("senanca\n");

    bzero( buffer, sizeof(buffer));
//take input,after resetting the buffer
    fgets( buffer, sizeof(buffer), stdin);
    if( write( sockFd, buffer, sizeof(buffer)) < 0)
      error("cant write\n");

    bzero( buffer, sizeof(buffer));

    if( read( sockFd, buffer, sizeof(buffer)) < 0)
      error("cant read\n");

    printf("[Server] : %s\n", buffer);

    if( !strncmp(buffer, "bye", 3) )
      break;
  }
//  close(newSockFd);
  close(sockFd);
  return 0;
}
