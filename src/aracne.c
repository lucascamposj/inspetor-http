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
		#include <netinet/tcp.h>
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
	printf("                   HTTP INSPECTOR                      \n");
	printf("-------------------------------------------------------\n");
}

char menu(){
	char c;

  // Limpa a tela
  system("clear");

	printf("\n------------------------ MENU -------------------------\n");
	printf("1) Enviar request\n");
	printf("2) Imprimir Spider\n");
	printf("3) Realizar Dump \n");
  printf("4) Drop package \n");
	printf("Digite a funcionalidade desejada: ");

	scanf("%c", &c);
	getchar();

  // Limpa a tela
  system("clear");

	return c;
}

int main(int argc, char *argv[])
{
  int proxy_port = 0;
  int sock, newsock;
	struct hostent *server;
  int n, numbytes, tr = 1, spiderLevel;
  char buffer[5000];
	char request[5000];
	char link[500];
  char junk[50];
  char spiderFileName[150], tmpLinkName[150];
	char ip[100], hostname[500], *reply;
	char choice;
	socklen_t clilen;
	struct sockaddr_in proxy_address, client_address;
	FILE *frequest, *freply, *fcache, *spiderFile;
	char yn;
	int cached = 0;
	spiderList *spider;

  // Limpa a tela
  system("clear");

  // Adicionar a leitura do argumento passado pelo terminal
  if(argc == 3)
	{
		if(strcmp(argv[1], "-p") == 0)
    	proxy_port = atoi(argv[2]);
	}
  else
	{
    proxy_port = 8228;
	}

	printf("Porta escolhida: %d\n", proxy_port);

  while(1)
  {

    topMenu(); //imprime nome do programa

  	// criando socket para receber requisição do browser
  	sock = socket(AF_INET, SOCK_STREAM, 0);

  	if (sock < 0)
  		error("ERROR opening socket");

  	bzero((char *) &proxy_address, sizeof(proxy_address));

  	proxy_address.sin_family = AF_INET;
  	proxy_address.sin_addr.s_addr = inet_addr("127.0.0.1");
  	proxy_address.sin_port = htons(proxy_port);

    // Remove o uso do socket (para evitar erros de bind)
    if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&tr,sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }

  	if (bind(sock, (struct sockaddr *) &proxy_address, sizeof(proxy_address)) < 0)
  		error("ERROR on binding");

    listen(sock, 10);
   	clilen = sizeof(client_address);

   	printf("Aguardando browser...\n\n");

		// libera o socket utiliza outro para lidar com o pedido
		newsock = accept(sock,(struct sockaddr *) &client_address, &clilen);
    if (newsock < 0) error("Erro ao aceitar");

    close(sock);

    // Limpa a tela
    system("clear");

    // Criando o diretório
    system("mkdir -p ./files/proxy");

		bzero(request,sizeof(request));

    n = read(newsock, request, sizeof(request));
    if(n < 0) error("Erro ao ler o socket");

		if((frequest = fopen("files/proxy/request.txt", "w")) == NULL)
			error("Erro ao criar arquivos files/proxy/request.txt");
		fprintf(frequest, "%s", request);
		fclose(frequest);

		GetLinkFromHeader(request, sizeof(request), link, sizeof(link));

    // Seção dropping packages inuteis:
    GetFromText("CONNECT", 1, ':', request, sizeof(request), junk, 50);

    // Verifica se é o lixo do mozilla, se for descarta.
    if (strcmp("push.services.mozilla.com", junk) == 0)
    {
      // Limpa a tela
      system("clear");

      close(newsock);
      continue;
    }

    // Drop no request do site 'http://ocsp.digicert.com/'
    if (strstr(link, "ocsp.") != NULL)
    {
      // Limpa a tela
      system("clear");

      close(newsock);
      continue;
    }

    // Impressão do request
    printf("Request do browser:\n\n");
		printf("%s",request);

		if((fcache = GetHttpFromCache(link)) != NULL)
    {
			printf("\n\n[PROXY] Pedido encontrado na cache\n");
			printf("Deseja retorna o arquivo da cache para o browser? ");
			scanf("%c", &yn);
			getchar();
			yn = tolower(yn);

			if(yn == 'y' || yn == 's')
      {
        ClearString(buffer, sizeof(buffer));

				send(newsock, "HTTP/1.0 200 OK\r\n\r\n", 19, 0);

				while(fread(buffer, 1, sizeof(buffer), fcache) == sizeof(buffer))
        {
					send(newsock, buffer, sizeof(buffer), 0);
				}

				shutdown(newsock, SHUT_RDWR);
				fclose(fcache);
				continue;
			}
		}

		printf("\n\nDeseja editar o pedido antes de enviar para o servidor? (y/n): ");

		scanf("%c", &yn);
		getchar();
		yn = tolower(yn);
		if(yn == 'y' || yn == 's'){
			system("nano files/proxy/request.txt");
		}

		choice = menu();

		switch (choice) {
			case '1':
				send_request();
				printf("\n\n");

				printf("\n\nDeseja editar a resposta antes de enviar para o browser? (y/n): ");

				scanf("%c", &yn);
				getchar();
				yn = tolower(yn);
				if(yn == 'y' || yn == 's'){
					system("nano files/proxy/reply.txt");
				}

				if((freply = fopen("files/proxy/reply.txt", "rb")) == NULL)
					error("Erro ao criar arquivos files/reply.txt");

        ClearString(buffer, sizeof(buffer));

				while(fread(buffer, 1, sizeof(buffer), freply) == sizeof(buffer)){
					send(newsock, buffer, sizeof(buffer), 0);
				}
				shutdown(newsock, SHUT_RDWR);

				fclose(freply);
				break;

			case '2':
        spider = NULL;
				printf("\nSPIDER:\n");
        GetLinkFromHeader(request, sizeof(request), link, sizeof(link));
        GetHostFromHeader(request, sizeof(request), hostname, sizeof(hostname));

        printf("\nDigite a quantidade de níveis desejada no spider (-1 para ilimitado): ");

        scanf("%d", &spiderLevel);
      	getchar();

				Spider(link, hostname, 0, &spider, spiderLevel);
        system("mkdir -p ./files/spider");
        ClearString(spiderFileName, 150);
        ClearString(tmpLinkName, 150);
        GetHttpMainFather(link, spiderFileName, 150);
        GetLinkWithoutHttp(spiderFileName, tmpLinkName, 150);
        ClearString(spiderFileName, 150);
        RemoveChar('/', tmpLinkName, 150, 0);
        strcpy(spiderFileName, "./files/spider/");
        strcat(spiderFileName, tmpLinkName);
        strcat(spiderFileName, ".txt");
        spiderFile = CreateDataFile(spiderFileName);
        SaveToFileSpider(spiderFile, spider, NULL, 0);
        fclose(spiderFile);

				printf("\n\n");
        printf("\nSpider finalizado!\n");
        printf("\nArquivo salvo em %s\n\n", spiderFileName);
        printf("Deseja imprimir o spider no terminal? ");

        scanf("%c", &yn);
    		getchar();
    		yn = tolower(yn);

    		if(yn == 'y' || yn == 's')
        {
          // Limpa a tela
          system("clear");

          PrintSpider(spider, NULL, 0);
          printf("\nDigite 'enter' para continuar...");
          getchar();
    		}

        DeleteSpiderList(&spider);

        // Limpa a tela
        system("clear");

				break;
			case '3':
        spider = NULL;
				printf("\nDUMP:\n");
        GetLinkFromHeader(request, sizeof(request), link, sizeof(link));
        GetHostFromHeader(request, sizeof(request), hostname, sizeof(hostname));
				Spider(link, hostname, 1, &spider, -1);
				DeleteSpiderList(&spider);

				break;

      case '4':
        close(newsock);
        continue;
				break;

			default:
				printf("Opção inválida\n");
		}

    close(newsock);

    // Apaga o ./files/proxy
    system("rm -rf ./files/proxy");

    // Limpa a tela
    system("clear");
	 }

  return 0;
}
