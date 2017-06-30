#include<math.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#define MAX 1000
#define PORT 55562
#define SA struct sockaddr
#define N strlen(g)


int sockfd,connfd,len1;
struct sockaddr_in servaddr,cli;

typedef struct frameStruct
{
  char preamble[9];
  char sof[9];
  char dest[49];
  char src[49];
  char len[9];
  char data[81];
  char crc[5];
}Frame;

int c,a,e;
void convert(char str[]);
void toBinary(int n);
void createFrames(char [],int,int);
void crc();
void xor();
char t[1000],bin[1000],bin1[1000],t1[1000];

char fstr[248];
char cs[28],g[]="10110";
char len[1000];
char *lstFrame;
char buff[MAX];
int n;

main()
{
  int i,length,nof1,temp;
  Frame frame[1000];
  float nof;
  strcpy(frame[0].preamble,"10101010");
  strcpy(frame[0].sof,"10101011");
  strcpy(frame[0].dest,"101010101010101010101010101010101010101010101010");
  strcpy(frame[0].src,"101010101010101010101010101010101010101010101010");
  //  printf("%s\n",frame[0].preamble);
  //printf("%s\n",frame[0].sof);
  //printf("%s\n",frame[0].dest);
  //printf("%s\n",frame[0].dest);

  char s[1000];


  printf("\nEnter the data: ");
  scanf( "%[^\n]s" , s );

  convert(s);
  
  toBinary(84);
  strcpy(len,bin);

  length=strlen(t);
  

  nof=ceilf(((float)length / (float)(80)));
  
  nof1=(int)nof;
  printf("\nNo of frames =%d\n",nof1);  
  
  createFrames(t,length,(int)nof);
  
}

int nof;



int binary_decimal(char n[])
{ int decimal=0, i=0, rem;
  int len=strlen(n),c=0,k;

  //printf("%d",len);

  for(i=len-1;i>=0;i--)
    {

      if(n[i]=='0')
        k=0;
      else
        k=1;

      //printf("%d\n",k);
      decimal+=k*pow(2,c);
        c++;

    }
	printf("\n\n%d\n\n",decimal);
  return decimal;
}


void createFrames(char t[],int length,int nof)
{

  Frame f[nof];
  int i,j,k,tempLen,e1;

	
  for(j=0,i=0;i<nof;i++)
    {
      strcpy(f[i].preamble,"10101010");
      strcpy(f[i].sof,"10101011");
      strcpy(f[i].dest,"101010101010101010101010101010101010101010101010");
      strcpy(f[i].src,"101010101010101010101010101010101010101010101010");
      for(k=0;j<length && k < 80;k++)
	{
	  f[i].data[k]=t[j];
	  j++;
	}
      if(j<length)
      	{
	  f[i].data[80]='\0';
	  //toBinary(80);
	  strcpy(f[i].len,len);


	  strcpy(t1,f[i].data);
	  a=strlen(t1);
	  for(e=a;e<a+N-1;e++)
	    t1[e]='0';
	  
	  crc();
	  
//	  printf("\n\ncs=%s\n\n",cs);
	  strcpy(f[i].crc,cs);
//	  printf("\n\ncrc=%s\n\n",f[i].crc);
	}
      else
	{
	  f[i].data[k]='\0';
	  
	  
	  toBinary(k+4);

	  strcpy(len,bin);
	  strcpy(f[i].len,len+8);


	  strcpy(t1,f[i].data);
	  a=strlen(t1);
	  for(e=a;e<a+N-1;e++)
	    t1[e]='0';
	  
	  //printf("\nt1=%s\n",t1);
	  crc();

	  //printf("\n\ncs=%s\n\n",cs);
	  if(strlen(cs)==N)
	    strcpy(f[i].crc,cs+1);
	  else
	    strcpy(f[i].crc,cs);
	  //	  printf("\n\n%s\n\n",f[i].crc)
	}
    }

  for(i=0;i<nof;i++)
    {
      printf("\n\nFrame %d\n\n",i+1);
      printf("\tPreamble:\t\t%s\n",f[i].preamble);
      printf("\tSOF:\t\t\t%s\n",f[i].sof);
      printf("\tDestination MAC:\t%s\n",f[i].dest);
      printf("\tSource MAC:\t\t%s\n",f[i].dest);
      printf("\tLen of Data:\t\t%s\n",f[i].len);
      printf("\tData:\t\t\t%s\n",f[i].data);
      printf("\tCRC:\t\t\t%s\n",f[i].crc);
    }
  

  printf("\n\n");

  srand(time(NULL));
 

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
  servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
  servaddr.sin_port=htons(PORT);
  if((bind(sockfd,(SA*)&servaddr, sizeof(servaddr)))!=0)
    {
      printf("socket bind failed...\n");
      exit(0);
    }
  else
    printf("Socket successfully binded..\n");
  if((listen(sockfd,5))!=0)
    {
      printf("Listen failed...\n");
      exit(0);
    }
  else
    printf("Server listening for client to connect..\n");
  len1=sizeof(cli);
  connfd=accept(sockfd,(SA *)&cli,&len1);
  if(connfd<0)
    {
      printf("server acccept failed...\n");
      exit(0);
    }
  else
    printf("Server acccept the client request...\n");




  
  for(i=0;i<nof;i++)
    {
      lstFrame=(char *)malloc(sizeof(char)*500);
      printf("\n\nSending Frame %d\n",i+1);	
      printf("\nDo you want error in the data? 1(yes) 0(no)? : ");
      scanf("%d",&e1);
      if(e1==1)
	{
		printf("hello");
	  //do{
	  
	  e=rand();
	tempLen=binary_decimal(f[i].len);
		
	
	  //e=e%80;
	e=e%(tempLen);
		printf("tempLen=%d e=%d\n\n\n",tempLen,e);
	
	  printf("\n\nError Position=%d\n\n",e);
	  //  printf("\nEnter the position where error is to be inserted : ");
	  //scanf("%d",&e);
	  //}while(e==0 || e>a+N-1);
	  printf("\nOriginal data:\n\n%s\n",f[i].data);
	
		if(f[i].data[e]=='0')
			f[i].data[e]='1';
		else if(f[i].data[e]=='1')
			f[i].data[e]='0';	    

	 // f[i].data[e]=(f[i].data[e-1]=='0')?'1':'0';
	  printf("\n------------------------------------------------------------------------------------------------------------------\n");
	  printf("\nData with Error:\n\n%s\n",f[i].data);
	  
	  
	}
      
      strcat(lstFrame,f[i].preamble);
      strcat(lstFrame,f[i].sof);
      strcat(lstFrame,f[i].src);
      strcat(lstFrame,f[i].dest);
      strcat(lstFrame,f[i].len);
      strcat(lstFrame,f[i].data);
      strcat(lstFrame,f[i].crc);

      printf("\nActual frame%d to be sent: \n\n%s\n\n",i+1,lstFrame);
      //printf("\nlength=%d\n",strlen(lstFrame));


      int frameCnt=1;
      //printf("To client: ");
      //  for(;;)
      //  {
      
      

      bzero(buff,MAX);
      n=0;
      strcpy(buff,lstFrame);
      //while((buff[n++]=getchar())!='\n');
      //write(sockfd,buff,sizeof(buff));
      
      

      int cnt=write(connfd,buff,sizeof(buff));
      
      printf("\n\nFrame%d is Sent.\n",i+1);
      //printf("\nwritten cnt=%d\n",cnt);
      
      //if(strncmp("exit",buff,4)==0)
      //{
      //printf("Server Exit...\n");
      //break;
      //}
      
      
      //bzero(buff,MAX);
      //read(connfd,buff,sizeof(buff));
      //printf("From client: %s\t To client : ",buff);
      // }
      //printf("\n\n%d\n",cnt);
      //printf("hello");
      
      frameCnt++;
    }
  //strcpy(buff,"exit");
  //write(connfd,buff,sizeof(buff));
  
  
}



