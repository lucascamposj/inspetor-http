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
void GetFromText(char *, int, char, char *, int, char *, int);
void SaveToFile(char *,int, char *);
void GetLinkFromHeader(char *, int, char *, int);
void GetHttpFileName(char *, char *, int);
void GetHostFromHeader(char *, int, char *, int);
void GetHttpMainFather(char *, char *, int);
void GetHttpFolderPath(char *, char *, int );
void DumpFile(char *, char *);
void RemoveChar(char, char *, int, int);
