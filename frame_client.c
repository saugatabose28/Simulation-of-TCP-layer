#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdio.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<stdlib.h>
#define MAX 1000
#define PORT 55562
#define SA struct sockaddr
#define N strlen(g)

int sockfd,connfd;
struct sockaddr_in servaddr,cli;




char g[]="10110",cs[28],data[1000];
int a,e,c,length;

void xor();
void crc();
char buff[MAX];
int n;

main()
{
  int i,j;
  
  char frame[1000],*temp,len[8],crc1[N];
  
  
  
  
  sockfd=socket(AF_INET,SOCK_STREAM,0);
  if(sockfd==-1)
    {
      printf("socket creation failed...\n");
      exit(0);
    }
  else
    printf("Socket successfully created..\n");
  bzero(&servaddr,sizeof(servaddr));
  servaddr.sin_family=AF_INET;
  servaddr.sin_addr.s_addr=inet_addr("127.0.0.1");
  servaddr.sin_port=htons(PORT);
  if(connect(sockfd,(SA *)&servaddr,sizeof(servaddr))!=0)
    {
      printf("connection with the server failed...\n");
      exit(0);
    }
  else
    printf("connected to the server..\n");
  
  
  
  
  
  
  
  
  //  for(;;)
  //  {
  int cnt=1;
  int readCnt;
  
  while(1)
    {
      
      printf("\n\nFrame %d\n",cnt);
      
      
      bzero(cs,sizeof(cs));
      
      bzero(frame,sizeof(frame));
      //      bzero(buff,sizeof(buff));
      //      read(sockfd,buff,sizeof(buff));
      readCnt=read(sockfd,frame,sizeof(frame));
      
      //printf("\nReadcnt=%d",readCnt);
      //      printf("From Server : %s",frame);
      //if((strncmp(buff,"exit",4))==0)
      //	{
      //	  printf("Client Exit...\n");
      //	  break;
      //	}
      
      if(readCnt==0)
	break;
      //bzero(buff,sizeof(buff));
      //printf("Enter the string : ");
      //n=0;
      //while((buff[n++]=getchar())!='\n');
      //write(sockfd,buff,sizeof(buff));
      //}
      
      
      
      
      
      
      //	strcpy(frame,buff);
      
      //scanf("%s",frame);
      
      printf("\nReceived Frame:\n\n%s\n",frame);
      
      length=strlen(frame);
      
      printf("\n\nLength of Received Frame: %d",length);
      j=0;
      for(i=120;i<length-4;i++)
	{
	  data[j]=frame[i];
	  j++;
	}
      data[j]='\0';
      
      j=0;
      
      for(i=length-4;i<length;i++)
	{
	  crc1[j]=frame[i];
	  j++;
	}
      crc1[j]='\0';
      printf("\n\ndata: %s",data);
      printf("\n\ncrc: %s",crc1);
      
      strcat(data,crc1);
      //printf("\n\ndata=%s",data);
      length=strlen(data);
      
      crc();
      
      for(e=0;(e<N-1) && (cs[e]!='1');e++);
      if(e<N-1)
	printf("\nError detected\n\n");
      else
	printf("\nNo error detected\n\n");
      
      cnt=cnt+1;
      
    }
  
}

void xor(){
  for(c = 1;c < N; c++)
    cs[c] = (( cs[c] == g[c])?'0':'1');
}

void crc()
{
  for(e=0;e<N;e++)
    cs[e]=data[e];
  do{
    if(cs[0]=='1')
      xor();
    for(c=0;c<N-1;c++)
      cs[c]=cs[c+1];
    cs[c]=data[e++];
  }while(e<=length);
}
