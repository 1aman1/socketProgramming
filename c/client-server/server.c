#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>

void error( const char* msg) {
  perror(msg);
  exit(1);
}

int main( int argc, char* argv[] ) {
  //rguments must be supplied,,|| FIRST-filename || SECOND-portNo || 
  if( argc < 2 ) {
    fprintf(stderr, "port no not provided->proceeding to termination\n");
    exit(1);
  }

  int sockFd, newSockFd, portNo, n;
  char buffer[64];

  struct sockaddr_in SERVERaddr, CLIENTaddr;
  socklen_t cliLen;
  if( (sockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    error("error opening socket\n");
  else
    printf("socket created successfully\n");
  

 	 //printf("a\n");
  portNo = atoi(argv[1]);
//  printf("b\n");
  bzero( &SERVERaddr, sizeof(SERVERaddr)); // clears anything,if present, in whtever it refers to

  SERVERaddr.sin_family = AF_INET;
  SERVERaddr.sin_addr.s_addr = INADDR_ANY;
  SERVERaddr.sin_port = htons(portNo);

  //    
  if(( bind(sockFd, (struct sockaddr *)& SERVERaddr, sizeof(SERVERaddr) )) < 0)
    error("binding failed\n");

  //  FD, max clients allowed
  listen(sockFd, 5);

  cliLen = sizeof(CLIENTaddr);
  if( (newSockFd = accept(sockFd, (struct sockaddr *)& CLIENTaddr, &cliLen)) < 0)
    error("no accept\n");

  while( 1 ) {
    bzero( buffer, sizeof(buffer));
    if( read( newSockFd, buffer, sizeof(buffer)) < 0)
      error("cant read\n");
    printf("[Client] : %s\n", buffer);
//take input,after resetting the buffer
    bzero( buffer, sizeof(buffer));
    fgets( buffer, sizeof(buffer), stdin);

    if( write( newSockFd, buffer, sizeof(buffer)) < 0)
      error("cant write\n");
    if( !strncmp(buffer, "bye", 3) )
      break;
  }
  close(newSockFd);
  close(sockFd);
  return 0;
}
