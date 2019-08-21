;
  struct sockaddr_in servaddr,clientaddr;
  char buff[1024];
  struct hostent *hostp;
  char *hostaddrp;
  char str[1000];
  sockfd=socket(AF_INET,SOCK_DGRAM,0);
  if(sockfd<0)
    perror("cannot create socket");
  bzero(&servaddr,sizeof(servaddr));
  servaddr.sin_family=AF_INET;
  servaddr.sin_addr.s_addr=INADDR_ANY;
  servaddr.sin_port=htons(7228);
  if(bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr))<0)
    perror("Bind error");
  listen(sockfd,4);
  pid_t i = fork();

  len =sizeof(clientaddr);
  if(i>0){
    while(1){
      int n,len;
      n = recvfrom(sockfd,(char*)buff,100,MSG_WAITALL,(struct sockaddr*)&clientaddr,&len);
      hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, sizeof(clientaddr.sin_addr.s_addr), AF_INET);
      hostaddrp = inet_ntoa(clientaddr.sin_addr);
      buff[n] = '\0';
      printf("client %s : %s \n",hostaddrp,buff);
      bzero(&clientaddr,sizeof(clientaddr));
    }
  }
  else
    {
      printf("Server: ");
      scanf("%s",buff);
      int sn = send(sockfd,(char*)buff,sizeof(buff),0);
      bzero(buff,sizeof(buff));
    }
  
  close(sockfd);
  close(newfd);
  return 0;
  
}
