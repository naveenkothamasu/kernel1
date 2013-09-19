
typedef struct{
	char transType;
	time_t transTime;
	long long transAmount; //FIXME: 0 to 2^31 -1
	char *transDesc;
		
}My402SortElem; 

char *substring(char *,char *);
int insertionSort(My402List *pList, My402SortElem *pKey);
void printList(My402List *pList);
void printObj(My402SortElem *obj);
My402SortElem *createSortElem(char **pSortElem);
int verifySortElem(char **pSortElem);
char* trim(char *);
int isTabsOK(char *);
void unlinkSortElements(My402List *);
int getNoDigitsBeforeDecimal(char *);
