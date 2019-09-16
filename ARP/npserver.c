#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(char **argc,int args)
{
  //create socket
  int sfd = socket(AF_INET,SOCK_STREAM,0);
  struct sockaddr_in serveraddr,clientaddr;
  

  // clear address memory
  bzero(&serveraddr,sizeof(struct sockaddr_in));
  bzero(&clientaddr,sizeof(struct sockaddr_in));
  
  // initialise address structure for server
  serveraddr.sin_family=AF_INET;
  serveraddr.sin_addr.s_addr=INADDR_ANY;
  serveraddr.sin_port=htons(7892);

  //bind the socket structure to its address
  if (bind(sfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr))<0)
    {
      perror("bind error");
      exit(0);
    }

  //listen to incoming client connection
  listen(sfd,10);

  int ch = 1;
  int i = 0;
  int fd[10];
  char iplist[10][100];
  struct sockaddr_in clist[10];
  while(ch == 1)
    {
      int len = sizeof(struct sockaddr_in);
      int f1 = accept(sfd,(struct sockaddr *)&clientaddr,&len);
      fd[i] = f1;
      printf("%d",f1);
      clist[i] = clientaddr;
      printf("\nnew connection received: ");
      struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&clientaddr;
      struct in_addr ipAddr = pV4Addr->sin_addr;
      char str1[INET_ADDRSTRLEN];
      inet_ntop( AF_INET, &ipAddr, str1, INET_ADDRSTRLEN );
      strcpy(iplist[i],str1);
      i++;
      printf(" %s\n",str1);
      sleep(2);
      printf("\nenter 1 to connect to another client: ");
      scanf("%d",&ch);
      //bzero(&clientaddr,sizeof(struct sockaddr_in));
    }

  char* ipreq;
  char* macreply;
  macreply = (char*) malloc(100);
  ipreq = (char*) malloc(100);
  printf("Enter the ip to be requested: ");
  scanf("%s",ipreq);
  printf("Requested IP is : %s\n",ipreq);
  int total_connections =i;
  i = 0;
  while(i<total_connections){
    write(fd[i],ipreq,100);
    printf("request sent to:\t%s\n",iplist[i]);
    sleep(5);
    int nr = read(fd[i],macreply,99);
    macreply[nr] = '\0';
    if(strcmp(macreply,"null") != 0){
      printf("\nrecieved mac of %s:\t%s\n",iplist[i],macreply);
      char buff[] = "10110110";
      write(fd[i],buff,sizeof(buff));
    
    }
    else{
      char buff[] = "ip mismatch";
      write(fd[i],buff,sizeof(buff));
    }
i++;
  }

  close(sfd);
  return 0;
}
