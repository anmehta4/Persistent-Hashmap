#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

int contains(char* key) {
    FILE *fp;
    fp = fopen("database.txt", "a+"); // open the file
    if(fp==NULL){
        fprintf(stderr, " Error while opening file.\n");
    }

    bool found = false;
    char *line = NULL;
    int line_number = 0;
    size_t linecap = 0;
    ssize_t linelen;

    while ((linelen = getline(&line, &linecap, fp))>0) {
        line_number = line_number + 1;
        char* keyDB = strtok(line, ",");
        if (strcmp(key, keyDB)==0) {
            found = true;
            break;
        }
    }

    if(fclose(fp)!=0){ // close the opened file
        printf("Error while closing the file \n");
        exit(1);
    }

    if(found){
        return line_number;
    } else {
        return -1;
    }
}

void get(char* key) {
    FILE *fp;
    fp = fopen("database.txt", "a+"); // open the file
    if(fp==NULL){
        fprintf(stderr, " Error while opening file.\n");
    }

    bool found = false;
    char *line = NULL;
    int line_number = 0;
    size_t linecap = 0;
    ssize_t linelen;
    int line_number_DB = contains(key);
    rewind(fp);

    while ((linelen = getline(&line, &linecap, fp))>0) {
        line_number = line_number + 1;
        if (line_number==line_number_DB) {
            found = true;
            break;
        }
    }

    if(fclose(fp)!=0){ // close the opened file
        printf("Error while closing the file \n");
        exit(1);
    }

    if(found){
        printf("%s", line);
    } else {
	printf("%s %s\n", key, "not found");
    }
}

void displayAll() {

    FILE *fp;
    fp = fopen("database.txt", "a+"); // open the file
    if(fp==NULL){
        fprintf(stderr, " Error while opening file.\n");
    }

    char *line = NULL;
    int line_number = 0;
    size_t linecap = 0;
    ssize_t linelen;

    while ((linelen = getline(&line, &linecap, fp))>0) {
        line_number = line_number + 1;
        printf("%s", line);
    }

    if(fclose(fp)!=0){ // close the opened file
        printf("Error while closing the file \n");
        exit(1);
    }
}



void delete (char* key) {

    FILE *fp;
    fp = fopen("database.txt", "a+"); // open the file
    if(fp==NULL){
        fprintf(stderr, " Error while opening file.\n");
    }

    if(contains(key)==-1) {
        printf("%s %s\n", key, "not found");
	return;
    }
    rewind(fp);

    FILE *fptr2;
    char file1[] ="database.txt";
    char file2[] ="databasecopy.txt";
    int line_number = 0;
    int del = contains(key);
    char curr;

    fptr2 = fopen(file2, "w");
    curr = getc(fp);
    if(curr!=EOF) {
        line_number =1;
    }
    while(1) {
        if(del != line_number) {
            if(del==1 && line_number==2 && curr=='\n') {
                //do nothing
            } else {
                    putc(curr, fptr2);
            }
        }
        curr = getc(fp);
        if(curr =='\n') {
            line_number++;
        }
        if(curr == EOF) {
            break;
        }
    }
    fclose(fptr2);

    remove(file1);
    rename(file2,file1);
    if(fclose(fp)!=0){ // close the opened file
        printf("Error while closing the file \n");
        exit(1);
    }
}

void put(char* key, char* value) {
    FILE *fp;
    fp = fopen("database.txt", "a+"); // open the file
    if(fp==NULL){
        fprintf(stderr, " Error while opening file.\n");
    }

    char* entry = (char *) malloc(strlen(key) + 1 + strlen(value));
    strcpy(entry, key);
    strcat(entry, ",");
    strcat(entry, value);

    if(contains(key)==-1) {
        fprintf(fp, "%s\n", entry);
    } else {
        delete(key);
        fp = fopen("database.txt", "a+"); // open the file
        if(fp==NULL) {
            fprintf(stderr, " Error while opening file.\n");
        }
        fprintf(fp, "%s\n", entry);
    }
    free(entry);
    if(fclose(fp)!=0){ // close the opened file
        printf("Error while closing the file \n");
        exit(1);
    }
}

void clear() {
    char file1[] ="database.txt";
    remove(file1);

    FILE *fp;
    fp = fopen("database.txt", "a+"); // open the file
    if(fp==NULL){
        fprintf(stderr, " Error while opening file.\n");
    }
}

int main(int argc, char* argv[]) {

    if(argc < 1) {
        printf("Usage: ./kv function_name, parameters if needed \n");
        exit(1);
    }

    FILE *fp;
    fp = fopen("database.txt", "a+"); // open the file
    if(fp==NULL){
        fprintf(stderr, " Error while opening file.\n");
    }

    for(int i=1; i<argc; i++) {
        char* input = argv[i];
        char* func = strtok(input, ",");
        char* key =  strtok(NULL, ",");
	char* keyPtr;
	int isValidKey = 1;
	for (keyPtr = key;keyPtr!= NULL && *keyPtr != '\0'; keyPtr++){
	    if(isdigit(*keyPtr)==0) {
		 isValidKey = 0;
	    }
	}
	if(isValidKey == 0){
	    printf("bad command\n");
	    continue;
    	}	    

        char* value = strtok(NULL, ",");

        if (strcmp(func,"p")==0 && key!=NULL && value!=NULL) {
            put(key, value);
        } else if (strcmp(func,"g")==0 && key!=NULL && value==NULL) {
	    get(key);
        } else if (strcmp(func,"d")==0 && key!=NULL && value==NULL) {
            delete(key);
        } else if (strcmp(func,"a")==0 && key==NULL && value==NULL) {
            displayAll();
        } else if (strcmp(func,"c")==0 && key==NULL && value==NULL) {
            clear();
        } else {
	    // do nothing
            printf("bad command\n");
        }
    }

    if(fclose(fp)!=0){ // close the opened file
        printf("Error while closing the file \n");
        exit(1);
    }

    return 0;
}

