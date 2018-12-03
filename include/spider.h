/*
Propriedade de:
Andre Garrido Damaceno.- mat. 15/0117531
Lucas Campos Jorge - mat. 15/0154135
*/

typedef struct SpiderList
{
  char Link[500];
  struct SpiderList *nextLink, *previousLink, *fatherLink;
} spiderList;

typedef struct VisitedList
{
  char Link[500];
  struct VisitedList *nextLink, *previousLink;
} visitedList;

void AddSpiderList(spiderList **, spiderList *, char *);
void AddVisitedList(visitedList **, char *);
void DeleteSpiderList(spiderList **);
int VisitedListContains(visitedList *, char *); // Verifica se a lista visited já contém o link. Retorna 1 se tiver, 0 se não.
void Spider(char *, char *, int, spiderList **);
void AddVisitedList(visitedList **, char *);
void DeleteVisitedList(visitedList **);
void PrintSpider(spiderList *, spiderList *, int);
void PrintVisited(visitedList *);
