#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <errno.h>
#include <stdbool.h>
#include "newstructure.h"
#include"newadmin.h"
#include"newfaculty.h"
#include"newstudent.h"


#define PORT 2008
#define BUFFER_SIZE 1024


struct AcceptedSocket
{
    int acceptedSocketFD;
    struct sockaddr_in address;
    
}; 

int acceptedSockets[10] ;
int acceptedSocketsCount = 0;


void *receiveAndPrintIncomingData(void *p) {
     
      struct AcceptedSocket* acceptedSocket =  (struct AcceptedSocket*)p;
    
    int fd = acceptedSocket->acceptedSocketFD;
    //char buffer[1024];
    int choice;
    
     ssize_t  chrec = recv(fd,&choice,sizeof(int),0);
     if(chrec<0){
           perror("recv:");
           return NULL;
     }
     
     switch(choice){
          case 1:
              admin(fd);
              break;
          case 2:
              faculty(fd);
              break;
          case 3:
              student(fd);
              break;
          default:
             send(fd,"Invalid Choice!!", strlen("Invalid Choice!!"), 0);
              
             break;
     }
     
   
    
    pthread_exit(NULL);
    close(fd);
    return NULL;
}

int SeparateThreadForNewClient(int server_socket) {
    
    struct sockaddr_in  clientAddress ;
    int clientAddressSize = sizeof (struct sockaddr_in);
    int clientSocketFD = accept(server_socket,(struct sockaddr*)&clientAddress,&clientAddressSize);
    if(clientSocketFD==-1){
       perror("accept:");
       return -1;
    }
    
     struct AcceptedSocket* acceptedSocket = malloc(sizeof (struct AcceptedSocket));
    acceptedSocket->address = clientAddress;
    acceptedSocket->acceptedSocketFD = clientSocketFD;
    
    
    pthread_t id;
    if(pthread_create(&id,NULL,receiveAndPrintIncomingData,acceptedSocket)!=0){
       perror("Thread creation error");
       return -1;
    }
    
    return clientSocketFD;
}

void startAcceptingIncomingConnections(int server_socket) {

    while(acceptedSocketsCount<10)
    {
        int clientSocketFD = SeparateThreadForNewClient(server_socket);
        
        if(clientSocketFD==-1){
          break;
        }
        
        acceptedSockets[acceptedSocketsCount++] = clientSocketFD;
        
    }
    return;
}


int main() {
    int server_socket; 
    struct sockaddr_in server_addr;
    socklen_t addr_size;
    char buffer[BUFFER_SIZE];

    // Create the server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation error");
        exit(1);
    }
    else{
       printf("server socket created successfully..");
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the server socket to the address
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding error");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_socket, 10) == 0) {
        printf("Listening...\n");
    } else {
        perror("Listening error");
        exit(1);
    }
    
    startAcceptingIncomingConnections(server_socket);

    if(close(server_socket)<0){
      perror("closing server socket:");
      exit(1);
    }

    return 0;
}

