#include"shash.h"

#define PRIME 1009
#define MAXWORD 30
#define PERCENT 60

void readFile(FILE* fp, HashStruct* hashStruct);
double averageSearches(FILE* fp, HashStruct* hashStruct);
void dictionarySearch(FILE* fp, HashStruct * hashStruct);

int main (int argc, char* argv[]){

   FILE* fp, *fp2;
   HashStruct hashStruct;

   if (argc < 2){
      fprintf(stderr,"WRONG PROGRAMME USAGE\n%s FILENAME (FILENAME)\n",argv[0]);
      exit (1);
   }
   
   initialise(&hashStruct, PRIME, MAXWORD, PERCENT);

   fp = fopen(argv[1], "r");

   readFile(fp, &hashStruct);   
   printList(&hashStruct);
   if (argc ==3){
      fp2 = fopen(argv[2], "r");
      printf("average searches:%.3f\n", averageSearches(fp2, &hashStruct));
   }

   freeHash(&hashStruct);

   return 0;
   
}

double averageSearches(FILE* fp, HashStruct* hashStruct){

   int searches = 0;
   int cnt = 0;
   double average;
   int result;
   char* word = (char*)calloc(MAXWORD, sizeof(char));
   if (word == NULL){
      fprintf(stderr,"ALLOCATION FAILED\n");
      exit(1);
   }
  if (fp == NULL){
      fprintf(stderr,"FAILED TO OPEN FILE\n");
      exit(1);
   }
   while(fscanf(fp, "%s", word)!=EOF){
      result = search(hashStruct, word);
      if (result != -1){
         searches += result;
         cnt ++;
      }
   }   
   average = (double)searches/cnt;
  
   free(word);
   return average;
}

void readFile(FILE* fp, HashStruct * hashStruct){

   char* word= (char*) calloc(MAXWORD, sizeof(char));

   if (fp == NULL){
      fprintf(stderr,"FAILED TO OPEN FILE\n");
      exit(1);
   }

   if (word == NULL){
      fprintf(stderr,"ALLOCATION FAILED\n");
      exit(1);
   }

   while (fscanf(fp, "%s", word)!=EOF){
      insert(hashStruct, word);
   }

   free(word);
   fclose(fp);   

}
