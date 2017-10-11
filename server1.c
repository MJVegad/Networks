#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
  int sockid,clientid,portno;
  struct sockaddr_in server_addr,client_addr;
  int client_addr_len;
  char recvbuff[500];
  char sendbuff[]={'O','K',' ','M','I','H','I','R','\0'};
  char endmsg[]={'G','o','o','d','b','y','e',' ','M','I','H','I','R','\0'};
  int yes=1,errno;

   if (argc < 2) {
       fprintf(stderr,"usage %s <server-ip-addr> <server-port>\n", argv[0]);
       exit(0);
   }
   portno=atoi(argv[1]);

  //creating a socket whose socket descriptor is sockid
  sockid=socket(AF_INET,SOCK_STREAM,0);

  //initializing members of structure sockaddr_in
  memset(&server_addr,0,sizeof(server_addr));
  memset(&client_addr,0,sizeof(client_addr));

  //specifying server address
  server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
  server_addr.sin_port=htons(portno);

  //to reuse the port immedietly for bind after process on that terminates
  if((setsockopt(sockid,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)))==-1)
     printf("Cannot bind socket: [%d]:[%s]\n", errno, strerror(errno));

  //bind local address to the port
  int status1=bind(sockid,(struct sockaddr*)&server_addr,sizeof(server_addr));
  if(status1<0)
    {
      printf("Cannot bind socket: [%d]:[%s]\n", errno, strerror(errno));
      printf("bind unsuccessful.\n");
      printf("In case process is still running on this port.Please use another port & then execute.\nuse <fg> to see background process.\n");
      exit(1);
    }
  else
     printf("bind successful.\n");

  //to listen to accept new connection
  int status2=listen(sockid,1);
  if(status2<0)
     printf("not listening for connections.");
  else
     printf("listening for connections.\n");

     client_addr_len=sizeof(client_addr);
  if((clientid=accept(sockid,(struct sockaddr*)&client_addr,&client_addr_len))<0)
     printf("accept failed.");

     printf("connected with client socket no:%d\n",clientid);

     //to close the listening socket so no other client can connect
     close(sockid);

  while(1)
  {
    //to initialize recvbuff
    bzero(recvbuff,500);

    //int status3=recv(clientid,recvbuff,sizeof(recvbuff),0);
    int status3=read(clientid,recvbuff,sizeof(recvbuff));
    if(status3<0)
      printf("receivining error.\n");

      //printf("Client socket %d sent message:%s",clientid,recvbuff);
    if(recvbuff[0]=='B'&&recvbuff[1]=='y'&&recvbuff[2]=='e'&&recvbuff[3]=='\n')
       {
         printf("Client socket %d sent message:%s",clientid,recvbuff);
         int status4=write(clientid,endmsg,sizeof(endmsg));
         if(status4<0)
            printf("sending error.\n");
            printf("Replied to client 4.\n");
            printf("Client said bye;finishing..\n");
            close(clientid);
            exit(0);
       }
     else
       {
         printf("Client socket %d sent message:%s",clientid,recvbuff);
         int status5=write(clientid,sendbuff,sizeof(sendbuff));
         if(status5<0)
           printf("sending error.\n");
           printf("Replied to client 4.\n");

       }
   }


}






