struct node_t{
	char* nodeName;
	struct tokenType* startToken;
	int numOfTokens;
	struct node_t* children[5]; 
};
void parser(char*);
char* filterLine(char*);
char* getNewLine(char*);
void filterToken();
void error(int,char*);
void getNewToken();
void printTree(struct node_t*,int);
struct node_t* getNode();
void addTokenToNode(struct node_t*);
struct node_t* program();
struct node_t* vars();
struct node_t* block();
struct node_t* stats();
struct node_t* stat();
struct node_t* mstat();
struct node_t* in();
struct node_t* goTo();
struct node_t* label();
struct node_t* out();
struct node_t* expr();
struct node_t* N();
struct node_t* A();
struct node_t* M();
struct node_t* R();
struct node_t* RO();
struct node_t* assign();
struct node_t* loop();
struct node_t* If();
