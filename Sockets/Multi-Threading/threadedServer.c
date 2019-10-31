#include<pthread.h>
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdlib.h>
struct sockaddr_in serveraddr, clientaddr;
int client[100], numClients=0;
int serverfd;
//char buffRead[1000],buffWrite[1000];
pthread_t addClientID, clientID[100],serverID;
pthread_attr_t addClient_attr,client_attr[100],server_attr;
void *AddClients(void *param);
void *ListenToClient(void *param);
void *ListenToServer(void *param);
struct clientInfo{
    int clientfd,clientID;
};

int main(int argc,char *argv[])
{
    serverfd = socket(AF_INET,SOCK_STREAM,0);
    bzero(&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(7862);
    if(bind(serverfd,(struct sockaddr *)&serveraddr,sizeof(serveraddr))<0){
        perror("bind error");
    }
    listen(serverfd,5);
    pthread_attr_init(&addClient_attr);
    pthread_attr_init(&server_attr);
    pthread_create(&addClientID,&addClient_attr,AddClients,NULL);
    pthread_create(&serverID,&server_attr,ListenToServer,NULL);
    pthread_join(addClientID,NULL);
    pthread_join(serverID,NULL);
    close(serverfd);
    return 0;
}

void *AddClients(void *param){
    while(numClients<100){
        int len = sizeof(clientaddr);
        client[numClients] = accept(serverfd,(struct sockaddr *)&clientaddr,&len);
        struct clientInfo *c = (struct clientInfo *)malloc(sizeof(struct clientInfo));
        c->clientfd = client[numClients];
        c->clientID = numClients;
        numClients++;
        printf("\nclient %d connected\n",numClients-1);
        pthread_attr_init(&(client_attr[numClients]));
        pthread_create(&(clientID[numClients]),&(client_attr[numClients]),ListenToClient,(void *)c);
       // pthread_join(clientID[numClients],NULL);
    }
    pthread_exit(0); // this is of no use but still i don't want to take a risk
}

void *ListenToClient(void *param){
    struct clientInfo *c = (struct clientInfo *)param; 
    while(1){
        char message[1000];
        int messageSize = read(c->clientfd,message,1000);
        if(messageSize>0){
        printf("\nClient %d: %s\n",c->clientID,message);
        }
    }
    pthread_exit(0);
}

void *ListenToServer(void *param){
    while(1){
        char message[1000];
        printf("\nServer(enter client number followed by message): ");
        scanf("%s",message);
        char num[4];
        int i=0;
        while(message[i]!=':'){
            num[i] = message[i];
            i+=1;
           
        }
        num[i] ='\0';

        int clientNum = atoi(num);
        char actualMessage[1000];
        int j = 0;
        while(message[i]!='\0'){
            actualMessage[j] = message[i];
            i++; j++;
        }
        actualMessage[j] = '\0';
        write(client[clientNum],actualMessage,sizeof(actualMessage)); 
    }
    pthread_exit(0);
}