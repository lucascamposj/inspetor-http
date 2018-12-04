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

#ifndef _Spider_library
  #define _Spider_library
    #include "spider.h"
#endif

void Spider(char *link, char *hostname, int isDump, spiderList **spiderListHead)
{
  char newLink[500], tmpLink[500], txtLine[500], fatherLink[500], *j, *ptr;
  int i = 0, w, k, txtItem, fatherLinkSize;
  spiderList *linkToVisit;
  visitedList *visitedListHead;
  FILE *tmpFile;

  // Zera todas as strings no inicio
  ClearString(newLink, 500);
  ClearString(tmpLink, 500);
  ClearString(txtLine, 500);
  ClearString(fatherLink, 500);

  AddSpiderList(spiderListHead, NULL, link);
  visitedListHead = NULL;
  linkToVisit = *spiderListHead;
  GetHttpMainFather(link, fatherLink, 500);

  if (StringContainsAtEnd(fatherLink, '/', 500) == 1)
  {
    RemoveChar('/', fatherLink, 500, 1);
  }

  fatherLinkSize = StringLenth(fatherLink);

  strcpy(newLink, link);

  while (linkToVisit != NULL)
  {
    if (VisitedListContains(visitedListHead, newLink) == 0 && StringContains(newLink, '#', StringLenth(newLink)) == 0)
    {
      AddVisitedList(&visitedListHead, newLink);  // Adiciona link nos visitados
      RemoveTmp();
      CreateTmp();
      
      get_server_response(hostname, newLink); // (pede um novo arquivo do 'tmp' com o newLink)

      if (isDump == 1)
      {
        DumpFile(newLink);
      }

      tmpFile = OpenDataFile("./tmp/server_response.txt");

      ClearString(txtLine, 500);
      ClearString(tmpLink, 500);

      while ((txtItem = (char) fgetc(tmpFile)) != EOF)
      {
        txtLine[i] = txtItem;
        i++;

        if (txtItem == '\n' || i > 498)
        {
          i = 0;
          ptr = strstr(txtLine, "href=\"");

          if(ptr == NULL)
          {
            ptr = strstr(txtLine, "src=\"");

            if(ptr != NULL && ptr < (&txtLine[500] - 5))
            {
              j = ptr + 5;
            }
          }
          else
          {
            if (ptr < (&txtLine[500] - 5))
              j = ptr + 6;
          }

          if (ptr != NULL)
          {
            w = 0;
            while (*j != '"' && *j != '?' && w < 500 && j < &txtLine[500])
            {
              if(*j != 0x20 && *j != '\r' && *j != 0x09)
              {
                tmpLink[w] = *j;
              }

              j++;
              w++;
            }

            if ((tmpLink[0] == '/' || LinkHasHttpOrHttps(tmpLink) == 0) && LinkHasMailTo(tmpLink) == 0)
            {
              RemoveChar('/', tmpLink, 500, 0); // Remove a '/' do inicio e final da string, se tiver.

              for (k = 0; k < fatherLinkSize; k++)
                newLink[k] = fatherLink[k];

              newLink[fatherLinkSize] = '/';

              for (k = 1; k < 500; k++)
              {
                newLink[k + fatherLinkSize] = tmpLink[k-1];

                if (tmpLink[k] == '\0')
                  break;
              }

              for (int k = 0; k < 500; k++)
              {
                tmpLink[k] = newLink[k];

                if (tmpLink[k] == '\0')
                  break;
              }
            }

            GetHttpMainFather(tmpLink, newLink, 500);

            if (strcmp(fatherLink, newLink) == 0 && StringContains(newLink, '#', StringLenth(newLink)) == 0)
            {
              AddSpiderList(spiderListHead, linkToVisit, tmpLink);
            }
          }

          ClearString(txtLine, 500);
          ClearString(tmpLink, 500);
        }
      }

      fclose(tmpFile);
    }

    ClearString(newLink, 500);
    ClearString(txtLine, 500);
    ClearString(tmpLink, 500);

    linkToVisit = linkToVisit->nextLink;

    // Se ainda não estiver acabado a lista (para não dar problemas...)
    if (linkToVisit != NULL)
      strcpy(newLink, linkToVisit->Link);
  }

  DeleteVisitedList(&visitedListHead);
}

void PrintSpider(spiderList *spiderListHead, spiderList *spiderFather, int tabNum)
{
  spiderList *mover;
  mover = spiderListHead;
  int i;

  while (mover != NULL)
  {
    if (mover->fatherLink == spiderFather)
    {
      for (i = 0; i < tabNum; i++)
      {
        printf("%c", 0x09);
      }

      printf("%s\n", mover->Link);
      PrintSpider(spiderListHead, mover, tabNum + 1);
    }
    mover = mover->nextLink;
  }
}

void AddSpiderList(spiderList **spiderListHead, spiderList *fatherLink, char *link)
{
	spiderList *contentCreator, *lastElem;

	// Criação da lista do spider final
	contentCreator = (spiderList *)malloc(sizeof(spiderList));

	if (*spiderListHead == NULL)
	{
		*spiderListHead = contentCreator;
		(*spiderListHead)->nextLink = NULL;
		(*spiderListHead)->previousLink = NULL;
	}
	else
	{
		lastElem = *spiderListHead;

		while (lastElem->nextLink != NULL)
			lastElem = lastElem->nextLink;

		contentCreator->previousLink = lastElem;
		lastElem->nextLink = contentCreator;
		contentCreator->nextLink = NULL;
	}

  ClearString(contentCreator->Link, 500);
	strcpy(contentCreator->Link, link);
  contentCreator->fatherLink = fatherLink;
}

void DeleteSpiderList(spiderList **spiderListHead)
{
	spiderList *aux;

	while (*spiderListHead != NULL)
	{
		aux = *spiderListHead;
		*spiderListHead = (*spiderListHead)->nextLink;
		free(aux);
	}
}

int VisitedListContains(visitedList *visitedListHead, char *link)
{
  while (visitedListHead != NULL)
  {
    if (strcmp(link, visitedListHead->Link) == 0)
      return 1;

    visitedListHead = visitedListHead->nextLink;
  }

  return 0;
}

void AddVisitedList(visitedList **visitedListHead, char *link)
{
  visitedList *contentCreator, *lastElem;

	// Criação da lista do spider final
	contentCreator = (visitedList *)malloc(sizeof(visitedList));

	if (*visitedListHead == NULL)
	{
		*visitedListHead = contentCreator;
		(*visitedListHead)->nextLink = NULL;
		(*visitedListHead)->previousLink = NULL;
	}
	else
	{
		lastElem = *visitedListHead;

		while (lastElem->nextLink != NULL)
			lastElem = lastElem->nextLink;

		contentCreator->previousLink = lastElem;
		lastElem->nextLink = contentCreator;
		contentCreator->nextLink = NULL;
	}

  ClearString(contentCreator->Link, 500);
	strcpy(contentCreator->Link, link);
}

void DeleteVisitedList(visitedList **visitedListHead)
{
	visitedList *aux;

	while (*visitedListHead != NULL)
	{
		aux = *visitedListHead;
		*visitedListHead = (*visitedListHead)->nextLink;
		free(aux);
	}
}

void PrintVisited(visitedList *visitedListHead)
{
  printf("\nVisitados:\n");
  while (visitedListHead != NULL)
  {
    printf("%s\n", visitedListHead->Link);
    visitedListHead = visitedListHead->nextLink;
  }
}
