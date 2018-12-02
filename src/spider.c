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

void Spider(char *link, int isDump, spiderList **spiderListHead, int deepNess)
{
  char newLink[500];
  spiderList *linkToVisit;
  visitedList *visitedListHead;

  visitedListHead = NULL;
  AddSpiderList(spiderListHead, NULL, link);

  linkToVisit = *spiderListHead;
  strcpy(newLink, link);

  while (linkToVisit != NULL)
  {
  //  if (VisitedListContains(visitedListHead, newLink) == 0)
    {
  //    AddVisitedList(&visitedListHead, newLink);  // Adiciona link nos visitados

      // GetHttpContent(newLink) (pede um novo arquivo do 'tmp' com o newLink)

      if (isDump == 1)
      {
        DumpFile(newLink);
      }

      while (newLink[0] != '\0')
      {
        AddSpiderList(spiderListHead, linkToVisit, newLink);
        // AnaliseHttp(newLink);
      }
    }

    linkToVisit = linkToVisit->nextLink;
    strcpy(newLink, linkToVisit->Link);
  }

//  DeleteVisitedList(&visitedListHead);
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
