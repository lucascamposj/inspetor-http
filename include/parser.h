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
FILE * OpenDataFile(char *name);
void GetFromText(char *, int, char, char *, int, char *, int);
void SaveToFile(char *,int, char *);
void GetLinkFromHeader(char *, int, char *, int);
void GetHttpFileName(char *, char *, int);
void GetHostFromHeader(char *, int, char *, int);
void GetHttpMainFather(char *, char *, int);
void GetHttpFolderPath(char *, char *, int );
void DumpFile(char *);
void RemoveChar(char, char *, int, int);
void DumpTemp(char *);
void RemoveAllFiles();
void RemoveTmp();
void CreateTmp();
void ClearString(char *, int);
void RemoveTmpHeader();
int StringContains(char *, char, int);
int StringContainsAtEnd(char *, char, int);
int StringLenth(char *);
