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
FILE * CreateDataFile(char *, char *);
void GetFromText(char *, int, char, char *, int, char *, int);
void SaveToFile(char *,int, char *, char *);
void GetLinkFromHeader(char *, int, char *, int);
char * GetWgetFileName(char *);
void GetHostFromHeader(char *, int, char *, int);
