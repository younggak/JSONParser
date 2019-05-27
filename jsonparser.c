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

int tokens(char *buffer);
void print(jsmntok_t*token, int num, char*buffer);
void tokenizer(jsmntok_t *token, char *buffer, int index);

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
	printf("***** ALL TOKENS *****\n");

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

	tokenizer(token, buffer, index);
	print(token, tokens(buffer), buffer);
}

<<<<<<< HEAD
void tokenizer(jsmntok_t *token, char *buffer, int len){
	int token_num = 0;
	int temp, end, size = 0;
	int obj = 0, arr = 0, str = 0, prm = 0;

	for(int i = 0; i < len; i++) {
		if(buffer[i] == '{') {
			token[token_num].type = OBJECT;
			token[token_num].start = i;
			temp = obj;
			for(end = i+1; ; end++){
				if(buffer[end] == '{')
=======
void tokenizer(jsmntok_t *token, char *buffer, int index){
	int token_num=0;
	int temp,end=0;
	int size=0;
	int obj=0,arr=0,str=0,prm=0;
	for(int i=0;i<index;i++){
		if(buffer[i]=='{'){
			token[token_num].type=OBJECT;
			token[token_num].start=i;
			temp=obj;
			for(end=i+1;;end++){
				if(buffer[end]=='{')
>>>>>>> 6a45cf28ced6dc11c65a5b7d75f1a3634fcc28c8
					temp++;
				if(buffer[end] == '}')
					temp--;
				if(temp < 0)
					break;
			}
<<<<<<< HEAD
			token[token_num].end = end;
			obj++;
			size=0;
			for(int j = i; j < end; j++) {
				if(buffer[j] == ':') size++;
			}
			token[token_num].size = size;
			token_num++; // why?
=======
			token[token_num].end=end;
			obj++;
			size=0;

			 for(int j=i; j<end;j++){
                         if(buffer[j]==':'){
                                size++;

                         }
                        }
                        token[token_num].size=size;
                        token_num++;
>>>>>>> 6a45cf28ced6dc11c65a5b7d75f1a3634fcc28c8
		}
		if(buffer[i] == '[') {
			token[token_num].type = ARRAY;
			token[token_num].start = i;
			temp = arr;
			for(end = i+1; ; end++) {
				if(buffer[end] == '[')
					temp++;
				if(buffer[end] == ']')
					temp--;
				if(temp < 0)
					break;
			}
			token[token_num].end = end;
			arr++;
<<<<<<< HEAD
			size = 1;
			for(int j = i; j < end; j++) {
				if(buffer[j] == ':') size++;
			}
			token[token_num].size = size;
			token_num++;
=======
			size=1;
			
			 for(int j=i; j<end;j++){
                         if(buffer[j]==':'){
                                size++;

                         }
                        }
                        token[token_num].size=size;
                        token_num++;


>>>>>>> 6a45cf28ced6dc11c65a5b7d75f1a3634fcc28c8
		}
		if(buffer[i] == '"') {
			token[token_num].type = STRING;
			token[token_num].start = i;
			for(end = i+1; buffer[end] != '"'; end++);
			token[token_num].end = end;
			i = end;
			str++;
<<<<<<< HEAD
			size = 0;
			if(buffer[end+1] == ':') size++;
			token[token_num].size = size;
=======
			size=0;

			 
                         if(buffer[end+1]==':'){
                                size++;

                         }
                        token[token_num].size=size;
>>>>>>> 6a45cf28ced6dc11c65a5b7d75f1a3634fcc28c8
			token_num++;
		}
		if((buffer[i] != '{') && (buffer[i] != '}') && (buffer[i] != '[') && (buffer[i] != ']') && (buffer[i] != '"') && (buffer[i] != '\0') && (buffer[i] != '\n') && (buffer[i] != ' ') && (buffer[i] != ':') && (buffer[i] != ',') && (buffer[i] != '\t')) {
			token[token_num].type = PRIMITIVE;
			token[token_num].start = i;
			for(end = i+1; buffer[end] == ' '; end++);
			token[token_num].end = end;
			i = end;
			prm++;
<<<<<<< HEAD
			size = 0;
			if(buffer[end+1] == ':') size++;
			token[token_num].size = size;
			token_num++;
=======
	                size=0;
                         
                         if(buffer[end+1]==':'){
                                size++;

                         }
                        
                        token[token_num].size=size;
                        token_num++;

>>>>>>> 6a45cf28ced6dc11c65a5b7d75f1a3634fcc28c8
		}
	}
};

void print(jsmntok_t *token,int num,char*buffer){
	int i,j;
	for(i = 0;i < num; i++){
		printf("[%2d] ", i);
		for(j = token[i].start; j < token[i].end+1; j++){
			if(buffer[j] == EOF) continue;
			printf("%c", buffer[j]);
		}
<<<<<<< HEAD
		printf(" (size=%d, %d~%d,", token[i].size, token[i].start, token[i].end);
		if(token[i].type == 1)
=======
		printf(" %d, %d~%d,",token[i].size,token[i].start,token[i].end);
		if(token[i].type==1)
>>>>>>> 6a45cf28ced6dc11c65a5b7d75f1a3634fcc28c8
			printf(" JSMN_OBJECT)\n");
		else if(token[i].type == 2)
			printf(" JSON_ARRAY)\n");
		else if(token[i].type == 3)
			printf(" JSON_STRING)\n");
		else if(token[i].type == 4)
			printf(" JSON_PRIMITIVE)\n");
	}
};

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
};

/* 1. so far we just counted the number of tokens (not complete, because numbers aren't read)
 * 2. we have to tokenize:
 *   1) by objects
 *   2) by arrays
 *   3) by strings
 *   according to the struct defined above (jsmntok_t)
 * 3. print out all tokens as shown in the slide
 */
