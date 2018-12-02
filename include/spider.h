/*
Propriedade de:
Andre Garrido Damaceno.- mat. 15/0117531
Lucas Campos Jorge - mat. 15/0154135
*/

typedef struct ListOfLinks
{
  char Link[500];                   // Link
  struct ListOfLinks *nextLink, *previousLink;
} listOfLinks;

typedef struct ListOfList
{
  struct ListOfLinks *listHead;     // Inicio da lista de links
  struct ListOfList *nextList, *previousList;
} listOfList;

typedef struct SpiderList
{
  char Link[500];
  int offSet;                     // Quantos tabs serão impressos
  struct SpiderList *nextLink, *previousLink;
} spiderList;

void AddLink(listOfLinks **, listOfList **, char *);
void AddListOfList(listOfList **, listOfLinks *);
