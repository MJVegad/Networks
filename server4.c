#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

void doProcessing(int);
int main(int argc, char* argv[])
{
  int sockid,clientid,portno,max_ids=10,i;
  struct sockaddr_in server_addr,client_addr;
  int client_addr_len;
  char recvbuff[500];
  char sendbuff[]={'O','K',' ','M','I','H','I','R','\0'};
  char endmsg[]={'G','o','o','d','b','y','e',' ','M','I','H','I','R','\0'};
  fd_set master_fds;
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

  //listening for connection requests
  int status2=listen(sockid,1);
  if(status2<0)
     printf("not listening for connections.");
  else
     printf("listening for connections.\n");

  client_addr_len=sizeof(client_addr);

  FD_ZERO(&master_fds);
  //add sockid to master_fds
  FD_SET(sockid,&master_fds);
  max_ids=sockid;

  fd_set read_fds;
  int status7;

  printf("starting select loop. fdmax  %d\n",max_ids);

   for(;;)
  {

   read_fds = master_fds;
   if(select(max_ids+1, &read_fds, NULL, NULL, NULL) == -1)
   {
    perror("Server-select() error lol!");
    exit(1);
   }

  //looking for existing socket descriptors in read_fds
  for(i = 0; i <= max_ids; i++)
   {
    if(FD_ISSET(i, &read_fds))
    {
     //new connection requests arrived on sockid 
     if(i == sockid)
      {
       if((clientid = accept(sockid, (struct sockaddr *)&client_addr, &client_addr_len)) == -1)
      {
       perror("accept failed.");
      }
      else
     {
      //add new connection des. to master_fds
      FD_SET(clientid, &master_fds);
      if(clientid > max_ids)
      {
       max_ids = clientid;
      }
      printf("In select loop:socket %d is ready to read\n",clientid);
     }
   }
   else
   {
    //activity on some other socket apart from sockid(on any client socket)

    //to initialize recvbuff
    bzero(recvbuff,500);

    int status3;
    if((status3=read(i,recvbuff,sizeof(recvbuff)))<=0)
   {
     if(status3 == 0)
      printf("connetion closed on socket:%d",i);
     else
      perror("recevier error.");

     close(i);
     FD_CLR(i, &master_fds);
    }
     else
    {

     if(recvbuff[0]=='B'&&recvbuff[1]=='y'&&recvbuff[2]=='e'&&recvbuff[3]=='\n')
       {
         printf("Client socket %d sent message:%s",i,recvbuff);
         int status4=write(i,endmsg,sizeof(endmsg));
         if(status4<0)
           printf("sending error.\n");
           printf("Replied to client %d.\n",i);
           printf("Client said bye;finishing..\n");
           close(i);
           FD_CLR(i, &master_fds);
       }
     else
       {
         printf("Client socket %d sent message:%s",i,recvbuff);
         int status5=write(i,sendbuff,sizeof(sendbuff));
         if(status5<0)
           printf("sending error.\n");
           printf("Replied to client %d.\n",i);

       }
      }
     }
    }
   }
  }
 return 0;
}




