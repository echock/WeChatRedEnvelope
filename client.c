#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <sys/select.h>
#include <unistd.h>
#define MAXSIZE 1024
void showtable();
void showtable2();
void showtable3();
void EnterChattingRoom();
void Enter();
void Register();
void friendlist();
void ChattingRoom();
void add();
void tulunbei();
int sockfd1,sockfd2;
struct sockaddr_in server,server2;
int main()
{
    int n;
    showtable();
    bzero(&server,sizeof(server));
    inet_aton("127.0.0.1",&server.sin_addr);
    sockfd1=socket(PF_INET,SOCK_DGRAM,0);
    sockfd2=socket(PF_INET,SOCK_STREAM,0);
    scanf("%d",&n);
    while(1)
    {	
//	showtable();
	switch(n)
	{
	    case 1:Enter();
		break;
	    case 2:Register();
	    	break;
	    case 3:exit(0);
	    default :break;
	}
	showtable();
	scanf("%d",&n);
    }
}

void showtable(void )
{
    system("clear");
    printf("\n\n\n\n\n\n\n");
    printf("\t\t\t Welcome To Chatting ROOM\n");
    printf("\t*****************************************************\n");
    printf("\t*                                                   *\n");
    printf("\t\t 1- Enter\n");
    printf("\t*                                                   *\n");
    printf("\t\t 2-Register\n");
    printf("\t*                                                   *\n");
    printf("\t\t 3-exit\n");
    printf("\t*                                                   *\n");
    printf("\t*                                                   *\n");
    printf("\t*****************************************************\n");

}
void Enter()
{
    
    int username;
    char password[20];
    char buf[MAXSIZE]={0};
    system("clear");
    printf("please input your username\n");
    scanf("%d",&username);
    printf("please input your password\n");
    scanf("%s",password);
    snprintf(buf,MAXSIZE,"username:%d;password:%s;\n",username,password);
    //fgets(buf,MAXSIZE,stdin);
    server.sin_port=htons(6600);
    server.sin_family=AF_INET;
    sendto(sockfd1,buf,strlen(buf),0,(struct sockaddr *)(&server),sizeof(server));
    memset(buf,0,MAXSIZE);
    socklen_t len=sizeof(server2);
    recvfrom(sockfd1,buf,MAXSIZE,0,(struct sockaddr *)(&server2),&len);
    printf("%s\n",buf);    
    if(!strncmp(buf,"ok",2))
    {
	EnterChattingRoom();
    }
    else
    {
	printf("username or password error,now return last step\n");
   	sleep(1);
	return ;
    }
        
}
void Register()
{

}
void EnterChattingRoom()
{
    showtable2();
    int n;
    scanf("%d",&n);
    while(1)
    {
	switch(n)
	{
	    case 1:friendlist();
		break;
	    case 2:ChattingRoom();
	    	break;
	    case 3:return;
	    default :break;
	}
	scanf("%d",&n);
    }
   
}
void friendlist(){}
void ChattingRoom()
{
    
    showtable3();
    int n;
    scanf("%d",&n);
    while(1)
    {
	switch(n)
	{
	    case 1:tulunbei();
		break;
	    case 2:add();
	    	break;
	    case 3:return;
	    default :break;
	}
	scanf("%d",&n);
    }
}
void add(){}
void tulunbei()
{   
    int ret;
    char buff[MAXSIZE]={0};
    server.sin_family=AF_INET;
    server.sin_port=htons(6601);
    ret=connect(sockfd2,(struct sockaddr*)(&server),sizeof(server));
    if(ret==-1)
    {
	printf("connect error\n");
    }
    printf("connect success\n");
    fd_set readfds;
    FD_ZERO(&readfds);
    struct timeval time;
    while(1)
    {
	FD_SET(0,&readfds);
	FD_SET(sockfd2,&readfds);
	time.tv_sec=2;
 	time.tv_usec=0;
	ret=select(sockfd2+1,&readfds,NULL,NULL,&time);
	if(ret<0)
	{
	    printf("select error\n");
	    break;
	}
//	printf("%d\n",ret);
       // printf("nothing happened\n");
	if(FD_ISSET(0,&readfds))
	{
	    if(fgets(buff,MAXSIZE,stdin)==NULL)
		{
		    break;
		}
	    send(sockfd2,buff,strlen(buff),0);
	    printf("had sent\n");
	}
	if(FD_ISSET(sockfd2,&readfds))
	{
	    memset(buff,0,MAXSIZE);
	    if(recv(sockfd2,buff,MAXSIZE,0)==0)
	    {
		break;
 	    }
	    printf("%s\n",buff);
	}
    }
    close(sockfd2);
}
void showtable3()
{
    system("clear");
    printf("\n\n\n\n\n\n\n");
    printf("\t\t\t Welcome To Chatting ROOM  LIST\n");
    printf("\t*****************************************************\n");
    printf("\t*                                                   *\n");
    printf("\t\t 1-tulunbei\n");
    printf("\t*                                                   *\n"); 
    printf("\t\t 2-add\n");
    printf("\t*                                                   *\n"); 
    printf("\t\t 3-exit\n");
    printf("\t*                                                   *\n");
    printf("\t*                                                   *\n");
    printf("\t*****************************************************\n");
}
void showtable2()
{
    system("clear");
    printf("\n\n\n\n\n\n\n");
    printf("\t\t\t Welcome To Chatting ROOM\n");
    printf("\t*****************************************************\n");
    printf("\t*                                                   *\n");
    printf("\t\t 1-friendlist\n");
    printf("\t*                                                   *\n");
    printf("\t\t 2-ChattingRoom\n");
    printf("\t*                                                   *\n");
    printf("\t\t 3-exit\n");
    printf("\t*                                                   *\n");
    printf("\t*                                                   *\n");
    printf("\t*****************************************************\n");
}
ConnectServer()
{
     
}
