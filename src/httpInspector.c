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

#ifndef _Spider_library
  #define _Spider_library
    #include "spider.h"
#endif

#define REQUEST_SIZE 1000
#define REPLY_SIZE 5000

void topMenu(){
	printf("-------------------------------------------------------\n");
	printf("                   HTTTP INSPECTOR                     \n");
	printf("-------------------------------------------------------\n");
}

char menu(){
	char c;

	printf("Digite a funcionalidade desejada: \n");
	printf("1) Enviar request\n");
	printf("2) Imprimir Spider\n");
	printf("3) Realizar Dump \n");

	scanf("%c", &c);
	getchar();

	return c;
}

int main(int argc, char *argv[])
{
  int proxy_port = 0;
  int sock, newsock, pid;
	struct hostent *server;
  int n, numbytes;
  char buffer[1000];
	char request[1000];
	int connection_status;
	char ip[100], hostname[500], *reply;
	char request_test[] = "GET / HTTP/1.1\r\nHost: flaviomoura.mat.br/\r\n\r\n";
	int biding_status;
	char choice;
	socklen_t clilen;
	struct sockaddr_in proxy_address, client_address;
	FILE *frequest, *freply, *fcache;
	char yn;
	int cached = 0;

  // Adicionar a leitura do argumento passado pelo terminal
  if(argc == 2)
    proxy_port = atoi(argv[1]);
  else
    proxy_port = 8228;

	topMenu(); //imprime nome do programa

	// criando socket para receber requisição do browser
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		error("ERROR opening socket");

	bzero((char *) &proxy_address, sizeof(proxy_address));

	proxy_address.sin_family = AF_INET;
	proxy_address.sin_addr.s_addr = INADDR_ANY;
	proxy_address.sin_port = htons(proxy_port);

	if (bind(sock, (struct sockaddr *) &proxy_address, sizeof(proxy_address)) < 0)
		error("ERROR on binding");

	listen(sock,5);
	clilen = sizeof(client_address);

	printf("Aguardando browser...\n");

  while(1)
  {
		// libera o socket utiliza outro para lidar com o pedido
		newsock = accept(sock,(struct sockaddr *) &client_address, &clilen);
    if (newsock < 0) error("Erro ao aceitar");
		else close(sock);

		bzero(request,sizeof(request));

    n = read(newsock, request, sizeof(request)-1);
    if(n < 0) error("Erro ao ler o socket");
		strcat(request,buffer);

		if((frequest = fopen("files/request.txt", "w")) == NULL)
			error("Erro ao criar arquivos files/request.txt");
		printf("Request do browser:\n\n");
		printf("%s",request);
		fprintf(frequest, "%s", request);
		fclose(frequest);

		if(cached){
			printf("Deseja retorna o arquivo da cache para o browser?\n");
			scanf("%c", &yn);
			getchar();
			yn = tolower(yn);
			if(yn == 'y' || yn == 's'){
				// retorna o arquivo
				printf("retorna arquivo\n");
			}
		}

		printf("Deseja editar o pedido antes de enviar para o servidor? (y/n)\n");

		scanf("%c", &yn);
		getchar();
		yn = tolower(yn);
		if(yn == 'y' || yn == 's'){
			system("nano files/request.txt");
		}

		choice = menu();

		switch (choice) {
			case '1':
				send_request();

				printf("Deseja editar a resposta antes de enviar para o browser? (y/n)\n");

				scanf("%c", &yn);
				getchar();
				yn = tolower(yn);
				if(yn == 'y' || yn == 's'){
					system("nano files/reply.txt");
				}

				if((freply = fopen("files/reply.txt", "w")) == NULL)
					error("Erro ao criar arquivos files/reply.txt");

				while(fread(&buffer, 1, sizeof(buffer), freply) == sizeof(buffer) ){
					send(newsock, buffer, sizeof(buffer), 0);
				}
				fclose(freply);

				break;
			case '2':
				printf("spider()\n");
				break;
			case '3':
				printf("dump()");
				break;
			default:
				printf("Opção inválida\n");
		}
		close(newsock);
	}

	close(sock);

  return 0;
}
