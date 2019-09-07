#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

int main(char **argc,int args)
{
  char mac[100];
  fp = popen("ifconfig -a | grep -Po 'HWaddr \K.*$'", "r");
  if (fgets(mac, 99, fp) != NULL) {
    printf("%s", mac);
  }
    
  //create socket
  int cfd = socket(AF_INET,SOCK_STREAM,0);
  struct sockaddr_in *serveraddr,*clientaddr;
  
  //creating memory to the server and client address structures
  serveraddr = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
  clientaddr = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
  
  // clear address memory
  bzero(serveraddr,sizeof(*serveraddr));
  bzero(clientaddr,sizeof(*serveraddr));
  
  // initialise address structure for server
  bzero(&servaddr,sizeof(servaddr));
  servaddr.sin_family=AF_INET;
  servaddr.sin_addr.s_addr=inet_addr("10.6.9.11");
  servaddr.sin_port=htons(7228);
  connect(cfd,(struct sockaddr*)&servaddr,sizeof(servaddr));

  char *buff;
  buff = (char*)malloc(100);
  
  
  while(1){
    read(cfd,buff,sizeof(buff));
    if(strcmp(buff,))
  }
    
  return 0;
}
