#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(char **argc,int args)
{
  //create socket
  int sfd = socket(AF_INET,SOCK_STREAM,0);
  struct sockaddr_in *serveraddr,*clientaddr;
  
  //creating memory to the server and client address structures
  serveraddr = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
  clientaddr = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
  
  // clear address memory
  bzero(serveraddr,sizeof(*serveraddr));
  bzero(clientaddr,sizeof(*serveraddr));
  
  // initialise address structure for server
  (*serveraddr).sin_family=AF_INET;
  (*serveraddr).sin_addr.s_addr=INADDR_ANY;
  (*serveraddr).sin_port=htons(7889);

  //bind the socket structure to its address
  if (bind(sfd,serveraddr,sizeof(*serveraddr))<0)
    {
      perror("bind error");
      exit(0);
    }

  //listen to incoming client connection
  listen(sfd,10);

  char ch = 'y';
  int i = 0;
  int fd[10];
  char iplist[10][100];
  struct sockaddr_in clist[10];
  while(ch == 'y')
    {
      int f1 = accept(sfd,clientaddr,sizeof(*clientaddr));
      fd[i] = f1;
      clist[i] = *clientaddr;
      printf("new connection recieved");
      char *ip = inet_ntoa(clist[i].sin_addr);
      strcpy(iplist[i],ip);
      i++;
      printf(" %s\n",ip);
      printf("\nenter y to connect to another client:");
      scanf("%c",ch);
    }

  char* ipreq;
  char* macreply;
  macreply = (char*) malloc(100);
  ipreq = (char*) malloc(100);
  printf("enter the ip to be reuested");
  scanf("%s",ipreq);
  int total_connections =i;
  i = 0;
  while(i<total_connections){
    write(fd[i],ipreq,sizeof(*ipreq));
    printf("request sent to:\t%s\n",clist[i]);
    read(fd[i],macreply,sizeof(*macreply));

    if(strcmp(macreply,"null") != 0){
      printf("\nrecieved mac of %s:\t%s\n",clist[i],macreply);
      char* buff = "10110110";
      write(fd[i],buff,sizeof(buff));
    }
    else{
      char* buff = "ip mismatch";
      write(fd[i],buff,sizeof(buff));
    }
  }

  close(sfd);
  return 0;
}
