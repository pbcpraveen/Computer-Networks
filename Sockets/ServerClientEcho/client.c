#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
int main(int argc,char **argv)
{
int len;
int sockfd,n;
struct sockaddr_in servaddr,cliaddr;
char str[1000];
char buff[1024];
sockfd=socket(AF_INET,SOCK_STREAM,0);
if(sockfd<0)
perror("cannot create socket");
bzero(&servaddr,sizeof(servaddr));
servaddr.sin_family=AF_INET;
servaddr.sin_addr.s_addr=inet_addr(argv[1]);
servaddr.sin_port=htons(7228);
connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
//Sending Message
printf("Enter the message:  ");
scanf("%s",buff);
n=write(sockfd,buff,sizeof(buff));
printf("Sent Message: %s\n",buff);
n=read(sockfd,buff,sizeof(buff));
printf("\nReceived Message is: \t%s ",buff);
printf("\n");
close(sockfd);
return 0;
}
