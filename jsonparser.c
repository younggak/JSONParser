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
void print(jsmntok_t*token,char*buffer);
void tokenizer(jsmntok_t *token,char *buffer, int index);

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
	
	jsmntok_t *token = (jsmntok_t *)malloc(sizeof(jsmntok_t) * 1024);
	for(int num=0;num<1024;num++){
		token[num].type=UNDEFINED;
	}

	tokenizer(token,buffer,index);
	print(token,buffer);
}

void tokenizer(jsmntok_t *token, char *buffer, int index){
	int token_num=0;
	int temp1,temp2,end=0;
	int obj=0,arr=0,str=0,prm=0;
	int size=0,check;
	for(int i=0;i<index;i++){
		if(buffer[i]=='{'){
			size=0;
			token[token_num].type=OBJECT;
			token[token_num].start=i;
			temp1=0;
			for(end=i+1;;end++){
				if(buffer[end]=='{')
					temp1++;
				if(buffer[end]=='}')
					temp1--;
				if(temp1<0)
					break;
			}
			token[token_num].end=end;
			//obj size checker
			check=0;
			size=0;
			for(temp2=token[token_num].start+1;temp2<=token[token_num].end;temp2++){
				if(buffer[temp2]==',')
					size++;
				if(buffer[temp2]=='{'){
					for(check=temp2;;check++){
						if(buffer[check]=='}')
							break;
					}
				temp2=check;
				}
                               if(buffer[temp2]=='['){
                                        for(check=temp2;;check++){
                                                if(buffer[check]==']')
                                                        break;
                                        }
                                temp2=check;
                                }
			}
			token[token_num].size=size+1;
			obj++;
			token_num++;
		}
		if(buffer[i]=='['){
			token[token_num].type=ARRAY;
			token[token_num].start=i;
			temp1=0;
			for(end=i+1;;end++){
				if(buffer[end]=='[')
					temp1++;
				if(buffer[end]==']')
					temp1--;
				if(temp1<0)
					break;
			}
			token[token_num].end=end;
			check=0;
			size=0;
                        for(temp2=token[token_num].start+1;temp2<=token[token_num].end;temp2++){
                                if(buffer[temp2]==',')
                                        size++;
                                if(buffer[temp2]=='['){
                                        for(check=temp2;;check++){
                                                if(buffer[check]==']')
                                                        break;
                                        }
					temp2=check;
				}
				 if(buffer[temp2]=='{'){
                                        for(check=temp2;;check++){
                                                if(buffer[check]=='}')
                                                        break;
                                        }
                                	temp2=check;
				}
                        }
                        token[token_num].size=size+1;
			arr++;
			token_num++;
		}
		if(buffer[i]=='"'){
			token[token_num].type=STRING;
			token[token_num].start=i;
			for(end=i+1;buffer[end]!='"';end++);
			token[token_num].end=end;
			i=end;
			//
			check=0;
			size=0;
			for(temp1=token[token_num].end+1;temp1<token[token_num].end+4;temp1++){
                                if(buffer[temp1]==':')
                                        size++;
                        }
			token[token_num].size=size;
			str++;
			token_num++;
		}
		if((buffer[i]!=EOF)&&(buffer[i]!='{')&&(buffer[i]!='}')&&(buffer[i]!='[')&&(buffer[i]!=']')&&(buffer[i]!='"')&&(buffer[i]!='\0')&&(buffer[i]!='\n')&&(buffer[i]!=' ')&&(buffer[i]!=':')&&(buffer[i]!=',')&&(buffer[i]!='\t')){
			token[token_num].type=PRIMITIVE;
			token[token_num].start=i;
			for(end=i+1;buffer[end]!=' ';end++);
			token[token_num].end=end;
			i=end;
			check=0;
                        size=0;
                        for(temp1=token[token_num].end+1;temp1<token[token_num].end+4;temp1++){
                                if(buffer[temp1]==':')
                                        size++;
                        }
                        token[token_num].size=size;
			prm++;
			token_num++;
		}
	}
};

void print(jsmntok_t *token,char*buffer){
	int i,j;
	for(i=0;token[i].type!=0;i++){
		printf("[%2d] ",i);
		for(j=token[i].start;j<token[i].end+1;j++){
			if(buffer[j]==EOF) continue;
			printf("%c",buffer[j]);
		}
		printf(" (size=%d, %d~%d,",token[i].size,token[i].start,token[i].end);
		if(token[i].type==1)
			printf(" JSMN_OBJECT)\n");
		else if(token[i].type==2)
			printf(" JSON_ARRAY)\n");
		else if(token[i].type==3)
			printf(" JSON_STRING)\n");
		else if(token[i].type==4)
			printf(" JSON_PRIMITIVE)\n");
	}
};

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
};

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
