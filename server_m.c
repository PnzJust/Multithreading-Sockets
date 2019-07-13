#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <pthread.h>
#define PORT 8080 
typedef struct{
	int socket;
}SOCKET;

int sent_all(int sock,const char *x,unsigned n,unsigned nr)
{
	if(n==0)
	send(sock, &nr, sizeof(nr), 0);
	if(n>=nr)
		return 0;
	int i=send(sock,x,nr-n,0);
	if(i<0)
		{printf("Nu s-a putut afisa tot\n");  return -1;}
	else
		return sent_all(sock,x+i,n+i,nr);
	return 1;
}

int  read_all(int sock,char *x , int n,int dimensiune)
{
	if(n<dimensiune)
	{
		int z=read(sock,x+n,dimensiune);
		if(z<=0)
			{printf("Nu s-a citit tot\n");return-1;}
		n+=z;
		return read_all(sock,x,n,dimensiune);
	}
	return 1;
}
void *f(void *vargp)
{
		SOCKET *a=(SOCKET *)vargp;
	while (1)
	{
		int dimensiune;
		recv(a->socket, &dimensiune, sizeof(dimensiune), 0);
		
		char *x=(char*)malloc(sizeof(char)*dimensiune);
		
		if(read_all(a->socket,x,0,dimensiune)==-1) break;
		
		printf("%s\n",x);
		if(x[0]=='a')
				{ int s=sent_all(a->socket,"pa",0,strlen("pa\n"));break;}
			else
			 	{int s=sent_all(a->socket,"ok",0,strlen("ok\n"));}
		free(x);

	}
	free(a);

}
int main()
{ 


	int server_fd,new_socket; 
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address); 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed\n"); 
		exit(EXIT_FAILURE); 
	} 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt))) 
	{ 
		perror("setsockopt\n"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
	if (bind(server_fd, (struct sockaddr *)&address,sizeof(address))<0) 
	{ 
		perror("bind failed\n"); 
		exit(EXIT_FAILURE); 
	} 
	if (listen(server_fd, 3) < 0) 
	{ 
		perror("listen\n"); 
		exit(EXIT_FAILURE); 
	} 

		while(1)
		{

				SOCKET *x=(SOCKET*)malloc(sizeof(SOCKET));

				if ((x->socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen))<0) 
						{ 
							perror("accept\n"); 
							exit(EXIT_FAILURE); 
						} 
				else
 				{		
 				pthread_t thread_id; 
 		 		pthread_create(&thread_id, NULL, f, x); 
 				//pthread_join(thread_id, NULL); 
 				}
		}

	//close(new_socket);
	close(server_fd);

	return 0; 
} 
