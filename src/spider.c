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

#ifndef _Spider_library
  #define _Spider_library
    #include "spider.h"
#endif

void AddLink(listOfLinks **listOfLinksHead, listOfList **listOfListHead, char *link)
{
	listOfLinks *contentCreator, *lastElem;

	// Criação da lista de links
	contentCreator = (listOfLinks *)malloc(sizeof(listOfLinks));

	if (*listOfLinksHead == NULL)
	{
		*listOfLinksHead = contentCreator;
		(*listOfLinksHead)->nextLink = NULL;
		(*listOfLinksHead)->previousLink = NULL;
    AddListOfList(listOfListHead, *listOfLinksHead);
	}
	else
	{
		lastElem = *listOfLinksHead;

		while (lastElem->nextLink != NULL)
			lastElem = lastElem->nextLink;

		contentCreator->previousLink = lastElem;
		lastElem->nextLink = contentCreator;
		contentCreator->nextLink = NULL;
	}

	strcpy(contentCreator->Link, link);
}

void AddListOfList(listOfList **listOfListHead, listOfLinks *listOfLinksHead)
{
	listOfList *contentCreator, *lastElem;

	// Criação da lista de listas
	contentCreator = (listOfList *)malloc(sizeof(listOfList));

	if (*listOfListHead == NULL)
	{
		*listOfListHead = contentCreator;
		(*listOfListHead)->nextList = NULL;
		(*listOfListHead)->previousList = NULL;
	}
	else
	{
		lastElem = *listOfListHead;

		while (lastElem->nextList != NULL)
			lastElem = lastElem->nextList;

		contentCreator->previousList = lastElem;
		lastElem->nextList = contentCreator;
		contentCreator->nextList = NULL;
	}

	contentCreator->listHead =  listOfLinksHead;
}

void AddSpiderList(spiderList **spiderListHead, char *link, int offSet)
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
  contentCreator->offSet = offSet;
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

void DeleteLink(listOfLinks **listOfLinksHead)
{
	spiderList *aux;

	while (*spiderListHead != NULL)
	{
		aux = *spiderListHead;
		*spiderListHead = (*spiderListHead)->nextLink;
		free(aux);
	}
}
