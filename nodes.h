typedef struct Literal Literal;
typedef struct VarNode VarNode;
typedef struct DoNode DoNode;
typedef struct DoWhNode DoWhNode;
typedef struct IfNode IfNode;
typedef struct ElseNode ElseNode;
typedef struct StsNode StsNode;
typedef struct StNode StNode;
typedef struct AsgnNode AsgnNode;
typedef struct SayNode SayNode;
typedef struct IRNode IRNode;

struct Literal
{
	float numData;
	char* stringData;
	//0-number 1-string
	int datatype;
	IRNode* irNode;
};

struct  VarNode
{
	char* varname;
	int declared;
	VarNode *nptr;
	int byteOffset;
};

struct IRNode
{
	char* operator;
	IRNode* operand1;
	IRNode* operand2;
	char* count;
	//lit or variable
	Literal* literal;
	VarNode* var;
};

struct DoNode
{
	AsgnNode* asgnNode;
	IRNode* toExprVal;
	IRNode* byExprVal;
	StsNode* stsNode;
	IRNode* cnt;
};


struct DoWhNode
{
	IRNode* exprVal;
	StsNode* stsNode;
};

struct IfNode
{
	IRNode* exprVal;
	StsNode* stsNode;
	ElseNode* elseNode;
};

struct ElseNode
{
	StsNode* stsNode;
	IfNode* ifNode;
};

struct StsNode
{
	int ptr;
	StNode* stNode[20];
};

struct StNode
{
	DoNode* doNode;
	DoWhNode* doWhNode;
	IfNode* ifNode;
	AsgnNode* asgnNode;
	SayNode* sayNode;
};

struct AsgnNode
{
	VarNode* var;
	IRNode* exprVal;
};

struct SayNode
{
	IRNode* exprVal;
};

