#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <netinet/in.h>
#include <linux/if_tun.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>
#include <stdarg.h>

#define BUFSIZE 2000

//tun_alloc: allocates or reconnects to a tun device. The caller must reserve enough space in *dev.

int tun_alloc(char *dev, int flags) {

  struct ifreq ifr;
  int fd, err;
  char *clonedev = "/dev/net/tun";

  if( (fd = open(clonedev , O_RDWR)) < 0 ) {
    perror("Opening /dev/net/tun");
    return fd;
  }

  memset(&ifr, 0, sizeof(ifr));

  ifr.ifr_flags = flags;

  if (*dev) {
    strncpy(ifr.ifr_name, dev, IFNAMSIZ);
  }

  if( (err = ioctl(fd, TUNSETIFF, (void *)&ifr)) < 0 ) {
    perror("ioctl(TUNSETIFF)");
    close(fd);
    return err;
  }

  strcpy(dev, ifr.ifr_name);

  return fd;
}

int read_n(int fd, char *buf, int n) {

  int nread, left = n;

  while(left > 0) {
    if ((nread = read(fd, buf, left)) == 0){
      return 0 ;      
    }else {
      left -= nread;
      buf += nread;
    }
  }
  return n;  
}

//usage: prints usage and exits.
void usage(char* progname) {
  fprintf(stderr, "Usage:\n");
  fprintf(stderr, "%s <ifacename> <server-ip> <server-port> \n", progname);
  exit(1);
}


int main(int argc, char* argv[])
{
   int tun_fd,sockfd,netfd,portno,status,clilen,maxfd,yes=1;
   uint16_t read_len, write_len, pack_len;
   struct sockaddr_in serv_addr, cli_addr;
   char buffer[BUFSIZE];
   fd_set master_socks;
   fd_set rd_socks;


   if(argc!=4)
    {
      usage(argv[0]);
      exit(0);
    }

    char tun_interface[IFNAMSIZ];
    strcpy(tun_interface,argv[1]);

    tun_fd=tun_alloc(tun_interface,IFF_TUN | IFF_NO_PI);

     if(tun_fd<0)
    {
      fprintf(stderr,"ERROR:: Connecting to tun Interface %s\n",argv[1]);
      exit(1);
    }
      fprintf(stdout,"Successfully connected to tun interface %s\n",argv[1]);

    sockfd=socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd<0)
    {
      fprintf(stderr,"ERROR:: Socket creation error\n");
      exit(1);
    }

    portno=atoi(argv[3]);
    bzero((char*)&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(portno);
    inet_aton(argv[2],&serv_addr.sin_addr);

    status=connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
    if(status<0)
    {
      fprintf(stderr,"ERROR:: Connection error\n");
      exit(1);
    }

    netfd = sockfd;
    fprintf(stdout,"CLIENT: Connected to server %s\n", inet_ntoa(serv_addr.sin_addr));

    FD_ZERO(&master_socks);
    FD_ZERO(&rd_socks);
    FD_SET(netfd,&master_socks);
    FD_SET(tun_fd,&master_socks);
    maxfd=(tun_fd>netfd)?tun_fd:netfd;

    fprintf(stderr,"Select monitoring tun_fd and netfd simultaneously.\n");

    while(1)
    {
      rd_socks=master_socks;
      status=select(maxfd+1,&rd_socks,NULL,NULL,NULL);
      if(status==-1)
	{
	  fprintf(stderr,"ERROR:: Select error.\n");
	  exit(1);
	}

	/* data from the network: read it, and write it to the tun interface.
        We need to read the length first, and then the packet */
      if(FD_ISSET(netfd,&rd_socks))
	{
	  read_len = read_n(netfd, (char *)&pack_len, sizeof(pack_len));
	  if(read_len == 0)
	    {
	      break;
	    }

	  read_len = read_n(netfd, buffer, ntohs(pack_len));
	  fprintf(stderr,"Read packet from network\n");
	  write_len = write(tun_fd, buffer, read_len);
	  fprintf(stderr,"Packet written to tun interface.\n");
	}

	/*data from tun: just read it and write it to the network*/
      if(FD_ISSET(tun_fd,&rd_socks))
	{
	   read_len = read(tun_fd, buffer, BUFSIZE);

	  pack_len = htons(read_len);
	  write_len = write(netfd, (char *)&pack_len, sizeof(pack_len));
	  fprintf(stderr,"Packet length wrriten to network.\n");
	  write_len = write(netfd, buffer, read_len);
	  fprintf(stderr,"Packet written to Network\n");
	}
    }
  return 0;
}


