/*
Propriedade de:
Andre Garrido Damaceno.- mat. 15/0117531
Lucas Campos Jorge - mat. 15/0154135
*/

#ifndef _Primary_libraries
  #define _Primary_libraries
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <ctype.h>
#endif

#ifndef _Socket_libraries
  #define _Socket_libraries
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
		#include <arpa/inet.h>
    #include <unistd.h>
		#include<netdb.h>
#endif

#ifndef _Proxy_library
  #define _Proxy_library
    #include "proxy.h"
#endif

#ifndef _Parser_library
  #define _Parser_library
    #include "parser.h"
#endif

#define REQUEST_SIZE 1000
#define REPLY_SIZE 5000

int main(int argc, char *argv[])
{
  int proxy_port = 0;
  int socket, newsocket, pid;
  socklen_t clilen;
  struct sockaddr_in address,internet_address,cli_addr;
  int n;
  char buffer[1000];
	char request[1000], reply[5000];
	int connection_status;
	char ip[100], hostname[500];

  // Adicionar a leitura do argumento passado pelo terminal
  if(argc == 2)
    proxy_port = atoi(argv[1]);
  else
    proxy_port = 8228;

	// socket da parte servidor
  socket = create_socket();
  config_address(proxy_port,&address);

  int biding_status = proxy_bind(socket, &address);

  if(biding_status == -1)
    printf("ops binding\n");

  listen(socket, 5);
  clilen = sizeof(cli_addr);
  while(1)
  {
    newsocket = proxy_accept(socket);

    if (newsocket < 0)
      printf("Erro ao aceitar");

    pid = fork();

    if (pid < 0)
      printf("Erro no fork");
    if (pid == 0)  {
      close(socket);

    	bzero(request,sizeof(request));
    	n = read(newsocket,request,sizeof(request)-1);
    	if (n < 0) printf("Erro ao ler o socket");
			printf("Here is the message: %s\n",request);

			/************* INTERNET *************/
			int internet_socket = create_socket();

			GetHostFromHeader(request, 1000, hostname, 500);
			get_ip(hostname, ip);

			printf("HostName: %s IP: %s\n", hostname, ip);

			internet_address.sin_family = AF_INET;
			internet_address.sin_port = htons(80);
			inet_aton(ip, (struct in_addr *) &internet_address.sin_addr.s_addr);

			connection_status = proxy_connect(socket, &internet_address);

			if(connection_status == -1)
			{
			  printf("Erro na conexão.\n");
			}

			n = write(internet_socket,request,sizeof(request)-1);
    	if (n < 0) printf("Erro ao escrever no socket internet");

			bzero(reply,sizeof(reply));
    	n = read(internet_socket,reply,sizeof(reply)-1);
    	if (n < 0) printf("Erro ao ler o socket internet");
			printf("Here is the site: %s\n",reply);

			/************************************/

    	n = write(newsocket,reply,sizeof(reply)-1);
    	if (n < 0) printf("Erro ao escrever no socket");

    	exit(0);
    }
    else close(newsocket);
  }
  return 0;
}
