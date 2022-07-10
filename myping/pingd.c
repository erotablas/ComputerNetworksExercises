/* UDP ping server */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>

int main(int argc, char**argv) { 
  int sockfd, n;
  extern int errno;
  struct sockaddr_in local_addr, remote_addr;
  socklen_t len = sizeof(struct sockaddr_in );
  char mesg[32];
   
  if(argc < 2) { 
    printf("Use: pingd listening_PORT");
	  return 0;
  }	 

  if((sockfd=socket(PF_INET,SOCK_DGRAM,0)) <0) { 
    printf("\nError in socket opening ... exit!");
    return -1;
  }

  memset(&local_addr,0,sizeof(local_addr));
  local_addr.sin_family = AF_INET;
  local_addr.sin_port=htons(atoi(argv[1]));

  if(bind(sockfd, (struct sockaddr *) &local_addr, len)<0) { 
    printf("\nBinding error! Error code n.%d \n",errno);
    return -1;
  }

  for (;;) { 
    n = recvfrom(sockfd,mesg,32,0,(struct sockaddr *) &remote_addr,&len);
	  printf("From IP:%s Port:%d  \n", inet_ntoa(remote_addr.sin_addr),  ntohs(remote_addr.sin_port));
	  sendto(sockfd,mesg,n,0,(struct sockaddr *)&remote_addr,len);
  }
}
