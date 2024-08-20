#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include<errno.h>
#include <stdbool.h>
#include "newstructure.h"
#include "newadmin.h"


#define PORT 2008
#define SERVER_IP "127.0.0.1" // Replace with the server's IP address

int main() {
    int clientSocketFD;
    struct sockaddr_in server_addr;
    

    // Create the client socket
    clientSocketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocketFD < 0) {
        perror("Socket creation error");
        exit(1);
    }
    else{
        printf("Client Socket Created Successfully..\n");
    }
    
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to the server
    if (connect(clientSocketFD, (struct sockaddr*)&server_addr, sizeof(server_addr)) == 0) {
        printf("Connected to the server.\n");
    } else {
        perror("Connection error");
        exit(1);
    }
    
    printf(".........Welcome Back to Academia :: Course Registration.........\n");
    printf("Login Type\n");
    printf("Enter Your Choice { 1.Admin , 2.Faculty  , 3.Student } : \n");
    int choice;
    scanf("%d",&choice);
    send(clientSocketFD,&choice,sizeof(int), 0);
    char buffer[1024];
    
     while (1)
    {
    
         memset(buffer, 0, sizeof(buffer));

         ssize_t  amountReceived = recv(clientSocketFD,buffer,sizeof(buffer),0);
         

        if(amountReceived>0)
        {
            buffer[amountReceived] = '\0';
            printf("%s\n",buffer);

        }
        else if(amountReceived<0){
           perror("recv:");
           break;
        }
        else if(strcmp(buffer,"exit")==0){
           exit(1);
        }
        
        
        memset(buffer, 0, sizeof(buffer));
        scanf("%s",buffer);
        
        
        send(clientSocketFD,buffer,sizeof(buffer), 0);

        if(amountReceived==0)
            break;
    
    }
    /*switch(choice){
          case 1:

              break;
              
          case 2:
              strcpy(message, "Faculty");
              break;
          case 3:
              strcpy(message, "Student");
              break;
          default:
             printf("Invalid Choice!!"); 
             continue;
                
    }*/
    
    
    
    printf("Do you want to exit? (1 for yes, 0 for no): ");
    int exitChoice;
    scanf("%d", &exitChoice);
    if (exitChoice == 1) {
    exit(1);
}


    close(clientSocketFD);

    return 0;
}

