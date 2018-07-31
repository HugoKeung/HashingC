#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct data{
   char* word;
   struct data* next;
}; typedef struct data Data;

struct hashStruct{
   int totalWords;
   int maxWord;
   int listSize;
   Data* hashList;
   int percent;
   int threshold;
}; typedef struct hashStruct HashStruct; 

/*main functions*/
void insert(HashStruct* hashStruct, char* word);
void initialise(HashStruct* hashStruct, int initialSize, int maxWord, int percent);
int search(HashStruct* hashStruct, char* word);
void freeHash(HashStruct* hashStruct);
void printList(HashStruct* hashStruct);

void reHash(HashStruct * hashStruct);
int isPrime(int n);
int newPrime(int listSize);
unsigned int hash(char* str, int listSize);
Data* insertToHash(Data*hashList, char* word, HashStruct* hashStruct, int hashNum);
void linkIt(char* word, Data* first, int n);
Data* newNode(char* word, int n);
Data newData(char* word, int n);
Data* lastinList(HashStruct* hashStruct, int i);
void goToLast(HashStruct* hashStruct, int i, Data** last, Data** previous);
void freeData(Data* previous, Data* last);
void printList(HashStruct* hashStruct);
