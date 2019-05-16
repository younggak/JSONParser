#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

	int c;
	do {
		c = fgetc(fp);
		printf("%c", c);
	} while(c != EOF);
	fclose(fp);
}
