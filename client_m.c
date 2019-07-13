#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <netinet/in.h> 
#include <stdlib.h>
#define PORT 8080 
_Bool verificare=0;
void sent_all(int sock,const char *x,unsigned n,unsigned nr)
{
	if(n==0)
	send(sock, &nr, sizeof(nr), 0);
	if(n>=nr)
		return;
	int i=send(sock,x,nr-n,0);
	if(i<0)
		{perror("Nu s-a putut afisa"); verificare=1; return;}
	else
		sent_all(sock,x+i,n+i,nr);
}
void read_all(int sock,char *x , int n,int dimensiune)
{
	if(n<dimensiune)
	{
		int z=read(sock,x+n,dimensiune);
		if(z<=0)
			{perror("Nu s-a citit tot");verificare=1;return;}
		n+=z;
		read_all(sock,x,n,dimensiune);
	}
}
int main()
{ 
	int sock = 0, valread; 	
	struct sockaddr_in serv_addr; 
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) 
   	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 
	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	if(inet_pton(AF_INET, "192.168.1.8", &serv_addr.sin_addr)<=0) 
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{ 
		printf("\nConnection Failed \n"); 
		return -1; 
	} 
	while(1)
	{		
		if(verificare==1) break;
		char *x;
		x=(char*)malloc(sizeof(char)*100);
		fgets(x,100,stdin);
		sent_all(sock,x,0,strlen(x));
		int dimensiune_raspuns;
		recv(sock, &dimensiune_raspuns, sizeof(dimensiune_raspuns), 0);

		char *sir=(char*)malloc(sizeof(char)*dimensiune_raspuns);
		read_all(sock,sir,0,dimensiune_raspuns);
		if(verificare==1)break;
		
		if(strcmp(sir,"pa")==0)
			{printf("%s\n",sir);break;}
		printf("%s\n",sir);
		free(x);
	}
	close(sock);
	return 0; 
}