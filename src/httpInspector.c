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
#endif

#ifndef _Proxy_library
  #define _Proxy_library
    #include "proxy.h"
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

  // Adicionar a leitura do argumento passado pelo terminal
  if(argc == 2)
  {
    proxy_port = atoi(argv[1]);
  }
  else
  {
    proxy_port = 8228;
  }
  //
  // /***************** CLIENT **********************/
  // // cria o socket
  // socket = create_socket();
  //
  // // endereço do servidor
  // server_adress = config_address(port);
  //
  // connection_status = proxy_connect(socket, &server_adress);
  //
  // if(connection_status == -1)
  // {
  //   printf("Erro na conexão.\n", );
  // }
  //
  // // recebe dado do servidor
  // char message[256];
  // proxy_receive(socket, message);
  //
  // // imprime resposta
  // printf("Resposta: $s\n", message);
  // // fecha o proxy_socket
  // close(socket);

  /*************************SERVIDOR********************/

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
    newsocket = accept(socket,(struct sockaddr *) &cli_addr, &clilen);

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
			config_address(80, &internet_address);

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
