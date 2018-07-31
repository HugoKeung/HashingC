#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct hashStruct{
   int totalWords;
   int maxWord;
   int listSize;
   char** hashList;
   int percent;
   int threshold;
}; typedef struct hashStruct HashStruct; 

/*main functions*/
void initialise(HashStruct* hashStruct, int initialSize, int maxWord, int percent);
void insert(HashStruct* hashStruct, char* word);
int search(HashStruct* hashStruct, char* word);
void freeHash(HashStruct* hashStruct);
void unHash(HashStruct* hashStruct, int hashNum);

void reHash(HashStruct * hashStruct);
int isPrime(int n);
int newPrime(int listSize);
unsigned int doubleHash(char* str, int listSize);
unsigned int hash(char* str, int listSize);
int findHash(char** hashList, char* word, int listSize);
void putInList(char* word, char** hashList, int n, int maxWord);

