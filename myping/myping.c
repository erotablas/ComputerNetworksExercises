/*  UDP ping client */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>

int main(int argc, char**argv) { 
  int sockfd, n;
  struct sockaddr_in remote_addr;
  char sendline[32];
  struct timespec Tstart; 

  socklen_t len = sizeof(struct sockaddr_in);

     
  if (argc != 3) { 
    printf("usage:  UDPclient <remote_IP remote_port>\n");
    return 1;
  }

  if((sockfd=socket(AF_INET,SOCK_DGRAM,0)) <0) { 
      printf("\nError in socket opening ... exit!");
      return -1;
    }


  if (fork()==0) {
    memset(&remote_addr,0,len);
    remote_addr.sin_family = AF_INET;
    inet_pton(AF_INET, argv[1], &(remote_addr.sin_addr)); 
    remote_addr.sin_port = htons(atoi(argv[2]));
   
    for (int n=0; n<10; ++n) {  
      clock_gettime(CLOCK_REALTIME, &Tstart);
      memcpy(sendline,&n,4);
      memcpy(sendline+4, &Tstart, sizeof(Tstart));
      sendto(sockfd,sendline,20,0, (struct sockaddr *)&remote_addr, len);
      sleep(1);
	    printf("Sending ping n=%d\n",n);  
    }
  }else { 
    int n2;
    long sec, nsec;
    struct timespec T1, T2;
    char recvline[32];
    
    for(;;){
      recvfrom(sockfd,recvline,20,0, (struct sockaddr *) &remote_addr, &len);
      clock_gettime(CLOCK_REALTIME, &T2);
      memcpy(&n2, recvline,4);
      memcpy(&T1,recvline+4, sizeof(T1));
      sec = T2.tv_sec - T1.tv_sec; 
      nsec = T2.tv_nsec - T1.tv_nsec; 
      printf("Ping from:%s Port:%d Ping:%d Time: sec=%ld nsec=%ld \n", inet_ntoa(remote_addr.sin_addr),  ntohs(remote_addr.sin_port),n2, sec, nsec);
    }
  }
}
