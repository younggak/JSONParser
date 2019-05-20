#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef enum {
	UNDEFINED = 0,
	OBJECT = 1,
	ARRAY = 2,
	STRING = 3,
	PRIMITIVE = 4
} jsmntype_t;

typedef struct {
	jsmntype_t type;
	int start;
	int end;
	int size;
} jsmntok_t;

int main(int argc, char **argv) {
	char file_name[strlen(argv[1])];
	strcpy(file_name, argv[1]);

	int dot_index = 0;
	for(int i = 0; i < strlen(file_name); i++)
		if(file_name[i] == '.') 
			dot_index = i;
	char file_type[strlen(file_name)-dot_index];
	strcpy(file_type, &file_name[dot_index+1]);	
	if(strcmp(file_type, "json") != 0)
		exit(0);

	FILE *fp;
	fp = fopen(file_name, "r");
	if(fp == NULL) {
		printf("File doesn't exist!\n");
	}
	printf("File is openned.\nFile contains the following: \n");

	char c;
	char *buffer = (char*)malloc(sizeof(char) * 1024);
	int index = 0;
	do {
		c = fgetc(fp);
		buffer[index] =  c;
		index++;	
	} while(c != EOF);
	fclose(fp);
	
	jsmntok_t *token = (jsmntok_t *)malloc(sizeof(jsmntok_t) * tokens(buffer));

}

//jsmntok_t *get_tokens(char *buffer) {
int tokens(char *buffer) {
	int count_tokens = 0;

	for(int i = 0; i < strlen(buffer); i++) // count string tokens
		if(buffer[i] == '"') count_tokens++;
	count_tokens /= 2;
	
	for(int i = 0; i < strlen(buffer); i++) // count object, array tokens
		if(buffer[i] == '{' || buffer[i] == '[') count_tokens++;
	
	for(int i = 0; i < strlen(buffer); i++) // count numbers (not working)
		if(isdigit(buffer[i]) && !isdigit(buffer[i+1])) count_tokens++;

	return count_tokens;


//	jsmntok_t *tokens = (jsmntok_t *)malloc(sizeof(jsmntok_t) * 128);
//	
//	if(buffer[0] = '{' && buffer[strlen(buffer-1)] == '}') {
//		tokens[0]->type = OBJECT;
//		tokens[0]->start = 0;
//		tokens[0]->end = strlen(buffer)-1;
//	}
	
/* 1. so far we just counted the number of tokens (not complete, because numbers aren't read)
 * 2. we have to tokenize:
 *   1) by objects
 *   2) by arrays
 *   3) by strings
 *   according to the struct defined above (jsmntok_t)
 * 3. print out all tokens as shown in the slide
 */
}
