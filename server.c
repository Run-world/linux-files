 #include <sys/types.h>          /* See NOTES */
 #include <sys/socket.h>
 #include <string.h>
 #include <stdio.h>
 #include <errno.h>
 #include <stdlib.h>
 #include<netinet/in.h>
 #define PORT 8888
 #define LIS_NUM 5

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

	return sock_fd;
}


void main(int args, char **agrv)
{
	int new_fd;
	struct sockaddr_in theiraddr; //client's message save
	int sock_fd = socket_init();
	listen (sock_fd, LIS_NUM);

	while(1)
	{
		int sin_size = sizeof(struct sockaddr_in);

		new_fd = accept(sock_fd, (struct sockaddr *)&theiraddr, &sin_size);
		if(-1 == new_fd)
		{
		 perror("accept");
		 return;
		}


	printf("connect is ok\nclient IP:%s",(char *)inet_ntoa(theiraddr.sin_addr));
	printf(" port:%u\n", theiraddr.sin_port);
	send(new_fd, "hello world", 12, 0);

	}

}

