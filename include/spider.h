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

void AddSpiderList(spiderList **, spiderList *, char *);
void DeleteSpiderList(spiderList **);
int SpiderListContains(spiderList *, char *); // Verifica se a lista spider já contém o link. Retorna 1 se tiver, 0 se não.
void Spider(int, spiderList **, int);
