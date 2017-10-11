/*
 * File Name : Server1.c
 * Author : Salman
 * Roll No : 143050030
 */

#include<stdio.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<stdlib.h>
#include<string.h>

// Setting Buffer Size
#define MAX 80

// Function declaration to handle client interaction (chat)
void cliIntrctn(int);


// main starts here

int main(int argc, char* argv[])
{
	int sockFd,conFd,len,portNo;

	// Checking validity of command-line arguments and extracting port number to bind server on it.
	if(argc < 2)
    {
    	printf("Insufficient arguments in command line \n");
    	exit(0);
    }
    
	printf("didnt go inside if");
    	portNo=atoi(argv[2]);	// Converting String to Integer (argv)

	// Structure to be used for storing server and client details
	struct sockaddr_in servAddr,clientAddr;

	// Socket creation  with appropriate options
	sockFd=socket(AF_INET,SOCK_STREAM,0);

	// Checking error while socket creation
	if(sockFd==-1)
	{
		printf("Socket Creation :: Failed \n");
		exit(1);
	}
	else
		printf("Socket Creation :: Successful \n");

	// Setting SO_REUSEADDR socket option, so to have another connection on same port if its not in active use
	int optVal = 1;
	setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal));

	// Clearing buffer
	bzero(&servAddr,sizeof(servAddr));

	// Setting values to sockaddr_in structure for server
	servAddr.sin_family=AF_INET;
//	servAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	if(!inet_aton(argv[1], &servAddr.sin_addr))
	{
		printf("Error setting IP \n");
		exit(1);
	}
	servAddr.sin_port=htons(portNo);

	// bind socket to address+port specified in sockaddr_in structure
	// As well checking error (if any)
	if((bind(sockFd,(struct sockaddr*)&servAddr, sizeof(servAddr)))!=0)
	{
		printf("Socket Bind :: Failed \nPort might be in Use \n");

		exit(1);
	}
	else
		printf("Socket Bind :: Successful\n");

	// Listening on socket
	// Checking error for listen call
	if((listen(sockFd,5))!=0)
	{
		printf("Socket Listen :: Failed\n");
		exit(1);
	}
	else
		printf("Socket Listen :: Successful\n");

	// Accepting connect request from client
	len=sizeof(clientAddr);
	conFd=accept(sockFd,(struct sockaddr *)&clientAddr,&len);

	// Checking for errors during accept call
	if(conFd<0)
	{
		printf("Server Acccept :: Failed\n");
		exit(1);
	}
	else
		printf("Connected with client socket number %d \n", conFd);

	// Closing listening socket to avoid 2nd client request
	close(sockFd);

	// Function handling chat with client
	cliIntrctn(conFd);
}


void cliIntrctn(int conFd)
{
	// Buffer for receiving and sending contents
	char buff[MAX], reply[MAX]="OK Salman";

	// Infinite loop to communicate more than once
	// till client says "Bye"
	for(;;)
	{
		//Clearing Buffers
		bzero(buff,MAX);

		// Reading message from client into buffer
		read(conFd,buff,sizeof(buff));

		printf("Client socket %d sent message : %s" , conFd, buff);

		// Manipulating reply message based on client message
		if(memcmp(buff,"Bye",strlen("Bye")) == 0 && buff[3]=='\n')
			memcpy(reply,"Goodbye Salman",sizeof("Goodbye Salman"));

		// Writing back to client
		write(conFd,reply,sizeof(reply));

		printf("Replied to client %d \n", conFd);

		// Controlling connection to client based on message
		if(memcmp(buff,"Bye",strlen("Bye")) == 0 && buff[3]=='\n')
		{
			printf("Client said bye ; finishing \n");
			break;
		}
	}
}
