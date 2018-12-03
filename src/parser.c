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
    dataFile = CreateDataFile(name);

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

FILE * CreateDataFile(char *name)
{
  FILE *dataFile;

  // Criação do arquivo '.html'
  dataFile = fopen(name,"w");

  if(dataFile == NULL)
  {
    printf("ERRO: Arquivo não criado\n");
    exit(1);
  }

  return dataFile;
}

FILE * OpenDataFile(char *name)
{
  FILE *dataFile;

  // Leitura do arquivo
  dataFile = fopen(name,"r");

  if(dataFile == NULL)
  {
    printf("ERRO: Arquivo não aberto\n");
    exit(1);
  }

  return dataFile;
}

void DumpTemp(char *directory)
{
  FILE *finalFile, *tempFile;
  char fileItem;

  RemoveTmpHeader();

  finalFile = CreateDataFile(directory);
  tempFile = OpenDataFile("./tmp/new_server_response.txt");

  // Leitura de caracter em caracter do arquivo, botando os caracteres em maiúsculo
  while ((fileItem = (char) fgetc(tempFile)) != EOF)
  {
    fprintf(finalFile, "%c", fileItem);
  }

  fclose(finalFile);
  fclose(tempFile);

  system("rm ./tmp/new_server_response.txt");
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

void SaveToFile(char *string, int stringSize, char *fileName)
{
  int i;

  FILE *dataFile;

  dataFile = CreateDataFile(fileName);

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
  int i, j = 0, slashIndex = 0, slashCounter = 0, linkSize = strlen(link);

  bzero(response, responseSize);

  for (i = 0; i < linkSize; i++)
  {
    if (link[i] == '/')
    {
      slashIndex = i;
      slashCounter += 1;
    }
  }

  if (slashCounter > 2)
  {
    for (i = (slashIndex + 1); i < linkSize && j < responseSize; i++)
    {
      response[j] = link[i];
      j++;
    }
  }
  else
  {
      strcpy(response, "index.html");
  }
}

void GetHttpFolderPath(char *link, char *response, int responseSize)
{
  int i = 0, j = 0, linkSize = strlen(link);
  char mainFather[300], httpFileName[300];
  int mainFatherStringSize, httpFileNameSize, folderPathLimit;

  bzero(response, responseSize);

  GetHttpMainFather(link, mainFather, 300);
  GetHttpFileName(link, httpFileName, 300);

  mainFatherStringSize = strlen(mainFather);
  httpFileNameSize = strlen(httpFileName);
  folderPathLimit = linkSize - httpFileNameSize;

  for (i = mainFatherStringSize; i < folderPathLimit && j < responseSize; i++, j++)
  {
    response[j] = link[i];
  }
}

void GetHttpMainFather(char *link, char *response, int responseSize)
{
  int i, slashIndex = 0, slashCounter = 0, linkSize = strlen(link);

  bzero(response, responseSize);

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

  if (slashIndex != 0)
  {
    for (i = 0; i < slashIndex && i < responseSize; i++)
    {
      response[i] = link[i];
    }
  }
  else
  {
    strcpy(response, link);
  }
}

void DumpFile(char *link)
{
  char *dump;
  int directoryStringSize, fileNameStringSize, linkSize = strlen(link), nullName = 0;
  char directoryName[linkSize], fileName[linkSize];

  GetHttpFolderPath(link, directoryName, linkSize);
  GetHttpFileName(link, fileName, linkSize);
  directoryStringSize = strlen(directoryName);
  fileNameStringSize = strlen(fileName);

  if (fileNameStringSize == 0)
    nullName = 10;

  dump = (char *)malloc(sizeof(char)*(directoryStringSize + fileNameStringSize + nullName + 16)); // 6 - 'Dump/' + '\0' + nullName se nome vier vazio. (index.html)

  strcpy(dump, "mkdir -p ./Dump");
  strcat(dump, directoryName);
  RemoveChar('/', dump, (directoryStringSize + fileNameStringSize + 16), 1);

  // Cria diretório
  system(dump);

  // Concatenação com o nome do arquivo
  bzero(dump, (directoryStringSize + fileNameStringSize + 16));
  strcpy(dump, "./Dump");
  strcat(dump, directoryName);
  RemoveChar('/', dump, (directoryStringSize + fileNameStringSize + nullName + 16), 1); // Remove a barra no final para garantir que sempre vai existir ela.
  strcat(dump, "/");

  if (fileName[0] == '\0')
  {
    fileName[0] = 'i';
    fileName[1] = 'n';
    fileName[2] = 'd';
    fileName[3] = 'e';
    fileName[4] = 'x';
    fileName[5] = '.';
    fileName[6] = 'h';
    fileName[7] = 't';
    fileName[8] = 'm';
    fileName[9] = 'l';
    fileName[10] = '\0';
  }

  strcat(dump, fileName);

  // Cria o arquivo final no diretório correto e popula ele com 'tmp/server_result.txt' - resposta do server
  DumpTemp(dump);

  free(dump);
}

// Remove apenas o char 'removeChar' que estiver no inicio e no fim da string, ou apenas checa no final caso lastOnly seja maior ou menor que 0
void RemoveChar(char removeChar, char *item, int size, int lastOnly)
{
  int i;
  if(item[0] == removeChar && lastOnly == 0)
  {
    for(i = 0; i<(size - 2); i++)
    {
      item[i] = item[i + 1];
    }
  }

  for(i = (size -2); i > 0; i--)
  {
    if(item[i] == removeChar && item[i+1] == '\0')
    {
      item[i] = '\0';
      break;
    }
  }

  if(item[(size -1)] == removeChar)
    item[(size -1)] = '\0';
}

void RemoveAllFiles()
{
  system("rm -rf ./Dump");
  system("rm -rf ./tmp");
}

void RemoveTmp()
{
  system("rm -rf ./tmp");
}

void CreateTmp()
{
  system("mkdir ./tmp");
}

// Limpa a string por completo, colocando '\0'
void ClearString(char *string, int size)
{
	for (int i = 0; i<size; i++)
		string[i] = '\0';
}

void RemoveTmpHeader()
{
  FILE *tmpFile, *newTmp;
  char txtItem;
  int wasCrNl, wasCr, isHeader = 1;
  tmpFile = OpenDataFile("./tmp/server_response.txt");
  newTmp = CreateDataFile("./tmp/new_server_response.txt");

  while ((txtItem = (char) fgetc(tmpFile)) != EOF)
  {
    if (isHeader == 1)
    {
      if (txtItem == '\n' && wasCr == 1)
      {
        wasCrNl += 1;
      }
      else
      {
        wasCr = 0;

        if (txtItem != '\r')
          wasCrNl = 0;
      }

      if (txtItem == '\r')
        wasCr = 1;

      if (wasCrNl == 2)
      {
        isHeader = 0;
      }
    }
    else
    {
      fprintf(newTmp, "%c", txtItem);
    }
  }

  fclose(tmpFile);
  fclose(newTmp);
}
