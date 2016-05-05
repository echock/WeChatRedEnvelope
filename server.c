#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <mysql/mysql.h>
#include <sys/select.h>
#include <assert.h>
#define MAXUSER 101
#define MAXSIZE 1024
char buf[MAXSIZE]={0};
void broadcast(int i,int *sock);
int main()
{
    int sock[MAXUSER]={0};
    int max=0;
    int ret=0;
    int i=0;
    int tmp=0;
    int j=0;
    struct sockaddr_in server,client;
    bzero(&server,sizeof(server));
    bzero(&client,sizeof(client));
    server.sin_family=PF_INET;
    server.sin_port=htons(6500);
    inet_aton("127.0.0.1",&server.sin_addr);
    sock[0]=socket(AF_INET,SOCK_STREAM,0);
    assert(sock[0]!=-1);
    ret=bind(sock[0],(struct sockaddr*)(&server),sizeof(server));
    assert(ret!=-1);
    ret=listen(sock[0],100);
    assert(ret!=-1);
//    char buf[1024]={0};
    fd_set readfds;
    FD_ZERO(&readfds);
    struct timeval time;
    max=sock[0];
    printf("ready,waitting\n");
    while(1)
    {
	
	memset(buf,0,MAXSIZE);
   	for(i=0;i<MAXUSER;++i)
	{
	    if(sock[i]>0)
		{
		    FD_SET(sock[i],&readfds);
		    if(sock[i]>max)
		    {
    			max=sock[i];
   		    }
		}
	}
	time.tv_sec=5;
	time.tv_usec=0;
    	ret=select(max+1,&readfds,NULL,NULL,&time);
        if(ret<0)
	{
	    printf("select error\n");
	    exit(0);
	}
	printf("%d\n",ret);
	for(i=0;i<MAXUSER;++i)
	{
	    if(sock[i]>=0)
	    {
		if(FD_ISSET(sock[i],&readfds))
                {
		    if(i==0)
		    {
			tmp=accept(sock[0],NULL,NULL);
			printf("%d is online\n",tmp);
			for(j=1;j<MAXUSER;++j)
			{
			    if(sock[j]<=0)
			    {
				sock[j]=tmp;
				break;
 			    }
			}		
   		    }
		    else if(i!=0)
		    {
			ret=recv(sock[i],buf,MAXSIZE-1,0);
			printf("had recv\n");
			if(ret==0)
			{
			    close(sock[i]);
			    sock[i]=-1;
			}
			else
			{
			    broadcast(i,sock);
			    memset(buf,0,MAXSIZE);
			}
		    }
                }
 	    }
	}
    }
    colse(sock[0]);
}

void broadcast(int i,int *sock)
{
	int j=1;
	printf("%d\n",i);
	for(j=1;j<MAXUSER;++j)
	{
	    if(sock[j]>0&&sock[j]!=i)
	    {
		send(sock[j],buf,strlen(buf),0);
	    }
	}
}
