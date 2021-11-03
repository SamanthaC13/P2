/*Name: Samantha Craig
Class:CS4280
   Project P2
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "parser.h"
int main(int argc, char**argv)
{
	char* filename;                   
	int option=0;
	//This option if the user only entered the Program name without a file
	if(argc==1)
	{
		char character;
		FILE* temp;
		option=1;
		temp=fopen("temp.txt","w");
		//Takes in the Keyboard input and writes it to a file I call temp.txt
		while((character=getc(stdin))!=EOF)
		{
			fputc(character,temp);
		}
		//Writes the end of file to the temp file I created for the keyboard input
		fputc('\n',temp);
		fputc('\0',temp);
		fclose(temp);
		filename=malloc(sizeof(char)*10);
		strcpy(filename,"temp.txt");
        }
	else
	{
		//filename is set to the file the user entered with the implict extension .fs
		filename=argv[1];
		filename=strcat(filename,".fs");
		
	}
	parser(filename);
}
