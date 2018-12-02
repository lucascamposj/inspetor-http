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

#ifndef _Parser_library
  #define _Parser_library
    #include "parser.h"
#endif

parseData parseHtml (char *htmlBuffer, int bufferSize)
{
  parseData data;
  FILE *dataFile;
  char name[100];
  int i = 0, wasCrNl = 0, QntCrNl = 0;

  for (i = 0; i < bufferSize-3; i++)
  {
    if (htmlBuffer[i] == '\r' && htmlBuffer[i+1] == '\n' && htmlBuffer[i+2] == '\r' && htmlBuffer[i+3] == '\n')
    {
      data.header[i] = '\r';
      data.header[i+1] = '\n';
      data.header[i+2] = '\0';
      data.headerSize = i+1;
      break;
    }

    data.header[i] = htmlBuffer[i];
  }

  i += 3;

  // Verifica se há dados na resposta (se não tiver, não cria arquivo)
  if (i < bufferSize)
  {
    // Copia o nome do arquivo para a struct
    strcpy(data.dataFileName,name);
    // Cria o arquivo '.html'
    dataFile = CreateDataFile(name, ".html");

    // Popula o arquivo '.html' com os dados (se existir algum)
    for (;i < bufferSize; i++)
    {
      fprintf(dataFile, "%c", htmlBuffer[i]);
    }

    fclose(dataFile);
  }
  else
  {
    data.dataFileName[0] = '-';
    data.dataFileName[1] = '1';
    data.dataFileName[2] = '\0';
  }

  return data;
}

FILE * CreateDataFile(char *name, char *extention)
{
  FILE *dataFile;
  char dataFileName[100];

  // Adicionando o '.asm' no nome do arquivo
  strcpy(dataFileName,name);
  strcat(dataFileName,extention);

  // Criação do arquivo '.html'
  dataFile = fopen(dataFileName,"w");

  if(dataFile == NULL)
  {
    printf("ERRO: Arquivo não criado\n");
    exit(1);
  }

  return dataFile;
}

// parameter - O que buscar, displacement - quando começar a pegar o dado (ex: test: dado\r\n, tem displacement de 2 pq tem ':' e espaço até começar o dado)
// buffer - onde procurar o parameter, bufferSize o tamanho real em memoria do buffer (1000 bytes por exemplo)
// stopSign - Até onde será procurado o dado (ex: test: dado\r\n, nesse caso o stopSign é '\r')
// OBS: Para pegar um dado do header, displacement sempre será 2.
// result - array onde será salvo o dado
// resultSize - tamanho do array result.
void GetFromText(char *parameter, int displacement, char stopSign, char *buffer, int bufferSize, char *result, int resultSize)
{
  char *ptr, *i;
  int paramSize = strlen(parameter) + displacement;
  int j=0;

  bzero(result, resultSize);
  ptr = strstr(buffer, parameter);

  if (ptr != NULL)
  {
    for(i = (ptr + paramSize); i < (buffer + bufferSize) && j < resultSize; i++)
    {
      if (*i == stopSign)
      {
        result[j] = '\0';
        break;
      }

      result[j] = *i;
      j++;
    }
  }
}

void SaveToFile(char *string, int stringSize, char *fileName, char *format)
{
  int i;

  FILE *dataFile;

  dataFile = CreateDataFile(fileName, format);

  for (i = 0; i < stringSize; i++)
  {
    fprintf(dataFile, "%c", string[i]);
  }

  fclose(dataFile);
}

void GetLinkFromHeader(char *headerBuffer, int bufferSize, char *result, int resultSize)
{
  GetFromText("GET", 1, 0x20, headerBuffer, bufferSize, result, resultSize);

  if (result[0] == '\0')
  {
    GetFromText("POST", 1, 0x20, headerBuffer, bufferSize, result, resultSize);
  }
}

void GetHostFromHeader(char *headerBuffer, int bufferSize, char *result, int resultSize)
{
  GetFromText("Host:", 1, '\r', headerBuffer, bufferSize, result, resultSize);
}

void GetHttpFileName(char *link, char *response, int responseSize)
{
  int i, j = 0, slashIndex = 0, linkSize = strlen(link);

  for (i = 0; i < linkSize; i++)
  {
    if (link[i] == '/')
      slashIndex = i;
  }

  for (i = (slashIndex + 1); i < linkSize && j < responseSize; i++)
  {
    response[j] = link[i];
    j++;
  }
}

void GetHttpFolderPath(char *link, char *response, int responseSize)
{
  int i, j = 0, linkSize = strlen(link);
  char mainFather[300], httpFileName[300];
  int mainFatherStringSize, httpFileNameSize, folderPathLimit;

  GetHttpMainFather(link, mainFather, 300);
  GetHttpFileName(link, httpFileName, 300);

  mainFatherStringSize = strlen(mainFather);
  httpFileNameSize = strlen(httpFileName);
  folderPathLimit = linkSize - httpFileNameSize;

  for (i = mainFatherStringSize; i < folderPathLimit && j < responseSize; i++, j++)
  {
    response[j] = link[i];
  }

  response[j] = '\0';
}

void GetHttpMainFather(char *link, char *response, int responseSize)
{
  int i, slashIndex = 0, slashCounter = 0, linkSize = strlen(link);

  for (i = 0; i < linkSize; i++)
  {
    if (link[i] == '/')
    {
      slashCounter += 1;
    }

    if (slashCounter == 3)
    {
      slashIndex = i;
      break;
    }
  }

  for (i = 0; i < slashIndex && i < responseSize; i++)
  {
    response[i] = link[i];
  }

  response[responseSize] = '\0';
}
