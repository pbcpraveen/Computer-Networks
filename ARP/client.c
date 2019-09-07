#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<errno.h>
#define backlog 2
#include <string.h>
int createSocket(int *sockfd)
{
    *sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(*sockfd<0)
    {
        perror("\nError in creating socket\n");
    }
}//esocket

void initServer(struct sockaddr_in *servaddr)
{

    bzero(servaddr,sizeof(*servaddr));
    (*servaddr).sin_family=AF_INET;
    (*servaddr).sin_addr.s_addr=inet_addr("127.0.0.1");     //Enter the system's IP address.
    (*servaddr).sin_port=htons(4431);
}

void getIP(char *recvIP,char *buff)
{
    char *token;
    token=strtok(buff,"|");

    while(token!=NULL)
    {
        bzero(recvIP,sizeof(recvIP));
        strcpy(recvIP,token);
        token=strtok(NULL,"|");
    }

    //printf("\nRecvIP: %s",recvIP);
}
int main(int argc,char **argv)
{
    int len;
    int sockfd,n,r;
    struct sockaddr_in servaddr,cliaddr;

    char str[1000];
    char buff[1024];

    createSocket(&sockfd);
    initServer(&servaddr);

    r=connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    printf("Connect returned %d\n",r);

    if(r==-1)
    {
        return 0;
    }

    n=read(sockfd,buff,sizeof(buff));
    buff[n]='\0';
    printf("\nServer: Sending ARP Packet\n%s\n",buff);

    char packet[50];
    strcpy(packet,buff);
    char recvIP[50],myIP[50];
    getIP(recvIP,buff);

    int counter=0;

    strcat(packet,"|45-DA-62-21-1A-B2");   //some MAC address    //later get it from user

    IP:
    printf("\nEnter Your IP: ");
    gets(myIP);
    if(inet_addr(myIP)==-1)
    {
        printf("\nInvalid IP\n");
        goto IP;
    }

    if(strcmp(recvIP,myIP)==0)
    {
        n=write(sockfd,packet,strlen(packet));
        n=read(sockfd,buff,sizeof(buff));
        buff[n]='\0';
        printf("\tARP Packet Received: %s\n",buff);
        strcpy(buff,"");
    }

    strcpy(buff,"end");
    n=write(sockfd,buff,sizeof("end"));


    printf("\nClosing....\n");
    close(sockfd);
    return 0;
}


