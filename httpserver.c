/*************************************************************************
    > File Name: httpserver.c
    > Author: liuguangsheng
    > Mail: ma6174@163.com 
    > Created Time: 2018年09月11日 星期二 18时06分36秒
 ************************************************************************/

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <strings.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/wait.h>
#include <stdlib.h>

void error_die(const char *sc)
{

	    /*出错信息处理 */
	    perror(sc);
	    exit(1);
}

int startup(u_short *port)
{
	int httpfd = 0;
	struct sockaddr_in name;

	/*建立 socket */
	httpfd = socket(PF_INET, SOCK_STREAM, 0);
	if (httpfd == -1)
		error_die("socket");
	memset(&name, 0, sizeof(name));
	name.sin_family = AF_INET;
	name.sin_port = htons(*port);
	name.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(httpfd, (struct sockaddr *)&name, sizeof(name)) < 0)
		error_die("bind");

	/*如果当前指定端口是 0，则动态随机分配一个端口*/
	if (0 == *port)  /* if dynamically allocating a port */
	{

		int namelen = sizeof(name);
		if (getsockname(httpfd, (struct sockaddr *)&name, &namelen) == -1)
			error_die("getsockname");
		*port = ntohs(name.sin_port);
	}

	/*开始监听*/
	if (listen(httpfd, 5) < 0)
		error_die("listen");

	/*返回 socket id */
	return(httpfd);
}


int get_line(int sock, char *buf, int size)
{
	int i = 0;
	char c = '\0';
	int n;

	/*把终止条件统一为 \n 换行符，标准化 buf 数组*/
	while ((i < size - 1) && (c != '\n'))
	{
		/*一次仅接收一个字节*/
		n = recv(sock, &c, 1, 0);
		/* DEBUG printf("%02X\n", c); */
		if (n > 0)
		{
			/*收到 \r 则继续接收下个字节，因为换行符可能是 \r\n */
			if (c == '\r')
			{
				/*使用 MSG_PEEK 标志使下一次读取依然可以得到这次读取的内容，可认为接收窗口不滑动*/
				n = recv(sock, &c, 1, MSG_PEEK);
				/* DEBUG printf("%02X\n", c); */
				/*但如果是换行符则把它吸收掉*/
				if ((n > 0) && (c == '\n'))
					recv(sock, &c, 1, 0);
				else
					c = '\n';
			}
			/*存到缓冲区*/
			buf[i] = c;
			i++;
		}
		else
			c = '\n';
	}
	buf[i] = '\0';

	/*返回 buf 数组大小*/
	return(i);
}

void accept_request(void* arg)
{

	char buf[1024];
	int numchars;
	char method[255];
	char url[255];
	char path[512];
	size_t i, j;
	struct stat st;
	int cgi = 0;      /* becomes true if server decides this is a CGI program */
	char *query_string = NULL;

	int client = *((int *)arg);
	/*得到请求的第一行*/
	numchars = get_line(client, buf, sizeof(buf));
	i = 0; j = 0;	
	printf("%s\n",buf);
#if 0
	/*把客户端的请求方法存到 method 数组*/
	while (!ISspace(buf[j]) && (i < sizeof(method) - 1))
	{

		method[i] = buf[j];
		i++; j++;
	}
	method[i] = '\0';

	/*如果既不是 GET 又不是 POST 则无法处理 */
	if (strcasecmp(method, "GET") && strcasecmp(method, "POST"))
	{

		unimplemented(client);
		return;
	}

	/* POST 的时候开启 cgi */
	if (strcasecmp(method, "POST") == 0)
		cgi = 1;

	/*读取 url 地址*/
	i = 0;
	while (ISspace(buf[j]) && (j < sizeof(buf)))
		j++;
	while (!ISspace(buf[j]) && (i < sizeof(url) - 1) && (j < sizeof(buf)))
	{

		/*存下 url */
		url[i] = buf[j];
		i++; j++;
	}
	url[i] = '\0';

	/*处理 GET 方法*/
	if (strcasecmp(method, "GET") == 0)
	{

		/* 待处理请求为 url */
		query_string = url;
		while ((*query_string != '?') && (*query_string != '\0'))
			query_string++;
		/* GET 方法特点，? 后面为参数*/
		if (*query_string == '?')
		{

			/*开启 cgi */
			cgi = 1;
			*query_string = '\0';
			query_string++;
		}
	}

	/*格式化 url 到 path 数组，html 文件都在 htdocs 中*/
	sprintf(path, "htdocs%s", url);
	/*默认情况为 index.html */
	if (path[strlen(path) - 1] == '/')
		strcat(path, "index.html");
	/*根据路径找到对应文件 */
	if (stat(path, &st) == -1) {

		/*把所有 headers 的信息都丢弃*/
		while ((numchars > 0) && strcmp("\n", buf))  /* read & discard headers */
			numchars = get_line(client, buf, sizeof(buf));
		/*回应客户端找不到*/
		not_found(client);
	}
	else
	{

		/*如果是个目录，则默认使用该目录下 index.html 文件*/
		if ((st.st_mode & S_IFMT) == S_IFDIR)
			strcat(path, "/index.html");
		if ((st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) || (st.st_mode & S_IXOTH)    )
			cgi = 1;
		/*不是 cgi,直接把服务器文件返回，否则执行 cgi */
		if (!cgi)
			serve_file(client, path);
		else
			execute_cgi(client, path, method, query_string);
	}
#endif

	/*断开与客户端的连接（HTTP 特点：无连接）*/
	close(client);
}

int main(void)
{
	int server_sock = -1;
	u_short port = 0;
	int client_sock = -1;
	struct sockaddr_in client_name;
	int client_name_len = sizeof(client_name);
	pthread_t newthread;

	/*在对应端口建立httpd服务*/
	server_sock = startup(&port);
	printf("httpd running on port %d\n", port);
	
	while (1)
	{

		/*套接字收到客户端连接请求*/
		client_sock = accept(server_sock,(struct sockaddr *)&client_name,&client_name_len);
		if (client_sock == -1)
			error_die("accept");
		printf("aaaaaaaa\n");
		/*派生新线程用 accept_request 函数处理新请求*/
		/* accept_request(client_sock); */

		if (pthread_create(&newthread , NULL, (void *)accept_request,(void *)&client_sock) != 0)
			perror("pthread_create");

		pthread_join(newthread, NULL);
		pthread_detach(newthread);
	}

	close(server_sock);

	return 0;
}
