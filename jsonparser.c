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
void readTopping(jsmntok_t *token,int price, int cal, int num, char *buffer);
void readDressing(jsmntok_t *token,int price, int cal, int num, char *buffer);

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
	else
	printf("File is openned.\nFile contains the following: \n");

	char c;
	char *buffer = (char*)malloc(sizeof(char) * 1024);
	int index = 0;
	do {
		c = fgetc(fp);
		buffer[index] =  c;
		index++;	
	} while(c != EOF);

	jsmntok_t *token = (jsmntok_t *)malloc(sizeof(jsmntok_t) * 1024);
	for(int num=0;num<1024;num++){
		token[num].type=UNDEFINED;
	}
	tokenizer(token,buffer,index);
	print(token,buffer);

	printf("\nsalad - price: 2000 / kcal: 160\n");
        printf("------------------------toppings-------------------------\n");	
	printf("1. salmon                2. tuna               3. avocado\n");
        printf("4. chicken tender        4. mango              5. kiwi\n");
	printf("6. tortilla              7. croutons           8. tofu\n");
	printf("10. egg                  0. NO\n");

	printf("------------------------dressings------------------------\n");
        printf("1. oriental              2. pineapple         \n");
        printf("3. black sesame          4. balsamic         \n");
	printf("5. mayonnaise\n");

	int num=0, num1=0, price=0, cal=0;
	char res, res2;

	printf("--------------------------------------------------------\n");
        printf("Do you want to add toppings? (y/n): \n");
        scanf("%c",&res);

        if(res=='y'){
                do{
                        printf("Choose all toppings you wish (0~10): \n");
                        scanf("%d",&num);
                        readTopping(token,price, cal, num, buffer);
                }while(num!=0);
        }

	printf("--------------------------------------------------------\n");
        printf("Do you want to add dressings? (y/n): \n");
        scanf("%c",&res2);

        if(res2=='y'){
                
                printf("Choose one dressing (1~5): \n");
                scanf("%d",&num1);
                readDressing(token,price, cal, num1, buffer);
               
        }

	price=price+2000;
	cal=cal+2000;

	printf("Total price: %d won\n", price);
	printf("Total calories: %d kcal\n", cal);

	printf("Enjoy your meal :)");

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
			token[token_num].end=end+1;
			//obj size checker
			check=0;
			size=0;
			for(temp2=token[token_num].start+1;temp2<=token[token_num].end-1;temp2++){
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
			token[token_num].end=end+1;
			check=0;
			size=0;
                        for(temp2=token[token_num].start+1;temp2<=token[token_num].end-1;temp2++){
                                if(buffer[temp2]==','){
					size++;
				}
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
			token[token_num].start=i+1;
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
			token[token_num].end=end-1;
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
		for(j=token[i].start;j<token[i].end;j++){
			if(buffer[j]==EOF) continue;
			printf("%c",buffer[j]);
		}
		printf(" (size=%d, %d~%d,",token[i].size,token[i].start,token[i].end);
		if(token[i].type==1)
			printf(" OBJECT)\n");
		else if(token[i].type==2)
			printf(" ARRAY)\n");
		else if(token[i].type==3)
			printf(" STRING)\n");
		else if(token[i].type==4)
			printf(" PRIMITIVE)\n");
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


void readTopping(jsmntok_t *token,int price, int cal, int num,char*buffer){
	int i=0;
	
	switch(num){
		case 1: i=5; break;
		case 2:	i=12; break;
		case 3: i=19; break;
		case 4: i=26; break;
		case 5: i=33; break;
		case 6: i=40; break;
		case 7: i=47; break;
		case 8: i=54; break;
		case 9: i=61; break;
		case 10: i=68; break;
	 	case 0: printf("You choose all topping.\n"); break;
		
		default: printf("Input again\n"); break;
	}

	char* p="";
	char* c="";

	for(int j=token[i+2].start;j<token[i+2].end;j++){
                if(buffer[j]==EOF) continue;
                p=p+buffer[j];
        }
	for(int j=token[i+2].start;j<token[i+2].end;j++){
                if(buffer[j]==EOF) continue;
                c=c+buffer[j];
        }

	price=price+atoi(p);
	cal=cal+atoi(c);


}

void readDressing(jsmntok_t *token,int price, int cal, int num,char*buffer){
        int i=0;

        switch(num){
                case 1: i=77; break;
                case 2: i=84; break;
                case 3: i=91; break;
                case 4: i=98; break;
                case 5: i=105; break;

                default: printf("Input again"); break;
        }

        char* p="";
        char* c="";

        for(int j=token[i+2].start;j<token[i+2].end;j++){
                if(buffer[j]==EOF) continue;
                p=p+buffer[j];
        }
        for(int j=token[i+2].start;j<token[i+2].end;j++){
                if(buffer[j]==EOF) continue;
                c=c+buffer[j];
        }

        price=price+atoi(p);
        cal=cal+atoi(c);


}


			
		
