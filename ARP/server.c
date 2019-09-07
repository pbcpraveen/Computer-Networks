#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<errno.h>
#include <stdlib.h>
#include <string.h>
#include<signal.h>
#include<math.h>
#define backlog 4

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
    (*servaddr).sin_addr.s_addr=INADDR_ANY;
    (*servaddr).sin_port=htons(4431);
}

void bindAndListen(int *sockfd,struct sockaddr_in *servaddr)
{
    if(bind(*sockfd,(struct sockaddr*)servaddr,sizeof(*servaddr))<0)     //bind the socket with the local protocol addr
    perror("Bind Error");                                               //which is a combination of ip addr and port no.

    listen(*sockfd,backlog);     //sets the state of the socket here sockfd to listening state. backlog --> no. of clients server can listen to.
}

void makeArpPacket(char *packet,char *ipAddr)
{
    char str[100]="127.0.0.1|00:24:be:c6:ce:a7|";

    strcat(str,ipAddr);
    strcpy(packet,str);

    //printf("\nPacket = %s",packet);
}

void func(int sockfd,int newfd,struct sockaddr_in *cliaddr,int *len,char *buff,char *packet,char *ipAddr)
{

    int found=0,n=0;
    newfd=accept(sockfd,(struct sockaddr*)cliaddr,len);

    printf("\nClient(s) connected !!!!\n");     //There may be more than one client

    strcpy(buff,packet);
    printf("\nServer: Sending Packet %s\n",packet);
    n=write(newfd,packet,strlen(packet));
    //printf("\nNo. of bytes transferred = %d\n",n);

    n=read(newfd,buff,100);

    found=1;
    printf("\nClient: %s",buff);
    if(strcmp(buff,"end")==0)
    {
        found=0;
        goto END;
    }
    strcat(buff,"|10101011");

    n=write(newfd,buff,strlen(buff));
    printf("\nARP Packet sent!!!\n");
    strcpy(buff,"");

    END:
    if(!found)
    {
        printf("\nThis client does not have the destination IP address( %s )\n",ipAddr);
    }
    printf("\nClosing this client's connection...\n");
    close(newfd);

}
int main(int argc,char **argv)
{

    int len;
    int sockfd,newfd,n,r;
    struct sockaddr_in servaddr,cliaddr;
    char buff[1024];
    char str[1000];

    createSocket(&sockfd);
    initServer(&servaddr);
    bindAndListen(&sockfd,&servaddr);

    //getInputs
    char ipAddr[50];
    GETIP:
    printf("\nEnter the destination IP: \n");
    gets(ipAddr);
    if(inet_addr(ipAddr)==-1)
    {
        printf("\nInvalid IP\n");
        goto GETIP;
    }
    //printf("\nIP addr: %s",ipAddr);
    char packet[100];
    makeArpPacket(packet,ipAddr);
    //

    //start accepting
    len=sizeof(cliaddr);

    int found=0,pid;

    fork();             //This line controls the concurrent client.
                        //By the above line - I have made four clients to chat with server at the same time.
                        //remove that line, you will then have only 2 clients to chat with the server at the same time.
    //for(int i=0;i<(int)ceil(backlog/2);i++)
    {
        pid=fork();

        if(pid==0)
        {
            printf("********************************");
            func(sockfd,newfd,&cliaddr,&len,buff,packet,ipAddr);
            pid=fork();
            /*if(pid==0)
            {
                printf("********************************");
                func(sockfd,newfd,&cliaddr,&len,buff,packet,ipAddr);
                printf("********************************");
                close(newfd);

            }
            else
            {
                printf("********************************");
                func(sockfd,newfd,&cliaddr,&len,buff,packet,ipAddr);
                printf("********************************");
                close(newfd);

            }*/
            printf("********************************");
            close(newfd);

        }
        else
        {
            printf("#######################################");
            func(sockfd,newfd,&cliaddr,&len,buff,packet,ipAddr);
            printf("#######################################");
            close(newfd);
            close(newfd);

        }
    }


    close(sockfd);
    return 0;
}
