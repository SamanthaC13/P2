/*Name: Samantha Craig
 * Project 2
 * Class:CS4280*/
#include<stdio.h>
#include<stdlib.h>
#include"token.h"
#include"scanner.h"
#include"parser.h"
int ignore;
int startChar;
int lineNum;
FILE* input;
int len=124;
char* line;
struct tokenType currentToken;
void parser(char* filename)
{
	//file is opened for reading 
	input=fopen(filename,"r");
	if(input==NULL)
	{
		//Error is printed if input file is  unable to be opened
		fprintf(stderr,"Error opening input file in TestScanner");
		exit(1);
	}
	line=malloc(len*sizeof(char));
	//read first line from file
	line=fgets(line,len,input);
	//send line to filter to filter out comments
	line=filterLine(line);
	lineNum=1;
	startChar=0;
	currentToken=scanner(line,lineNum,startChar);
	filterToken(currentToken);
	program();
	//if it passes the parser send it to print the tree
	if(currentToken.tokenID=EOFTK)
	{
		printf("\nParsed through successfully\n");
	}
	else
	{
		printf("\nParsed through but no EOF token\n");
	}
}
void program()
{
	vars();
 	if(strcmp(currentToken.tokenInstance,"program")==0)
 	{
		getNewToken();
		block();
	}
	else
	{
		error(0,"program");
	}
}
void vars()
{
	return;
}
void block()
{
	if(strcmp(currentToken.tokenInstance,"start")==0)
	{
		getNewToken();
		vars();
		stats();
		if(strcmp(currentToken.tokenInstance,"stop")==0)
		{
			getNewToken();
			return;
		}
		else
		{	
			error(2,"stop");
		}
	}
	else
	{
		error(2,"start");
	}
}
void stats()
{
	stat();
	mstat();
	return;
}
void stat()
{
	in();
	if(currentToken.tokenID==SMCLNTK)
	{
		getNewToken();
		return;
	}
	else
	{
		error(5,"Semi-Colon");
	}
}
void mstat()
{
	return;
}
void in()
{
	if(strcmp(currentToken.tokenInstance,"listen")==0)
	{
		getNewToken();
		if(currentToken.tokenID==IDTK)
		{
			getNewToken();
			return;
		}
		else
		{
			error(6,"Identifier");
		}
	}
	else
	{
		error(6,"listen");
	}
}
void error(int function,char* expected)
{
	char *functionNames[10]={"Program","vars","block","stats","mstats","stat","in"};
	printf("\nPARSER ERROR:In Node %s expected to get %s and got %s at line Number:%d Character Count: %d\n",functionNames[function],expected,currentToken.tokenInstance,lineNum,startChar); 
	exit(1);
}
void getNewToken()
{
	startChar+=currentToken.charCount;
	currentToken=scanner(line,lineNum,startChar);
	filterToken(currentToken);	
}
void filterToken(struct tokenType token)
{
	if(token.tokenID<0)
	{
		printf("\nSCANNER ERROR: LINE NUMBER-%d",token.lineCount);
		exit(0);
	}
	if(currentToken.tokenID==WSTK)
	{
		getNewToken();
	}
	if(token.tokenID==EOLTK)
	{
		line=getNewLine(line);
		currentToken=scanner(line,lineNum,startChar);
	}
}
char* getNewLine(char* line)
{
	char* lastLine=line;
	line=fgets(line,len,input);
	if(feof(input))
	{//if the current line was the last line then the lastLine variable will not be the line again and the next token gotten will be the EOFTK
		line=lastLine;
		startChar=startChar+1;
	}
	else
	{
		line=filterLine(line);
		lineNum=lineNum++;
		startChar=0;
	}
	return line;
}
char* filterLine(char* line)
//This function takes the line from the file and filters it into a newLine without comments
{
	//Count for the orginal line
	int lineCharCount=0;
	//count for the new created line
	int newLineCharCount=0;
	//flag for if there is a comment in this line
	int commentFlag=0;
	//c is the currrnet character from the orginal line
	char c=line[lineCharCount];
	char* newLine=malloc(sizeof(char)*124);
	while(c!='\n')
	//want to read the orginal line til it ends with the \n character
	{
		if(c=='&')
		//If we get one apersand then we look to see if the next character is an apersand
		{
			lineCharCount++;
			if(line[lineCharCount]=='&')
			//if we found a second apersand then we need to figure out if it is the beginning pair or the end pair of the comment for this I used the global variable ignore
			{
				if(ignore==0)
				//if ignore is 0 then this is the beginning pair and sets ignore to 1 and commentFlag to 1
				{
					ignore=1;
					commentFlag=1;
				}
				else if(ignore==1)
				//if ignore if already 1 then this is the end pair and ignore is set back to one but the comment Flag is set to 1 so the apersand is not put into newLine 
				{
					ignore=0;
					commentFlag=1;
				}
			}
			else
			//if we got one apersand and then their was not a second one then the appersand will be added to the newLine 
			{
				lineCharCount--;
			}	
		}
		if(ignore==0&&commentFlag==0)
		//if both the ignore and comment flags are 0 then the character c can be added to newLine and the new Line count is incremented
		{
			newLine[newLineCharCount]=c;
			newLineCharCount++;	

		}
		//the orginal line count is increased on every iteration
		lineCharCount++;
		c=line[lineCharCount];
		if(commentFlag==1&&ignore==0)
		//if the comment flag and ignore are different then comment flag is set to zero this is set so the first apersand or the last apersand are not added to newLine
		{
			commentFlag=0;
		}
	}
	//a new line character is added to the end of newLine 
	newLine[newLineCharCount]='\n';	
	//the newLine is returned 
	return newLine;
}
