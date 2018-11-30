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

parseData parseHtml (char *, int);
FILE * CreateDataFile(char *);
char * GetFromHeader(char *, char *);
