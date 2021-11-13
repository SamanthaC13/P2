/*Name: Samantha Craig
 * Project 2
 * Class:CS4280*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
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
	struct node_t* root;
	root=program();
	//if it passes the parser send it to print the tree
	if(currentToken.tokenID=EOFTK)
	{
		printf("\nParsed through successfully\n");
		printTree(root,0);
	}
	else
	{
		printf("\nParsed through but no EOF token\n");
	}
	printf("\n");
}
void  printTree(struct node_t* p, int level)
{
	int i;
	printf("\n%*c%d:%s-%d",level*2,' ',level,p->nodeName,p->numOfTokens);
	for(i=0;i<5;i++)
	{
		if(p->children[i]!=NULL)
		{
			printTree(p->children[i],level+1);
		}
	}
	return;
}
struct node_t* getNode(char* functionName)
{
	struct node_t* p= malloc(sizeof(struct node_t));
	p->nodeName=malloc(sizeof(char)*20);
	strcpy(p->nodeName,functionName);
	return p;
}
void addTokenToNode(struct node_t* p)
{
	int i=p->numOfTokens;
/*	p->tokenList[i] = malloc(sizeof(struct tokenType));
	p->tokenList[i]->tokenID=currentToken.tokenID;
	p->tokenList[i]->lineCount=currentToken.lineCount;
	p->tokenList[i]->charCount=currentToken.charCount;
	strcpy(p->tokenList[i]->tokenInstance,currentToken.tokenInstance);*/
	p->numOfTokens+=1;	
}
struct node_t* program()
{
	struct node_t* p=getNode("program");
	p->children[0]=vars();
 	if(strcmp(currentToken.tokenInstance,"program")==0)
 	{
		getNewToken();
		p->children[1]=block();
		return p;
	}
	else
	{
		error(0,"program");
	}
}
struct node_t* vars()
{
	if(strcmp(currentToken.tokenInstance,"declare")==0)
	{
		struct node_t* p=getNode("vars");
		addTokenToNode(p);
		getNewToken();
		if(currentToken.tokenID==IDTK)
		{
			addTokenToNode(p);
			getNewToken();
			if(currentToken.tokenID==EQTK)
			{
				addTokenToNode(p);
				getNewToken();
				if(currentToken.tokenID==NUMTK)
				{
					addTokenToNode(p);
					getNewToken();
					if(currentToken.tokenID==SMCLNTK)
					{
						addTokenToNode(p);
						getNewToken();
						p->children[0]=vars();
						return p;
					}
					else
					{
						error(1,"Semi-Colon");
					}	
				}
				else
				{
					error(1,"Number");
				}
			}
			else
			{
				error(1,"Equal Sign");
			}
		}
		else
		{
			error(1,"Identifier");
		}
	}
	else
	{
		return NULL;
	}
}
struct node_t* block()
{
	if(strcmp(currentToken.tokenInstance,"start")==0)
	{
		struct node_t* p=getNode("block");
		addTokenToNode(p);
		getNewToken();
		p->children[0]=vars();
		p->children[1]=stats();
		if(strcmp(currentToken.tokenInstance,"stop")==0)
		{
			addTokenToNode(p);
			getNewToken();
			return p;
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
struct node_t* stats()
{
	struct node_t* p=getNode("stats");
	p->children[0]=stat();
	p->children[1]=mstat();
	return p;
}
struct node_t* stat()
{
	struct node_t* p=getNode("stat");
	if(strcmp(currentToken.tokenInstance,"listen")==0)
	{	
		p->children[0]=in();
		if(currentToken.tokenID==SMCLNTK)
		{
			addTokenToNode(p);
			getNewToken();
			return p;
		}
		else
		{
			error(5,"Semi-Colon");
		}
	}
	else if(strcmp(currentToken.tokenInstance,"label")==0)
	{
		p->children[0]=label();
		if(currentToken.tokenID==SMCLNTK)
		{
			addTokenToNode(p);
			getNewToken();
			return p;
		}
		else
		{
			error(5,"Semi-Colon");
		}
	}
	else if(strcmp(currentToken.tokenInstance,"jump")==0)
	{
		p->children[0]=goTo();
		if(currentToken.tokenID==SMCLNTK)
		{
			addTokenToNode(p);
			getNewToken();
			return p;
		}
		else
		{
			error(5,"Semi-Colon");
		}
	}
	else if(strcmp(currentToken.tokenInstance,"start")==0)
	{
		p->children[0]=block();
		return p;
	}
	else if(strcmp(currentToken.tokenInstance,"talk")==0)
	{
		p->children[0]=out();
		if(currentToken.tokenID==SMCLNTK)
		{
			addTokenToNode(p);
			getNewToken();
			return p;
		}
		else
		{
			error(5,"Semi-Colon");
		}
	}
	else if(strcmp(currentToken.tokenInstance,"assign")==0)
	{
		p->children[0]=assign();
		if(currentToken.tokenID==SMCLNTK)
		{
			addTokenToNode(p);
			getNewToken();
			return p;
		}
		else
		{
			error(5,"Semi-Colon");
		}
	}
	else if(strcmp(currentToken.tokenInstance,"while")==0)
	{
		p->children[0]=loop();
		if(currentToken.tokenID==SMCLNTK)
		{	
			addTokenToNode(p);
			getNewToken();
			return p;
		}
		else
		{
			error(5,"Semi-Colon");
		}
	}
	else if(strcmp(currentToken.tokenInstance,"if")==0)
	{
		p->children[0]=If();
		if(currentToken.tokenID==SMCLNTK)
		{
			addTokenToNode(p);
			getNewToken();
			return p;
		}
		else
		{
			error(5,"Semi-Colon");
		}
	}
	else
	{
		error(5,"Stat Option");
	}
}
struct node_t* mstat()
{	
	if(strcmp(currentToken.tokenInstance,"listen")==0||strcmp(currentToken.tokenInstance,"label")==0||strcmp(currentToken.tokenInstance,"jump")==0||strcmp(currentToken.tokenInstance,"start")==0||strcmp(currentToken.tokenInstance,"talk")==0||strcmp(currentToken.tokenInstance,"while")==0||strcmp(currentToken.tokenInstance,"if")==0||strcmp(currentToken.tokenInstance,"assign")==0)
	{
		struct node_t* p=getNode("mstat");
		p->children[0]=stat();
		p->children[1]=mstat();
		return p;
	}
	else
	{
		return NULL;	
	}
}
struct node_t* in()
{
	struct node_t* p=getNode("in");
	if(strcmp(currentToken.tokenInstance,"listen")==0)
	{
		addTokenToNode(p);	
		getNewToken();
		if(currentToken.tokenID==IDTK)
		{
			addTokenToNode(p);
			getNewToken();
			return p;
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
struct node_t* label()
{
	struct node_t* p=getNode("label");
	addTokenToNode(p);	
	getNewToken();
	if(currentToken.tokenID==IDTK)
	{
		addTokenToNode(p);
		getNewToken();
		return p;
	}
	else
	{
		error(7,"Identifier");
	}
}
struct node_t* goTo()
{
	struct node_t* p=getNode("goTo");
	addTokenToNode(p);
	getNewToken();
	if(currentToken.tokenID==IDTK)
	{
		addTokenToNode(p);
		getNewToken();
		return p;
	}
	else
	{
		error(8,"Identifier");
	}
}
struct node_t* out()
{
	struct node_t* p=getNode("out");
	addTokenToNode(p);
	getNewToken();
	p->children[0]=expr();
	return p;
}
struct node_t* expr()
{
	struct node_t* p=getNode("expr");
	p->children[0]=N();
	if(currentToken.tokenID==PSTK)
	{
		addTokenToNode(p);
		getNewToken();
		p->children[1]=expr();
		return p;
	}
	return p;
}
struct node_t* N()
{
	struct node_t*p=getNode("N");
	p->children[0]=A();
	if(currentToken.tokenID==SLTK)
	{
		addTokenToNode(p);
		getNewToken();
		p->children[1]=N();
		return p;
	}
	else if(currentToken.tokenID==ASKTK)
	{
		addTokenToNode(p);
		getNewToken();
		p->children[1]=N();
		return p;
	}
	return p;
}
struct node_t* A()
{
	struct node_t* p=getNode("A");
	p->children[0]=M();
	if(currentToken.tokenID==MINTK)
	{
		addTokenToNode(p);
		getNewToken();
		p->children[1]=A();
		return p;
	}
	return p;
}
struct node_t* M()
{
	struct node_t* p=getNode("M");
	if(currentToken.tokenID==PERTK)
	{
		addTokenToNode(p);
		getNewToken();
		p->children[0]=M();
	}
	else
	{
		p->children[0]=R();
	}
	return p;
}
struct node_t* R()
{
	struct node_t* p=getNode("R");
	if(currentToken.tokenID==LPTK)
	{
		addTokenToNode(p);
		getNewToken();
		p->children[0]=expr();
		if(currentToken.tokenID==RPTK)
		{
			addTokenToNode(p);
			getNewToken();
			return p;
		}
		else
		{
			error(14,"Right Parentheses");
		}
	}
	if(currentToken.tokenID==IDTK)
	{
		addTokenToNode(p);
		getNewToken();
		return p;
	}
	if(currentToken.tokenID==NUMTK)
	{
		addTokenToNode(p);
		getNewToken();
		return p;
	}
}
struct node_t* assign()
{
	struct node_t* p=getNode("assign");
	addTokenToNode(p);
	getNewToken();
	if(currentToken.tokenID==IDTK)
	{
		addTokenToNode(p);
		getNewToken();
		if(currentToken.tokenID==EQTK)
		{
			addTokenToNode(p);
			getNewToken();
			p->children[0]=expr();
			return p;
		}
		else
		{
			error(15,"Eqaul Sign");
		}
	}
	else
	{
		error(15,"Identiifier");
	}
}
struct node_t* RO()
{
	struct node_t* p=getNode("RO");
	if(currentToken.tokenID==GTTK)
	{
		addTokenToNode(p);
		getNewToken();
		return p;
	}
	else if(currentToken.tokenID==LTTK)
	{
		addTokenToNode(p);
		getNewToken();
		return p;
	}
	else if(currentToken.tokenID==DBEQTK)
	{
		addTokenToNode(p);
		getNewToken();
		return p;
	}
	else if(currentToken.tokenID==PCTTK)
	{
		addTokenToNode(p);
		getNewToken();
		return p;
	}
	else if(currentToken.tokenID==LBCTK)
	{
		addTokenToNode(p);
		getNewToken();
		if(currentToken.tokenID==DBEQTK)
		{
			addTokenToNode(p);
			getNewToken();
			if(currentToken.tokenID==RBCTK)
			{
				addTokenToNode(p);
				getNewToken();
				return p;
			}
			else
			{
				error(16,"RightBrace");
			}
		}
		else
		{
			error(16,"Double Equal");
		}
	}
	else
	{
		error(16,"Relational Operators");
	}	
}
struct node_t* loop()
{
	struct node_t* p=getNode("loop");
	addTokenToNode(p);
	getNewToken();
	if(currentToken.tokenID==LBKTK)
	{
		addTokenToNode(p);
		getNewToken();
		p->children[0]=expr();
		p->children[1]=RO();
		p->children[2]=expr();
		if(currentToken.tokenID=RBKTK)
		{
			addTokenToNode(p);
			getNewToken();
			p->children[3]=stat();
			return p;
		}
		else
		{
			error(17,"Right Bracket");
		}
			
	}
	else
	{
		error(17,"Left Bracket");
	}

}
struct node_t* If()
{
	struct node_t* p=getNode("If");
	addTokenToNode(p);
	getNewToken();
	if(currentToken.tokenID==LBKTK)
	{
		addTokenToNode(p);
		getNewToken();
		p->children[0]=expr();
		p->children[1]=RO();
		p->children[2]=expr();
		if(currentToken.tokenID==RBKTK)
		{
			addTokenToNode(p);
			getNewToken();
			if(strcmp(currentToken.tokenInstance,"then")==0)
			{
				addTokenToNode(p);
				getNewToken();
				p->children[3]=stat();
				if(strcmp(currentToken.tokenInstance,"else")==0)
				{
					addTokenToNode(p);
					getNewToken();
					p->children[4]=stat();
					return p;
				}
				else
				{
					return p;
				}
			}
			else
			{
				error(18,"then");
			}
		}
		else
		{
			error(18,"Right Bracket");
		}
	}
	else
	{
		error(18,"Left Bracket");
	}
}
void error(int function,char* expected)
{
	char *functionNames[25]={"Program","vars","block","stats","mstats","stat","in","label","goTo","out","expr","N","A","M","R","assign","RO","loop","if"};
	printf("\nPARSER ERROR:In Node %s expected to get %s and got %s at line Number:%d Character Count: %d\n",functionNames[function],expected,currentToken.tokenInstance,lineNum,startChar); 
	exit(1);
}
void getNewToken()
{
	startChar+=currentToken.charCount;
	currentToken=scanner(line,lineNum,startChar);
	filterToken(currentToken);	
}
void filterToken()
{
	if(currentToken.tokenID<0)
	{
		printf("\nSCANNER ERROR: LINE NUMBER-%d",currentToken.lineCount);
		exit(0);
	}
	if(currentToken.tokenID==WSTK)
	{
		getNewToken();
	}
	if(currentToken.tokenID==EOLTK)
	{
		line=getNewLine(line);
		currentToken=scanner(line,lineNum,startChar);
		filterToken(currentToken);
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
		lineNum=lineNum+1;
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
