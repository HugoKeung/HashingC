#include"shash.h"

#define RANDOMPRIME 31

void initialise(HashStruct* hashStruct, int initialSize, int maxWord, int percent){
   hashStruct->totalWords = 0;
   hashStruct->listSize = initialSize;
   hashStruct->hashList = (Data*)calloc(initialSize, sizeof(Data));
   if (hashStruct->hashList== NULL){
      fprintf(stderr, "ALLOCATION FAILED\n");
      exit(1);   
   }
   hashStruct->maxWord = maxWord;
   hashStruct->percent = percent;
   hashStruct->threshold = hashStruct->listSize * percent/100;
}

Data newData(char* word, int n){
   Data new;
   new.word = calloc(n, sizeof(char));
   if(new.word==NULL){
      fprintf(stderr, "ALLOCATION FAILED\n");
   }
   strcpy(new.word, word);
   new.next = NULL;
   return new;
}

void printList(HashStruct* hashStruct){
   int i;
   for(i = 0; i < hashStruct->listSize; i++){
      if(hashStruct->hashList[i].word != NULL){
         Data* first = &hashStruct->hashList[i];
         printf("%d %s   ", i, hashStruct->hashList[i].word);
         while(first->next != NULL){
            first = first->next;
            printf("%s", first->word);
         }
         printf("\n");   
      }
   }
}
Data* newNode(char* word, int n){
   Data* new;
   
   new = (Data*)calloc(1, sizeof(Data));
   if (new == NULL){
      fprintf(stderr, "ALLOCATION FAILED\n");
      exit(1);
   }
   *new = newData(word, n);

   return new;
}
void insert(HashStruct* hashStruct, char* word){
   
   int hashNum = hash(word, hashStruct->listSize);

   hashStruct->hashList = insertToHash(hashStruct->hashList, word, hashStruct, hashNum);
   hashStruct -> totalWords++;
/*if over threshold or linkCnt is higher than acceptable level, rehash*/
   if(hashStruct -> totalWords > hashStruct->threshold){
      reHash(hashStruct);
   }   
}

void reHash(HashStruct * hashStruct){

   int i, newHash;
   Data* last, *tmpList, *previous;
   int oldSize = hashStruct -> listSize;
   hashStruct -> listSize = newPrime(hashStruct -> listSize);

   tmpList = (Data*)calloc(hashStruct->listSize, sizeof(Data));
   if (!tmpList){
      fprintf(stderr, "ALLOCATION FAILED\n");
      exit(1);
   }
/*cycle through old hashList and assign new hashnum, putting it into tmpList*/
   for (i = 0; i < oldSize; i++){    

      while (hashStruct->hashList[i ].word != NULL){
         goToLast(hashStruct, i, &last, &previous);
         newHash = hash(last->word, hashStruct->listSize);
/*check if tmpList[newHash] is empty or not, if not insert it at the end of list*/
         tmpList = insertToHash(tmpList, last->word, hashStruct, newHash);

         freeData(previous, last);
      }
   }

   free(hashStruct->hashList);
   hashStruct->hashList = tmpList;
   /*update threshold*/
   hashStruct->threshold = hashStruct->listSize * hashStruct->percent/100;

}

Data* insertToHash(Data*hashList, char* word, HashStruct* hashStruct, int hashNum){
/*if is first element insert normally, else have to link it*/
   if (hashList[hashNum].word == NULL){
      hashList[hashNum] = newData(word, hashStruct->maxWord);
   }

   else{      
      linkIt(word, &hashList[hashNum], hashStruct->maxWord);
   }
   return hashList;

}

void linkIt(char* word, Data* first, int n){

   Data* tracker = first;
/*if next is not pointing to anything*/
   while(tracker->next != NULL){
      tracker = tracker->next;
   }   
/*tracker should be the last one now*/
   tracker->next = newNode(word, n);
}

int search(HashStruct* hashStruct, char* word){
/*do check if does not contain word as well!?*/
   int cnt = 1;
   int hashNum = hash(word, hashStruct->listSize);
   Data* current = &hashStruct->hashList[hashNum];

   while (current->word !=NULL){
      if (strcmp(current->word,word)!=0 && current->next != NULL){
         current = current->next;
         cnt++;
      }
/*all possibilities searched*/
      else return cnt;
   }
/*at this point, current is NULL, and no word is found*/
   printf("%s is not in the original file\n", word);
   return -1;

}

void goToLast(HashStruct* hashStruct, int i, Data** last, Data** previous){
   *last = & hashStruct->hashList[i];
   *previous = *last;
   while ((*last)->next != NULL){
      *previous = *last;
      *last = (*last)->next;
   }
      
}

void freeData(Data* previous, Data* last){

   free(last->word);
   last->word = NULL;
/*if 'last' is not the first in list*/
   if(previous != last){
      free(last);
   }
   previous->next = NULL;

}

void freeHash(HashStruct* hashStruct){
   int i;

   for (i = 0; i < hashStruct->listSize; i++){
      while (hashStruct->hashList[i].word != NULL){
         Data* last, * previous;
         goToLast(hashStruct, i, &last, &previous);

         freeData(previous, last);
      }
   }
   free(hashStruct->hashList);

}

unsigned int hash(char* str, int listSize){
   double hash = 0;
   unsigned i;
   for ( i = 0; i < strlen(str); i++) {
      hash = hash*RANDOMPRIME + str[i];
   }
   
   return (unsigned int)hash % listSize;
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

