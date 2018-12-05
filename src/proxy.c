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
    #include <netdb.h>
		#include <arpa/inet.h>
    #include <unistd.h>
#endif

#ifndef _Parser_library
  #define _Parser_library
    #include "parser.h"
#endif

int create_socket()
{
  return socket(AF_INET,SOCK_STREAM,0);
}

void config_address(int port, struct sockaddr_in * address)
{
  bzero((char *) address, sizeof(*address));
  address->sin_family = AF_INET;
  address->sin_port = htons(port);
  address->sin_addr.s_addr = INADDR_ANY;
}

int get_ip(char hostname[], char *ip){
	struct hostent *he;
	struct in_addr **addr_list;
	int i;

	if ( (he = gethostbyname( hostname ) ) == NULL)
	{
		return -1;
	}

	addr_list = (struct in_addr **) he->h_addr_list;

	for(i = 0; addr_list[i] != NULL; i++)
	{
		strcpy(ip , inet_ntoa(*addr_list[i]) );
	}

	return 0;
}

int proxy_connect(int socket,struct sockaddr_in * address){
  return connect(socket, (struct sockaddr *) address, sizeof(*address));
}

// recebe dado do servidor
int proxy_receive(int socket, char * message){
  return read(socket, message, sizeof(message));
}

int proxy_bind(int socket, struct sockaddr_in *address){
  return bind(socket, (struct sockaddr *) address, sizeof(*address));
}

int proxy_accept(int socket){
  return accept(socket, NULL, NULL);
}

int proxy_send(int socket, char * message){
  return send(socket, message, sizeof(message), 0);
}

void error(char *msg)
{
  perror(msg);
  exit(1);
}

/*
Função:
	GetServerResponse: A partir de um inform busca e armazena a resposta do servidor em tmp/server_response.txt
Argumentos:
	hostname: Ponteiro para string com um hostname
	url: Ponteiro para string com uma url
*/

void get_server_response(char *hostname, char *url)
{
  int sock,port = 80, n, msgSize, firstPack = 1;
	char http_request[300];
	char path[500];
	char name[500];
	char buffer[5000], msgLenth[100], *tmpBuffer;
	FILE* file;

  system("mkdir -p ./tmp");
	file = fopen("./tmp/server_response.txt","w");
	if(file == NULL)
		error("Erro ao abrir o arquivo");

	struct sockaddr_in server_address;
	struct hostent *server;

	// printf("url: %s\n", url);
	// printf("hostname: %s\n", hostname);

	ClearString(http_request,300);
	GetHttpFolderPath(url, path, 500);
	GetHttpFileName(url, name, 500);
	strcat(path, name);

	strcat(http_request,"GET ");
	strcat(http_request,path);
	strcat(http_request," HTTP/1.0\r\nHost: ");
	strcat(http_request,hostname);
	strcat(http_request,"\r\n\r\n");

	// printf("Request:\n %s\n", http_request);

	// Criando socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
  if (socket < 0)
  	error("Erro ao criar socket");

	// Informações do servidor
  if ((server = gethostbyname(hostname)) == NULL)
  	error("Host inexistente");

	//configuração do endereço do servidor
  bzero((char *) &server_address, sizeof(server_address));
  server_address.sin_family = AF_INET;
  bcopy((char *)server->h_addr,
       (char *)&server_address.sin_addr.s_addr,
       server->h_length);
  server_address.sin_port = htons(port);


  if (connect(sock,(struct sockaddr *)&server_address,sizeof(server_address)) < 0)
    error("Erro na conexão");

  if (write(sock,http_request,strlen(http_request)) < 0)
    error("Erro ao escrever no socket");

	while(1){
		bzero(buffer,sizeof(buffer));
  	n = read(sock,buffer,sizeof(buffer) - 1);

    if (firstPack == 1)
    {
      GetFromText("Content-Length:", 1, '\r', buffer, sizeof(buffer), msgLenth, sizeof(msgLenth));

      if (msgLenth != '\0')
        msgSize = atoi(msgLenth);

      tmpBuffer = strstr(buffer, "\r\n\r\n");

      if (tmpBuffer != NULL)
      {
        tmpBuffer += 4;
        n = n - (int)(tmpBuffer - buffer);
      }
      firstPack = 0;
    }

		fprintf(stderr, ".");
  	if(n < 0)
    	error("Erro ao ler no socket");
		if(n > 0)
    {
	  	// printf("%s",buffer);
			fprintf(file, "%s", buffer);
      msgSize = msgSize - n;

      if (msgSize == 0)
        break;
		}
		if(n == 0) break;
	}
	shutdown(sock, 2);
	close(sock);
	fclose(file);
}

