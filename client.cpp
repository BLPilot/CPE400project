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
  
#define PROTOCOL 0
#define ADDRESS "127.0.0.1"
#define PORT 15050
#define BUFFER_SIZE 1024
#define cipherKey 2


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
    addr_con.sin_port = htons(PORT);
    addr_con.sin_addr.s_addr = inet_addr(ADDRESS);
    char buffer[BUFFER_SIZE];
    FILE* fname;
    FILE* input;
    char input_name[100];
    sprintf(input_name, "Input_file.txt");
    
  
    // creating socket
    sockfd = socket(AF_INET, SOCK_DGRAM, PROTOCOL);
                    
   if(sockfd <0){
   		perror("Socket creation failed");
   		exit(EXIT_FAILURE);
   }else{
		cout<<"Socket created succesfully"<<endl;
   }
   
   //TCP connection channel, creating connection

   /*if(connect(sockfd, (struct sockaddr*) &addr_con, sizeof(addr_con)) < 0){
    	perror("TCP Connection failed");
    	exit(EXIT_FAILURE);
    }else{
    	cout<<"TCP Connection succesful"<<endl;
    }*/
                    
                    
    while (1) {
        cout<<endl<<"Please enter file:"<<endl;
        cin>>buffer;
        sendto(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&addr_con, addrlen);
        
        
        
  
       
       	
  
        while (1) {
            // receiving file
            cout<<"Receiving file"<<endl;
            clear(buffer);
            n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&addr_con, &addrlen);
                             
            //Receiving file size from server using TCP
            //doesnt receive data
        	//char received[32];
        	//int size = read(sockfd, received, 32); 
        	//cout<<"File data size received from TCP :"<<sizeof(buffer)<<" bytes"<<endl;            
                              
            
             cout<<"Size of file being sent: "<<strlen(buffer)<<endl;
            	
            // decrypting file
            cout<<"decrypting file"<<endl;
            
            cout<<endl<<"Data Received: "<<endl;
            if (receiveFile(buffer, BUFFER_SIZE, input_name)) {
                break;
            }
            
            
            
           
        }
        
        
     
        
        cout<<endl;
    }
    return 0;
}

// function to clear buffer
void clear(char* b){
    int i;
    for (i = 0; i < BUFFER_SIZE; i++)
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
