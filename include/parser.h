/*
Propriedade de:
Andre Garrido Damaceno.- mat. 15/0117531
Lucas Campos Jorge - mat. 15/0154135
*/

typedef struct ParseData
{
	char header[2000];
	int headerSize;
  char dataFileName[100];
} parseData;

/*typedef struct AsmList
{
  char Program[204];
  struct AsmList *nextLine, *previousLine;
} asmList;

typedef struct AsmList
{
  char Program[204];
  struct AsmList *nextLine, *previousLine;
} asmList;*/

parseData parseHtml (char *, int);
FILE * CreateDataFile(char *, char *, char *);
char * GetFromText(char *, int, char, char *, int);
void SaveToFile(char *,int, char *);
char * GetLinkFromHeader(char *, int);
