#include<pthread.h>
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdlib.h>

int clientfd,serverfd;
struct sockaddr_in serveraddr, clientaddr;
pthread_t clientSend,clientReceive;
pthread_attr_t clientSend_attr,clientReceive_attr;
void *ClientSendMessage(void *param);
void *ClientReceiveMessage(void *param);

int main(int argc,char *argv[])
{
    clientfd = socket(AF_INET,SOCK_STREAM,0);
    bzero(&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
    serveraddr.sin_port = htons(7862);
    connect(clientfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
    pthread_attr_init(&clientSend_attr);
    pthread_attr_init(&clientReceive_attr);
    pthread_create(&clientSend,&clientSend_attr,ClientSendMessage,NULL);
    pthread_create(&clientReceive,&clientReceive_attr,ClientReceiveMessage,NULL);
    pthread_join(clientSend,NULL);
    pthread_join(clientReceive,NULL);
    close(clientfd);
    return 0;
}

void *ClientSendMessage(void *param){
    while(1){
        char message[1000];
        printf("\nClient: ");
        scanf("%s",message);
        write(clientfd,message,sizeof(message));
    }
}
void *ClientReceiveMessage(void *param){
    while(1){
        char message[1000];
        int messageSize = read(clientfd,message,1000);
        if(messageSize>0){
            printf("\nServer: %s\n",message);
        } 
    }
}