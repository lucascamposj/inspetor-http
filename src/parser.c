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
    dataFile = CreateDataFile(name, ".html", "w");

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

FILE * CreateDataFile(char *name, char *extention, char *mode)
{
  FILE *dataFile;
  char dataFileName[100];

  // Adicionando o '.asm' no nome do arquivo
  strcpy(dataFileName,name);
  strcat(dataFileName,extention);

  // Criação do arquivo '.html'
  dataFile = fopen(dataFileName,mode);

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
// OBS: Deve ser feito free() com o retorno do parametro dessa função, após termino de uso.
char * GetFromText(char *parameter, int displacement, char stopSign, char *buffer, int bufferSize)
{
  char *value, *ptr, *i;
  int paramSize = strlen(parameter) + displacement;
  int j=0;

  ptr = strstr(buffer, parameter);

  if (ptr != NULL)
  {
    value = (char *) malloc(sizeof(char)*500);

    for(i = (ptr + paramSize); i < (buffer + bufferSize); i++)
    {
      if (*i == stopSign)
      {
        value[j] = '\0';
        break;
      }

      value[j] = *i;
      j++;
    }

    return value;
  }

  return NULL;
}

void SaveToFile(char *string, int stringSize, char *fileName)
{
  int i;
  FILE *dataFile = CreateDataFile(fileName, ".txt", "w");

  for (i = 0; i < stringSize; i++)
  {
    fprintf(dataFile, "%c", string[i]);
  }

  fclose(dataFile);
}

char * GetLinkFromHeader(char *headerBuffer, int bufferSize)
{
  char *finalString;

  finalString = GetFromText("GET", 1, 0x20, headerBuffer, bufferSize);

  if (finalString == NULL)
  {
    finalString = GetFromText("POST", 1, 0x20, headerBuffer, bufferSize);
  }

  return finalString;
}
