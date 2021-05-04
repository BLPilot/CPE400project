// client code for UDP socket programming
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
  
#define IP_PROTOCOL 0
#define IP_ADDRESS "127.0.0.1" // localhost
#define PORT_NO 15050
#define NET_BUF_SIZE 32
#define cipherKey 2
#define sendrecvflag 0

using namespace std;
  
void clear(char* b);
char Decrypted(char ch);
int receiveFile(char* buf, int s, char * input_name);
  

  
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
    addr_con.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    char net_buf[NET_BUF_SIZE];
    FILE* fp;
    FILE* input;
    char input_name[100];
    sprintf(input_name, "Input_file.txt");
  
    // creating socket
    sockfd = socket(AF_INET, SOCK_DGRAM,
                    IP_PROTOCOL);
                    
   if(sockfd <0){
   		perror("Socket creation failed");
   		exit(EXIT_FAILURE);
   }else{
		cout<<"Socket created succesfully"<<endl;
   }
                    
                    
    while (1) {
        cout<<endl<<"Please enter file:"<<endl;
        cin>>net_buf;
        sendto(sockfd, net_buf, NET_BUF_SIZE,
               sendrecvflag, (struct sockaddr*)&addr_con,
               addrlen);
  
       	cout<<endl<<"Data Received: "<<endl;
  
        while (1) {
            // receive
            clear(net_buf);
            n = recvfrom(sockfd, net_buf, NET_BUF_SIZE,
                              sendrecvflag, (struct sockaddr*)&addr_con,
                              &addrlen);
                              
            
             
          
			
            
            
            // process
            if (receiveFile(net_buf, NET_BUF_SIZE, input_name)) {
			
				
                
                break;
            }
            
           
        }
        cout<<endl<<endl;
    }
    return 0;
}

// function to clear buffer
void clear(char* b){
    int i;
    for (i = 0; i < NET_BUF_SIZE; i++)
        b[i] = '\0';
}

// function for decryption of message
char Decrypted(char ch){
    return ch - cipherKey;
}

// function to receive file
int receiveFile(char* buf, int s, char * input_name){
    int i;
    char ch;
    ofstream out(input_name);
    
    for (i = 0; i < s; i++) {
        ch = buf[i];
        ch = Decrypted(ch);
        if (ch == EOF)
            return 1;
        else
            printf("%c", ch);
            out << ch;
    }
    
    out.close();
    return 0;
}
