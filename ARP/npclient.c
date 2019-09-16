#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

int main(char **argc,int args)
{
  char *mac, *ip;
  mac = (char*) malloc(100);
  ip = (char*) malloc(100);
  for(int i = 0; i<100;i++) ip[i] = '\0';
  FILE* fp = popen("fconfig -a | grep -Po 'HWaddr \\K.*$'", "r");
  if (fgets(mac, 99, fp) != NULL) {
    printf("The clients mac is : %s\n", mac);
  }
  mac[17] = '\0';

  
  fp =  popen("ip route get 8.8.4.4 | head -1 | awk '{print $7}'","r");
   if (fgets(ip, 99, fp) != NULL) {
     printf("The client IP is : %s\n",ip);
     ip[strlen(ip)-1] = '\0';
   }
  
  fclose(fp);
 
  //create socket
  int cfd = socket(AF_INET,SOCK_STREAM,0);
  struct sockaddr_in serveraddr,clientaddr;

  
  // clear address memory
  bzero(&serveraddr,sizeof(struct sockaddr_in));
  bzero(&clientaddr,sizeof(struct sockaddr_in));
  
  // initialise address structure for server
  bzero(&serveraddr,sizeof(serveraddr));
  serveraddr.sin_family=AF_INET;
  serveraddr.sin_addr.s_addr=inet_addr("127.0.0.1");
  serveraddr.sin_port=htons(7891);
  connect(cfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
 
  char *buff;
  char* buff1 = "null";
  char* buff2 = "null1";
  buff = (char*)malloc(100);

  
  //read the request from the server.
  int n1 = 0;
  while(n1 == 0)
    n1 = read(cfd,buff,100);
  buff[n1] = '\0';
  printf("Received IP request is %s,%p\n",buff,ip);

  // compare the client ip with the host ip
  if(strcmp(buff,ip)==0)
    {
      printf("Server is ip matched....\n");
      printf("Sending the mac address to server to initiate the communication\n");
      write(cfd,mac,strlen(mac));
      read(cfd,buff,100);
      printf("Received message: %s\n",buff);
    }
  
  else {
    printf("ip mismatch\n");
      write(cfd,buff1,(sizeof(buff1)));
    }
    close(cfd); 
  return 0;
}
