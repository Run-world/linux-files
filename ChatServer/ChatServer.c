/*************************************************************************
    > File Name: ChatServer.c
    > Author: liuguangsheng
    > Mail: ma6174@163.com 
    > Created Time: 2018年11月26日 星期一 21时12分20秒
 ************************************************************************/

#include<stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <pthread.h>
#include <sqlite3.h>

#define DEBUG
//sqlite
#define lenth 128

//socket  
#define PORT 8888
#define LIS_NUM 10
#define CMD_LEN 128
#define MSG_LEN 1024

char db_name[lenth] = "./User.db";
sqlite3 *mysql = NULL;

int sqlite3_init(){
	char *pErrMsg = NULL;
	/* 打开数据库 */
	int len = sqlite3_open(db_name,&mysql);
	if( len )
	{
		/*  fprintf函数格式化输出错误信息到指定的stderr文件流中  */
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(mysql));//sqlite3_errmsg(db)用以获得数据库打开错误码的英文描述。
		sqlite3_close(mysql);
		exit(1);
	}
	else
		printf("You have opened a sqlite3 database named user successfully!\n");

	/*创建表格*/

	/*插入数据*/
	int result = sqlite3_exec(mysql, "insert into USER_ID values(1,'12345678','lgs1111');", 0, 0, &pErrMsg);
	if(result == SQLITE_OK){

		printf("插入数据成功\n");
	}
	return  EXIT_SUCCESS;

}

/*int sqlite_deal(){
	
}*/

int socket_init()
{
	int sock_fd;
	struct sockaddr_in myaddr; //my message save

	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == sock_fd)
	{

		perror("socket");
		return EXIT_FAILURE;
	}
	memset(&myaddr, 0, sizeof(struct sockaddr_in));

	myaddr.sin_family=AF_INET;/*该属性表示接收本机或其他机器传输*/
	myaddr.sin_port=htons(PORT);/*端口号*/
	myaddr.sin_addr.s_addr=htonl(INADDR_ANY);/*IP，括号内容表示本机IP*/

	if((bind(sock_fd, (struct sockaddr *)&myaddr, sizeof(myaddr)))<0)
	{

		perror("bind");
		return EXIT_FAILURE;
	}
	printf("bind is ok!\n");
	/*异步*/
	listen (sock_fd, LIS_NUM);
	return sock_fd;
}

void *deal_client(void *fd)
{
	int cmd = 0;
	char buf_cmd[CMD_LEN];
	int fd_c = *((int *)fd);
	int ret  = recv(fd_c, buf_cmd, 128, 0);
	if(-1 == ret)
	{
		perror("recv");
	}
   
	if(strstr(buf_cmd, "register"))	{
		printf("register  is ok!");
	}
	else if(strstr(buf_cmd, "login")){
		printf("login  is ok!");
	}


	printf("----%s", buf_cmd);
	
}

int main(int args, char **argv)
{
	struct sockaddr_in theiraddr; //client's message save
	int sin_size = sizeof(struct sockaddr_in);
	int new_fd;
	pthread_t id;

	int sock_fd = socket_init();

/*	while(1)
	{

		new_fd = accept(sock_fd, (struct sockaddr *)&theiraddr, &sin_size);
		if(-1 == new_fd)
		{

			perror("accept");
			return;
		}

		printf("connect is ok\nclient IP:%s",(char *)inet_ntoa(theiraddr.sin_addr));
		printf(" port:%u\n", theiraddr.sin_port);

		int ret = pthread_create(&id, NULL, deal_client,(void *)&new_fd);
		if(0 != ret)
		{

			printf("thread create is fail\n");
			exit(0);
		}

		pthread_join(id, NULL);//wait thread is over    recover resource
	}*/
	sqlite3_init();

		return 0;
}
