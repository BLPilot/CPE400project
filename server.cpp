#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>

using namespace std;
  
#define PORT     8080
#define MAXLINE 1024
#define MAX 80

void infoExchange(int sockfd);

// Driver code
int main() {
    int sockfd;
    char buffer[MAXLINE];
    char hello[] = "Hello from server";
    struct sockaddr_in servaddr, cliaddr;
      
    // Creating socket file descriptor and verification
    
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0))< 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }else{
    	cout<<"Server socket created successfully"<<endl;
    }
    
    
   //possible solutoin to bind: address already in use error
   //didnt work but could if figured out
   //so far just restart computer/virtual machine if error encountered
   /*int tr=1;
	//kill "Address already in use" error message
	if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&tr,sizeof(int)) == -1) {
    	perror("setsockopt");
    	exit(1);
	}*/
	 
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
      
    // Filling server information (assigning IP, Port)
    servaddr.sin_family    = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
      
    // Bind the socket with the server address
    if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }else{
    	cout<<"Server Client Successfullly binded"<<endl;
    
    }
      
    int n;
    socklen_t len;
  
    len = sizeof(cliaddr);  //len is value/resuslt
  
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
                MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                &len);
    buffer[n] = '\0';
    printf("Client : %s\n", buffer);
    sendto(sockfd, (const char *)hello, strlen(hello), 
        MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
            len);
    printf("Hello message sent.\n"); 
    

      
    return 0;
}


//Tryna figure out how to do TCP medata information exhange.
//Got this off tutorial and trying to modify to suit our needs
//Funciton for TCP medata information exhange
void infoExchange(int sockfd){
	char buff[MAX];
    int n;
    // infinite loop for chat
    for (;;) {
        bzero(buff, MAX);
  
        // read the message from client and copy it in buffer
        read(sockfd, buff, sizeof(buff));
        // print buffer which contains the client contents
        printf("From client: %s\t To client : ", buff);
        bzero(buff, MAX);
        n = 0;
        // copy server message in the buffer
        while ((buff[n++] = getchar()) != '\n')
            ;
  
        // and send that buffer to client
        write(sockfd, buff, sizeof(buff));
  
        // if msg contains "Exit" then server exit and chat ended.
        if (strncmp("exit", buff, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
    }

}