/*
Função:
	send_request: Envia a requisição HTTP armazenada em files/request.txt para o
servidor de destino, obtem a reposta e armazena em files/reply.txt
*/

int send_request(){
	int ok, n, sock, numbytes;
	FILE *frequest, *freply;
	char *request, buffer[5000], hostname[500], *tmpBuffer;
	int port = 80, msgSize, firstPack = 1;
  char msgLenth[100];
	struct sockaddr_in server_address;
	struct hostent *server;

  // Criando o diretório
  system("mkdir -p ./files/proxy");

	// criando arquivos
	if((frequest = fopen("./files/proxy/request.txt", "r")) == NULL)
		error("Erro ao criar arquivos files/proxy/request.txt");

	if((freply = fopen("./files/proxy/reply.txt", "wb")) == NULL)
		error("Erro ao criar arquivos files/proxy/reply.txt");

	// captura o tamanho do arquivo em bytes
	fseek(frequest, 0L, SEEK_END);
	numbytes = ftell(frequest);
	// reposiciona no início do arquivo
	fseek(frequest, 0L, SEEK_SET);

	// aloca memória
	request = (char*)calloc(numbytes, sizeof(char));
	if(request == NULL) error("Erro ao alocar memória");

	// copia texto para o buffer
	fread(request, sizeof(char), numbytes, frequest);
	fclose(frequest);

	GetHostFromHeader(request, strlen(request), hostname, sizeof(hostname)-1);

	// Criando socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
  	error("Erro ao criar socket");

	// Informações do servidor
  if ((server = gethostbyname(hostname)) == NULL)
  	error("Host inexistente");

	//configuração do endereço do servidor
	bzero((char *) &server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
			 (char *)&server_address.sin_addr.s_addr,
			 server->h_length);
	server_address.sin_port = htons(port);

	if (connect(sock,(struct sockaddr *)&server_address,sizeof(server_address)) < 0)
		error("Erro na conexão");

	// Envia a requisição no socket
	if (write(sock,request,strlen(request)) < 0)
		error("Erro ao escrever no socket");

  ClearString(msgLenth, sizeof(msgLenth));

	// Armazena toda a resposta do servidor em files/reply.txt
	while(1)
  {
		bzero(buffer,sizeof(buffer));
		n = read(sock,buffer,sizeof(buffer) - 1);

    if (firstPack == 1)
    {
      GetFromText("Content-Length:", 1, '\r', buffer, sizeof(buffer), msgLenth, sizeof(msgLenth));

      if (msgLenth != '\0')
        msgSize = atoi(msgLenth);

      tmpBuffer = strstr(buffer, "\r\n\r\n");

      if (tmpBuffer != NULL)
      {
        tmpBuffer += 4;
        n = n - (int)(tmpBuffer - buffer);
      }
      firstPack = 0;
    }

		if(n < 0)
			error("Erro ao ler no socket");
		if(n > 0){
			printf("%s",buffer);
			fprintf(freply, "%s", buffer);
      msgSize = msgSize - n;

      if (msgSize == 0)
        break;
		}
		if(n == 0)
      break;
	}

  close(sock);
	fclose(freply);
	free(request);
	return ok;
}
