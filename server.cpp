// server code for UDP socket programming
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string.h>
  
#define PROTOCOL 0
#define PORT 15050
#define BUFFER_SIZE 32
#define cipherKey 2
#define nofile "File Not Found!"

using namespace std;
  
void clear(char* b);
char Encrypt(char ch); 
int sendFile(FILE* fname, char* buf, int s);
  

  
// driver code
int main()
{
    int sockfd;
    int n;
    struct sockaddr_in addr_con;
    socklen_t addrlen = sizeof(addr_con);
    
    //filling server information
    addr_con.sin_family = AF_INET;
    addr_con.sin_port = htons(PORT);
    addr_con.sin_addr.s_addr = INADDR_ANY;
    char buffer[BUFFER_SIZE];
    FILE* fname;
  
    // creating socket
    sockfd = socket(AF_INET, SOCK_DGRAM, PROTOCOL);
    
    if(sockfd <0){
   		perror("Socket creation failed");
   		exit(EXIT_FAILURE);
   }else{
		cout<<"Socket created succesfully"<<endl;
   }
    
  
    //Binding socket with server address
    if (bind(sockfd, (struct sockaddr*)&addr_con, sizeof(addr_con)) == 0){
        cout<<"Server Binded succesfully"<<endl;
    }else{
    	cout<<"Server Binding Failed"<<endl;
    }
    
    //TCP connection channel 
    
    /*socklen_t TCPsock;
    
    if(TCPsock = accept(sockfd, (struct sockaddr*) &addr_con, (socklen_t*)sizeof(addr_con)) == 0){
    	perror("TCP Connection accepting failed");
    	exit(EXIT_FAILURE);
    }else{
    	cout<<"TCP Connection accepted"<<endl;
    }*/
    
        
  
    while (1) {
        cout<<endl<<"Waiting for file"<<endl;
  
        // receive file name
        clear(buffer);
  
        n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&addr_con, &addrlen);
  
        fname = fopen(buffer, "r");
        cout<<endl<<"File Name Received:"<<buffer<<endl;
        if (fname == NULL)
            cout<<endl<<"File could not be opened"<<endl;
        else
            cout<<endl<<"File opened"<<endl;
  
  		
  	
  	
  	
  	
  	
        while (1) {
  
            // encrypting file
            cout<<"Encrypting file"<<endl;
            if (sendFile(fname, buffer, BUFFER_SIZE)) { 
            	sendto(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&addr_con, addrlen);
            	break;
            }
  
            // sending file
            cout<<"Sending File"<<endl;
            sendto(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&addr_con, addrlen);
            
            
             //Sending file size using TCP
  		//char info[32];
  		//strcpy(info, buffer);
  		//cout<<"File size sent using TCP ="<<sizeof(buffer)<<" bytes"<<endl;
  	
  		//send(sockfd, info, strlen(info), 0); 
            clear(buffer);
        }
        if (fname != NULL)
            fclose(fname);
            
       
            
    }
    return 0;
}

//////////////
//functions///
//////////////

// function to clear buffer
void clear(char* b){
    int i;
    for (i = 0; i < BUFFER_SIZE; i++)
        b[i] = '\0';
}

// function to encrypt the message
char Encrypt(char ch){
    return ch + cipherKey;
}

// function sending file
int sendFile(FILE* fname, char* buf, int s){
    int i, len;
    if (fname == NULL) {
        strcpy(buf, nofile);
        len = strlen(nofile);
        buf[len] = EOF;
        for (i = 0; i <= len; i++)
            buf[i] = Encrypt(buf[i]);
        return 1;
    }
  
    char ch, ch2;
    for (i = 0; i < s; i++) {
        ch = fgetc(fname);
        ch2 = Encrypt(ch);
        buf[i] = ch2;
        if (ch == EOF)
            return 1;
    }
    return 0;
}



