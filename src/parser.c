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

parseData parseHtml (char *htmlBuffer, int bufferSize)
{
  parseData data;
  FILE *dataFile;
  char name[100];
  int i = 0, wasCrNl = 0; QntCrNl = 0;

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
    data.dataFileName[0] = '-1';
    data.dataFileName[1] = '\0';
  }
}

FILE * CreateDataFile(char *name)
{
  FILE *dataFile;
  char dataFileName[100];

  // Adicionando o '.asm' no nome do arquivo
  strcpy(dataFileName,name);
  strcat(dataFileName,".html");

  // Criação do arquivo '.html'
  dataFile = fopen(dataFileName,"w");

  if(dataFile == NULL)
  {
    printf("ERRO: Arquivo não criado\n");
    exit(1);
  }
  return dataFile;
}

char * GetFromHeader(char *parameter, char *buffer)
{
  char *value, *ptr;
  int paramSize = strlen(parameter) + 2;
  int i, j=0;

  ptr = strstr(buffer, parameter);

  if (ptr != NULL)
  {
    value = (char *) malloc(sizeof(char)*500);

    for(i = (ptr + paramSize); i < buffer; i++)
    {
      if (buffer[i] == '\r')
      {
        value[j] = '\0';
        break;
      }

      value[j] = buffer[i];
      j++;
    }

    return value;
  }
  
  return NULL;
}