void xor(){
  for(c = 1;c < N; c++)
    cs[c] = (( cs[c] == g[c])?'0':'1');
}


void crc(){
  for(e=0;e<N;e++)
    cs[e]=t1[e];
  do{
    if(cs[0]=='1')
      xor();
    for(c=0;c<N-1;c++)
      cs[c]=cs[c+1];
    cs[c]=t1[e++];
  }while(e<=a+N-1);
}

/* void crc(char t[]){ */
/*   for(e=0;e<N;e++) */
/*     cs[e]=t[e]; */
/*   do{ */
/*     if(cs[0]=='1') */
/*       xor(); */
/*     for(c=0;c<N-1;c++) */
/*       cs[c]=cs[c+1]; */
/*     cs[c]=t[e++]; */
/*   }while(e<=a+N-1); */
/*   printf("crc in crc function is = %s\n",cs); */
/* } */

void toBinary(int n)
{
  int i,c,power;
    //c holds the character being converted 
  
  c = n;    //for each binary value below 256 (because ascii values < 256)
  for( power=7 ; power+1 ; power-- )
    //if c is greater than or equal to it, it is a 1
    if( c >= (1<<power) )
      {
	c -= (1<<power); //subtract that binary value
	strcat(bin,"1");
	//printf("1");  
      }
  //otherwise, it is a zero
    else
      {
	strcat(bin,"0");
	//  printf("0");     
      }
}

void convert(char s[])
{
  int i,c,power;
  
  for( i=0 ; s[i]!='\0' ; i++ )
    {
      //c holds the character being converted
      c = s[i];
      
      //for each binary value below 256 (because ascii values < 256)
      for( power=7 ; power+1 ; power-- )
	//if c is greater than or equal to it, it is a 1
	if( c >= (1<<power) )
	  {
	    c -= (1<<power); //subtract that binary value
	    strcat(t,"1");
	    //printf("1");
	  }
      //otherwise, it is a zero
	else
	  {
	    strcat(t,"0");
	    //  printf("0");
	  }
    }
  
}
/* void xor(){ */
/*   for(c = 1;c < N; c++) */
/*     cs[c] = (( cs[c] == g[c])?'0':'1'); */

/* } */
