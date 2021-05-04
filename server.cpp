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
  
#define IP_PROTOCOL 0
#define PORT_NO 15050
#define NET_BUF_SIZE 32
#define cipherKey 2
#define sendrecvflag 0
#define nofile "File Not Found!"

using namespace std;
  
void clear(char* b);
char Encrypt(char ch); 
int sendFile(FILE* fp, char* buf, int s);
  

  
// driver code
int main()
{
    int sockfd;
    int n;
    struct sockaddr_in addr_con;
    socklen_t addrlen = sizeof(addr_con);
    
    //filling server information
    addr_con.sin_family = AF_INET;
    addr_con.sin_port = htons(PORT_NO);
    addr_con.sin_addr.s_addr = INADDR_ANY;
    char net_buf[NET_BUF_SIZE];
    FILE* fp;
  
    // creating socket
    sockfd = socket(AF_INET, SOCK_DGRAM, IP_PROTOCOL);
    
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
        
  
    while (1) {
        cout<<endl<<"Waiting for file"<<endl;
  
        // receive file name
        clear(net_buf);
  
        n = recvfrom(sockfd, net_buf,
                          NET_BUF_SIZE, sendrecvflag,
                          (struct sockaddr*)&addr_con, &addrlen);
  
        fp = fopen(net_buf, "r");
        cout<<endl<<"File Name Received:"<<net_buf<<endl;
        if (fp == NULL)
            cout<<endl<<"File could not be opened"<<endl;
        else
            cout<<endl<<"File opened"<<endl;
  
        while (1) {
  
            // process
            if (sendFile(fp, net_buf, NET_BUF_SIZE)) {
                sendto(sockfd, net_buf, NET_BUF_SIZE,
                       sendrecvflag, 
                    (struct sockaddr*)&addr_con, addrlen);
                break;
            }
  
            // send
            sendto(sockfd, net_buf, NET_BUF_SIZE,
                   sendrecvflag,
                (struct sockaddr*)&addr_con, addrlen);
            clear(net_buf);
        }
        if (fp != NULL)
            fclose(fp);
    }
    return 0;
}

//////////////
//functions///
//////////////

// function to clear buffer
void clear(char* b){
    int i;
    for (i = 0; i < NET_BUF_SIZE; i++)
        b[i] = '\0';
}

// function to encrypt the message
char Encrypt(char ch){
    return ch + cipherKey;
}

// function sending file
int sendFile(FILE* fp, char* buf, int s){
    int i, len;
    if (fp == NULL) {
        strcpy(buf, nofile);
        len = strlen(nofile);
        buf[len] = EOF;
        for (i = 0; i <= len; i++)
            buf[i] = Encrypt(buf[i]);
        return 1;
    }
  
    char ch, ch2;
    for (i = 0; i < s; i++) {
        ch = fgetc(fp);
        ch2 = Encrypt(ch);
        buf[i] = ch2;
        if (ch == EOF)
            return 1;
    }
    return 0;
}



