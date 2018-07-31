#include"dhash.h"
#include"neillsdl2.h"
#define PRIME 1009
#define MAXWORD 30
#define PERCENT 60
#define DELAY 10
#define PADDING 50
#define BOXHEIGHT 200

void readFile(FILE* fp, HashStruct* hashStruct);
double averageSearches(FILE* fp, HashStruct* hashStruct);
void dictionarySearch(FILE* fp, HashStruct * hashStruct);
void fillBox(SDL_Simplewin* sw, SDL_Rect outline, HashStruct* hashStruct, char* word);
void reset(SDL_Simplewin* sw);
SDL_Rect printOutline(SDL_Simplewin* sw);
void update(SDL_Simplewin* sw);

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

   if (argc ==3){
      fp2 = fopen(argv[2], "r");
      dictionarySearch(fp2, & hashStruct);
   }

   freeHash(&hashStruct);

   return 0;
   
}
void dictionarySearch(FILE* fp, HashStruct * hashStruct){
   if (fp == NULL){
      fprintf(stderr,"FAILED TO OPEN FILE\n");
      exit(1);
   }
   printf("average searches: %.3f\n", averageSearches(fp, hashStruct));
   fclose(fp);
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
   SDL_Simplewin sw;
   Neill_SDL_Init(&sw);
   if (fp == NULL){
      fprintf(stderr,"FAILED TO OPEN FILE\n");
      exit(1);
   }

   if (word == NULL){
      fprintf(stderr,"ALLOCATION FAILED\n");
      exit(1);
   }
	do{
		printOutline(&sw);
		Neill_SDL_Events(&sw);
		while (fscanf(fp, "%s", word)!=EOF && !sw.finished){
		   fillBox(&sw, printOutline(&sw), hashStruct, word);
		   putInList(word, hashStruct->hashList, hashStruct->listSize, hashStruct->maxWord);
		   hashStruct -> totalWords++;
		   if(hashStruct -> totalWords > hashStruct->threshold){
		      reset(&sw);
		      reHash(hashStruct);	   
		   }  
		   SDL_Delay(DELAY);
		   Neill_SDL_Events(&sw);
		} 
	}while(!sw.finished);
   free(word);
   fclose(fp);   

}
SDL_Rect printOutline(SDL_Simplewin* sw){
   int widthPadding = 10;
   SDL_Rect outline; 
   outline.w = WWIDTH-widthPadding;
   outline.h = BOXHEIGHT;
   Neill_SDL_SetDrawColour(sw, 123, 0, 0);
   outline.x = widthPadding/2;
   outline.y = PADDING;
   SDL_RenderDrawRect(sw->renderer, &outline);
   update(sw);
   return outline;
}
void fillBox(SDL_Simplewin* sw, SDL_Rect outline, HashStruct* hashStruct, char* word){
   SDL_Rect fill;
   Neill_SDL_SetDrawColour(sw, 123, 123, 123);
   fill.w = 1;
   fill.h = outline.h;
   fill.x = 1.0*outline.w/hashStruct->listSize*(findHash(hashStruct->hashList, word, hashStruct->listSize))+outline.x;
   fill.y = outline.y;
   SDL_RenderFillRect(sw->renderer, &fill);
   update(sw);
}

void update(SDL_Simplewin* sw){

   SDL_RenderPresent(sw->renderer);
   SDL_UpdateWindowSurface(sw->win); 
}
void reset(SDL_Simplewin* sw){
   Neill_SDL_SetDrawColour(sw, 0, 0, 0);
   SDL_RenderClear(sw->renderer);
   printOutline(sw);
}
