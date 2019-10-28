#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<stdbool.h>
#include<math.h>
#define MAXLINE 1024
int countbits(long num)
{ 
  int r,count=0;
  while(num>0)
    {
      num=num/10;
      count++;
    }
  return count;
}
int binary(int num)
{
  int bin=0,r;
  int i=0;
  while(num>0)
    {
      r=num%2;
      bin+=r*pow(10,i);
      num/=2;
      i++;
    }
  return bin;
}
int ispresent(int num,int pos)
{
  int rem;
  for(int i=0;i<pos;i++)
    {
      rem=num%10;
      num=num/10;
    }
  if(rem==1)
    return 1;
  else
    return 0;
}
int isapower2(int n)
{
  if(ceil(log2(n)) == floor(log2(n)))
    return 1;
  else
    return 0;
}
int main()
{
  int sockfd,newfd,n,arr[30],count=0,bin;// n-no.of bits
  char buff[MAXLINE],buffer[MAXLINE],data_t[40]; //char buffer1[MAXLINE],token[100];
  int i,j,r,total,nob,rem,dig,pos;
  long data;
  struct sockaddr_in servaddr,cliaddr;
  printf("Enter the data:");
  scanf("%lu",&data);
  n=countbits(data);//no.of bits of data
  r=log2(n);
  r=floor(r);
  while(pow(2,r)<n+r+1)
    r+=1;
  printf("\nNo.of redundant bits:%d\n",r);
  total=n+r;
  //printf("total:%d\n",total);
  nob=floor(log2(total));
  //printf("nob:%d\n",nob);//no.of bits to represent total data


  for(i=1;i<=total;i++)
    {
      dig=data%10;
      if(isapower2(i)==0)
	{
	  arr[total-i]=dig;
	  data/=10;
	}
      else
	arr[total-i]=0;
    }


  for(i=0;i<r;i++)
    {
      //int x=(int)(pow(2,i));
      //printf("i: %d\n",x);
      for(j=1;j<=total;j++)
	{
	  if((int)pow(2,i)!=j)
	    {
	      bin=binary(j);
	      if(ispresent(bin,i+1))
		count+=arr[total-j];
	    }
	}
      // printf("count:%d\n",count);
      if(count%2==0)
	arr[total-(int)pow(2,i)]=0;
      else
	arr[total-(int)pow(2,i)]=1;
      count=0;
    }
  printf("\nData with redundant bits:");
  for(i=0;i<total;i++)
    printf("%d",arr[i]);
  printf("\n\nEnter the position where error has to be made:");
  scanf("%d",&pos);
  if(arr[total-pos]==0)
    arr[total-pos]=1;
  else
    arr[total-pos]=0;
  printf("\n");
  int k=0;
  long num=0;
  for(i=total-1;i>=0;i--)
    {
      //printf("arr[%d]=%d\n",i,arr[i]);
      num+=pow(10,k)*arr[i];
      k++;
    }
  sprintf(data_t, "%lu", num);
  printf("data transmitted is %s\n",data_t);
  // Creating socket file descriptor
  if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
    {
      perror("socket creation failed");
      exit(EXIT_FAILURE);
    }

  bzero(&servaddr,sizeof(servaddr));

  // Filling server information
  servaddr.sin_family = AF_INET; // IPv4
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(8080);

  // Bind the socket with the server address
  if ( bind(sockfd, (const struct sockaddr *)&servaddr,sizeof(servaddr)) < 0 )
    {
      perror("bind failed");
      exit(EXIT_FAILURE);
    }
  int len,m;
  listen(sockfd,2);
  len=sizeof(cliaddr);
  newfd=accept(sockfd,(struct sockaddr*)&cliaddr,&len);
  m=write(newfd,data_t,sizeof(data_t));
}
