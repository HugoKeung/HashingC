#include"dhash.h"

#define RANDOMPRIME 31

void initialise(HashStruct* hashStruct, int initialSize, int maxWord, int percent){
   hashStruct->totalWords = 0;
   hashStruct->listSize = initialSize;
   hashStruct->hashList = (char**)calloc(initialSize, sizeof(char*));
   if(hashStruct->hashList == NULL){
      fprintf(stderr,"ALLOCATION FAILED\n");
      exit(1);
   }
   hashStruct->maxWord = maxWord;
   hashStruct->percent = percent;
   hashStruct->threshold = hashStruct->listSize * percent/100;
}

int search(HashStruct* hashStruct, char* word){
   int initialHash = hash(word, hashStruct->listSize);
   int cnt = 1;
   int doubleHashed = doubleHash(word, hashStruct->listSize);
   int finalHash = initialHash;

   while(hashStruct->hashList[finalHash] != NULL){

      if (strcmp(hashStruct->hashList[finalHash],word)!=0){
         finalHash += doubleHashed;
         finalHash = finalHash % hashStruct->listSize;
         cnt++;
      }
      else return cnt;
   }
/*at this point, hashList[finalHash] is NULL*/
   printf("%s is not in the original file\n", word);
   return -1;

}

void insert(HashStruct* hashStruct, char* word){
   putInList(word, hashStruct->hashList, hashStruct->listSize, hashStruct->maxWord);
   hashStruct -> totalWords++;
   if(hashStruct -> totalWords > hashStruct->threshold){
      reHash(hashStruct);
   }   
}

void reHash(HashStruct * hashStruct){
   int i;
   char** tmpList;
   int oldSize = hashStruct -> listSize;
   hashStruct -> listSize = newPrime(hashStruct -> listSize);
   tmpList = (char**)calloc(hashStruct->listSize, sizeof(char*));
   if (tmpList == NULL){
      fprintf(stderr,"ALLOCATION FAILED\n");
      exit(1);
   }
/*cycle through old hashList and assign new hashnum, putting it into tmpList*/
   for (i = 0; i < oldSize; i++){ 
      if (hashStruct->hashList[i] != NULL){
         putInList(hashStruct->hashList[i], tmpList, hashStruct->listSize, hashStruct->maxWord);
         free(hashStruct->hashList[i]);
      }
   }
/*at this point, hashList content should be free, and tmpList is filled with previous words*/
   free(hashStruct->hashList);
   hashStruct->hashList = tmpList;
   /*update threshold*/
   hashStruct->threshold = hashStruct->listSize * hashStruct->percent/100;
}

void putInList(char* word, char** hashList, int n, int maxWord){
   int hashNum = findHash(hashList, word, n);
   hashList[hashNum] = (char*) calloc (maxWord, sizeof(char));
   if (hashList[hashNum] == NULL){
      fprintf(stderr,"ALLOCATION FAILED\n");
      exit(1);
   }
   strcpy(hashList[hashNum], word); 
}

void unHash(HashStruct* hashStruct, int hashNum){

   free(hashStruct->hashList[hashNum]);
   hashStruct->hashList[hashNum] = NULL;
   hashStruct->totalWords--;

}
void freeHash(HashStruct* hashStruct){
   int i;
   for (i = 0; i < hashStruct->listSize; i++){
      if(hashStruct->hashList[i] != NULL){
         free(hashStruct->hashList[i]);
      }
   }
   free(hashStruct->hashList);
}

int findHash(char** hashList, char* word, int listSize){
   
   int firstHash = hash(word, listSize);
   int secondHash = doubleHash(word, listSize);
   int finalHash = firstHash;

   while (hashList[finalHash] != NULL){
      finalHash += secondHash;
      finalHash = finalHash % listSize;
   }
   return finalHash;
}

unsigned int hash(char* str, int listSize){
   double hash = 0;
   unsigned int i;
   for ( i = 0; i < strlen(str); i++) {
      hash = hash*RANDOMPRIME + str[i];
   }
   return (unsigned int)hash % listSize;
}

unsigned int doubleHash(char* str, int listSize){   
   int i;
   int n = strlen(str);
   unsigned int hash = 0;

   for (i = 0; i < n; i++){
      hash = str[i] + RANDOMPRIME*hash;
   }
   if (1 > hash%listSize){
      return 1;
   }
   else return hash % listSize;
   
}

int newPrime(int listSize){
   int doubled = listSize * 2;
   while (isPrime(doubled) == 0){
      doubled++;
   }
   return doubled;
}

/*0 not prime, 1 prime*/
int isPrime(int n){
   int i;
   if (n <=1){
      return 0;
   }
   if (n %2 ==0 && n > 2){
      return 0;
   }
   for (i = 3; i < n; i+=2){
      if (n%i == 0){
         return 0;
      }
   }
   return 1;
}

