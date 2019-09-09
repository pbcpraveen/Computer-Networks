#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
//#include<strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

int main(char **argc,int args)
{
  printf("start");
  char *mac, *ip;
  mac = (char*) malloc(100);
  ip = (char*) malloc(100);
  for(int i = 0; i<100;i++) ip[i] = '\0';
  printf("before");
  FILE* fp = popen("ifconfig -a | grep -Po 'HWaddr \\K.*$'", "r");
  if (fgets(mac, 99, fp) != NULL) {
    printf("%s", mac);
  }
  printf("after");
  
  fp =  popen("ip route get 8.8.4.4 | head -1 | awk '{print $7}'","r");
   if (fgets(ip, 99, fp) != NULL) {
     printf("step3");
     printf("%s",ip);
   
   }

  fclose(fp);
  printf("step3");
  //create socket
  int cfd = socket(AF_INET,SOCK_STREAM,0);
  struct sockaddr_in *serveraddr,*clientaddr;
  printf("step2");
  //creating memory to the server and client address structures
  serveraddr = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
  clientaddr = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
  printf("nitin");
  // clear address memory
  bzero(serveraddr,sizeof(struct sockaddr_in));
  bzero(clientaddr,sizeof(struct sockaddr_in));
  
  // initialise address structure for server
  bzero(serveraddr,sizeof(*serveraddr));
  (*serveraddr).sin_family=AF_INET;
  (*serveraddr).sin_addr.s_addr=inet_addr("10.6.9.11");
  (*serveraddr).sin_port=htons(7889);
  connect(cfd,(struct sockaddr*)serveraddr,sizeof((*serveraddr)));
 
  char *buff;
  char* buff1 = "null";
   char* buff2 = "null1";
  buff = (char*)malloc(100);
  //write(cfd,buff2,100);
  while(read(cfd,buff,100)<=0) ;
  printf("%s,%d",buff,strlen(buff));/*
  if(strcmp(buff,ip)==0)
    {
      write(cfd,mac,(sizeof(mac)));
      read(cfd,buff,100);
      printf("Received message: %s",buff);
    }
  
  else {
      write(cfd,buff1,(sizeof(buff1)));
    }
    close(cfd); */
  return 0;
}
