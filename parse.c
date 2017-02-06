#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NO_OF_COUNTRIES 215
#define NO_OF_FILES 1000
#define MAX_FILE_LEN 250

typedef struct countrydict{
    char **Name;
    unsigned int CountryCnt;
}   CountryDict;


FILE *Create_ResultFile (CountryDict *PrimeDict);
void Save_Dictionary(char **argv, CountryDict *PrimeDict);
void Test_Dictionary (CountryDict *PrimeDict);
void Scan_Files(FILE *Result_File, CountryDict *PrimeDict);
void Search_in_File(FILE *Result_File, char *fname, CountryDict *PrimeDict);
int CountryFreq_Count(char *Name, FILE *fp);
int Exceptions(char *Name, char *file_contents);
int Word_Count (FILE *fname);


int main(int argc, char **argv){
    FILE *Result_File;
    CountryDict PrimeDict;

    Save_Dictionary(argv, &PrimeDict);
    Result_File = Create_ResultFile(&PrimeDict);
    Test_Dictionary(&PrimeDict);
    Scan_Files(Result_File, &PrimeDict);

    printf("\n\n----------------\nFinished Scanning Documents...");

    return(0);
}

FILE *Create_ResultFile (CountryDict *PrimeDict){
    unsigned int i;
    FILE *Result_File = NULL;
    Result_File = fopen("Results.txt", "w");

    fprintf(Result_File, "ID,Word Count");
    for (i = 0; i <  PrimeDict->CountryCnt-1; i++){
        fprintf(Result_File, ",%s", PrimeDict->Name[i]);
    }
    fprintf(Result_File, "\n");
    return Result_File;
}

void Save_Dictionary(char **argv, CountryDict *PrimeDict){
    FILE *dict_file;
    char TmpWord[1000];
    unsigned int i = 0;
    PrimeDict->CountryCnt = 0;

    /* Open File & Check First File*/
    if((dict_file = fopen(argv[1], "r")) == NULL){
        fprintf(stderr, "ERROR: Cannot open [%s]\n", argv[1]);
        exit(1);
    }

    while(fgets(TmpWord, 1000, dict_file) != NULL){
        ++PrimeDict->CountryCnt;
    }
    rewind(dict_file);

    PrimeDict->Name = calloc(sizeof(char*), PrimeDict->CountryCnt);
    for ( i = 0; i < PrimeDict->CountryCnt; i++){
        PrimeDict->Name[i] = calloc(sizeof(char), 1000);
    }
    i = 0;
    while(fgets(TmpWord, 100, dict_file) != NULL){
        TmpWord[strcspn(TmpWord, "\n")] = 0;
        memcpy(PrimeDict->Name[i], TmpWord, 1000);
        ++i;
    }

    fclose(dict_file);
    printf("Saved into Dict \n");
}

void Test_Dictionary (CountryDict *PrimeDict){
    int i;
    printf("\n\n DICTIONARY:\n--------------------\n");

    for(i=0; i< (int)PrimeDict->CountryCnt; ++i){
        printf("%s\n", PrimeDict->Name[i]);
    }
}

void Scan_Files(FILE *Result_File, CountryDict *PrimeDict){
    DIR *d;
    struct dirent *dir;
    char TmpString[1000];
    d = opendir(".");

    if (d){
        while ((dir = readdir(d)) != NULL){
          printf("Processing %s\n", dir->d_name);
          strcpy(TmpString, dir->d_name);
          Search_in_File(Result_File, TmpString, PrimeDict);
          printf("  Processed %s\n", dir->d_name);
        }
    closedir(d);
    }
}

void Search_in_File(FILE *Result_File, char *fname, CountryDict *PrimeDict){
	FILE *fp;
    int i;

    if(fname[0] == '.'){
           printf("Linux File Detected - Return\n");
           return;
       }

    if((fp = fopen(fname, "r")) == NULL) {
		printf("ERROR: Cannot open file %s\n", fname);
        exit(1);
	}

    fprintf(Result_File, "%s,", fname);
    fprintf(Result_File,"%d,", Word_Count(fp));
    for (i = 0; i < (int)PrimeDict->CountryCnt; i++){
        fprintf(Result_File,"%d,", CountryFreq_Count(PrimeDict->Name[i], fp));
    }
    fprintf(Result_File,"\n");
    fclose(fp);
}

int CountryFreq_Count(char *Name, FILE *fp) {
  int cnt = 0;
  char *file_contents, *ptr = NULL;
  long input_file_size;

  fseek(fp, 0, SEEK_END);
  input_file_size = ftell(fp);
  rewind(fp);
  file_contents = malloc(input_file_size * (sizeof(char)));
  fread(file_contents, sizeof(char), input_file_size, fp);

  ptr = file_contents;
  while((ptr = strstr(ptr, Name)) != NULL) {
    cnt++;
    ptr+=strlen(Name);
  }
  cnt = cnt - Exceptions(Name, file_contents);

  free(file_contents);
  return cnt;
}

int Exceptions(char *Name, char *file_contents) {
  int cnt = 0;
  char *ptr;
  if(!strcmp(Name, "Guinea")) {
    ptr = file_contents;
    while((ptr = strstr(ptr, "Equatorial Guinea")) != NULL) {
      cnt++;
      ptr+=strlen(Name);
    }
    ptr = file_contents;
    while((ptr = strstr(ptr, "Guinea Bissau")) != NULL) {
      cnt++;
      ptr+=strlen(Name);
    }
    ptr = file_contents;
    while((ptr = strstr(ptr, "Papua New Guinea")) != NULL) {
      cnt++;
      ptr+=strlen(Name);
    }
  }
  if(!strcmp(Name, "Russia")) {
    ptr = file_contents;
    while((ptr = strstr(ptr, "Russian Federation")) != NULL) {
      cnt++;
      ptr+=strlen(Name);
    }
  }
  if(!strcmp(Name, "Ireland")) {
    ptr = file_contents;
    while((ptr = strstr(ptr, "Northern Ireland")) != NULL) {
      cnt++;
      ptr+=strlen(Name);
    }
  }
  if(!strcmp(Name, "Niger")) {
    ptr = file_contents;
    while((ptr = strstr(ptr, "Nigeria")) != NULL) {
      cnt++;
      ptr+=strlen(Name);
    }
  }
  if(!strcmp(Name, "Syria")) {
    ptr = file_contents;
    while((ptr = strstr(ptr, "Syrian Arab Republic")) != NULL) {
      cnt++;
      ptr+=strlen(Name);
    }
  }
  if(!strcmp(Name, "Sudan")) {
    ptr = file_contents;
    while((ptr = strstr(ptr, "South Sudan")) != NULL) {
      cnt++;
      ptr+=strlen(Name);
    }
  }
  if(!strcmp(Name, "Dominica")) {
    ptr = file_contents;
    while((ptr = strstr(ptr, "Dominican Republic")) != NULL) {
      cnt++;
      ptr+=strlen(Name);
    }
  }
  if(!strcmp(Name, "Republic of Korea")) {
    ptr = file_contents;
    while((ptr = strstr(ptr, "Democratic People's Republic of Korea")) != NULL) {
      cnt++;
      ptr+=strlen(Name);
    }
  }
  return cnt;
}

int Word_Count (FILE *fname){
    char TmpWord[10000];
    int count = 0;
    while(fscanf(fname,"%s", TmpWord) == 1){

        ++count;
    }
    rewind(fname);
    return count;
}
