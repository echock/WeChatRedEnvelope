#include <stdio.h>
#include <assert.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <sys/select.h>
#define  MAXSIZE 128
int True(char * buf);
int username;
char user[20]={0};
char passwd[20]={0};
char order[MAXSIZE]={0};

int sockfd;
MYSQL  u_mysql;
MYSQL_RES *u_res;
MYSQL_ROW u_row;
int main()
{
	//    int sockfd;
  //  MYSQL  u_mysql;
  //  MYSQL_RES *u_res;
   // MYSQL_ROW u_row;
    const char *u_hostname="localhost";
    const char *u_username="root";
    const char *u_password="123456";
    const char *u_databasename="chattingroom";
    mysql_init(&u_mysql);
    if(!mysql_real_connect(&u_mysql,u_hostname,u_username,u_password,u_databasename,0,NULL,0))
    {
	printf("Can't connect to mysql\n");
    }
    printf("connect to mysql successfully\n");
    struct sockaddr_in server,client;
    server.sin_family=PF_INET;
    server.sin_port=htons(6600);
    inet_aton("127.0.0.1",&server.sin_addr);
    sockfd=socket(PF_INET,SOCK_DGRAM,0);
    assert(sockfd!=-1);
    int num=bind(sockfd,(struct sockaddr*)(&server),sizeof(server));
    assert(num!=-1);
    char buf[MAXSIZE]={0};
    fd_set readfds;
    int ret;
    socklen_t len;
    struct timeval time;
    FD_ZERO(&readfds);
    printf("time to select \n");
    while(1)
    {
	time.tv_sec=3;
 	time.tv_usec=0;
	memset(buf,0,MAXSIZE);
        FD_SET(sockfd,&readfds);
	ret=select(sockfd+1,&readfds,NULL,NULL,&time);
        assert(ret>=0);
	if(FD_ISSET(sockfd,&readfds))
	{
	    len=sizeof(client);
	    recvfrom(sockfd,buf,MAXSIZE,0,(struct sockaddr*)(&client),&len);
	    printf("%s\n",buf);
	    if(True(buf))
	    {
		sendto(sockfd,"ok",strlen("ok"),0,(struct sockaddr*)(&client),sizeof(client));
	    }
	    else	
		sendto(sockfd,"no",strlen("no"),0,(struct sockaddr*)(&client),sizeof(client));
	}
	memset(buf,0,MAXSIZE);
    }
//	if(mysql_query(&u_mysql,order)
//	{
//	    printf("select from database users error\n");
//	}
//	else
//	{
//	    u_res=mysql_store_result(&u_mysql);
//	    if(mysql_num_rows(u_res)!=0)
//		{
//		    while((u_row=mysql_fetch_row(u_res)))
//	            {
//			fprintf(stdout,"\t %s    %s   \n",u_row[0],u_row[1]);
//		    }
//		}
//	    else
//          {
//		printf("no data\n");
//	    }
//	    mysql_free_result(u_res);
//	}
//	mysql_close(&u_mysql);
 //  }
    mysql_close(&u_mysql);
    return 0;
    
}

int True(char *buf)
{
    int i=0;
    int j=0;
    char *p=NULL;
    p=strstr(buf,":");
    while(*(++p)!=';')
    {
	user[i++]=*p;	
    }
    i=0;
    p=p+9;
    while(*(++p)!=';')
    {
	passwd[i++]=*p;
    }
    i=0;
    printf("%s\n",user);
    username=atoi(user);
    printf("%d +  %s\n",username,passwd);
    snprintf(order,MAXSIZE-1,"select * from users where exists (select * from users where username=%d and password='%s')",username,passwd);
    printf("%s\n",order)  ; 
    if(mysql_query(&u_mysql,order))
    {
	 printf("select from database users error\n");
	 return 0;
    }
    else
    {
	   u_res=mysql_store_result(&u_mysql);
           if(mysql_num_rows(u_res)!=0)
	    {
		mysql_free_result(u_res);
//		mysql_close(&u_mysql);
		return 1;
	    }
	    else
	    {
//		mysql_close(&u_mysql);
		return 0;
	    }
    }
}
