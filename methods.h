Literal* createLiteral(float numData,char* stringData)
{
	Literal* node = malloc(sizeof(Literal));
	node->numData = numData;
	node->stringData = stringData;
	node->datatype = stringData!=NULL;
	return node;
}

char* stoi(int numData)
{
	int length = snprintf( NULL, 0, "%d", numData);
	char* str = malloc( length + 1 );
	snprintf( str, length + 1, "%d", numData);
	return str;	
}

char* getData(Literal* literal)
{
	if(literal->datatype==1)
		return literal->stringData;
	int length = snprintf( NULL, 0, "%g", literal->numData);
	char* str = malloc( length + 1 );
	snprintf( str, length + 1, "%g", literal->numData);
	return str;
}

DoNode* createDoNode(AsgnNode* asgnNode, IRNode* toExprVal, IRNode* byExprVal,StsNode* stsNode, IRNode* cnt)
{
	DoNode* node = malloc(sizeof(DoNode));
  	node->asgnNode = asgnNode;
  	node->toExprVal = toExprVal;
  	node->byExprVal = byExprVal;
  	node->stsNode = stsNode;
  	node->cnt = cnt;
  	return node;
}


DoWhNode* createDoWhNode(IRNode* exprVal, StsNode* stsNode)
{
	DoWhNode* node = malloc(sizeof(DoWhNode));
	node->exprVal = exprVal;
	node->stsNode = stsNode;
	return node;
}

IfNode* createIfNode(IRNode* exprVal, StsNode* stsNode,ElseNode* elseNode)
{
	IfNode* node = malloc(sizeof(IfNode));
	node->exprVal = exprVal;
	node->stsNode = stsNode;
	node->elseNode = elseNode;
	return node;
}

ElseNode* createElseNode(StsNode* stsNode,IfNode* ifNode)
{
	ElseNode* node = malloc(sizeof(ElseNode));
	node->stsNode = stsNode;
	node->ifNode = ifNode;
	return node;
}

StsNode* createStsNode()
{
	StsNode* node = malloc(sizeof(StsNode));
	node->ptr=0;
	return node;
}

StNode* createStNodeWithout()
{
	StNode* node = malloc(sizeof(StNode));
	node->doNode = NULL;
	node->doWhNode = NULL;
	node->ifNode = NULL;
	node->asgnNode = NULL;
	node->sayNode = NULL;
	return node;
}

StNode* createStNodeWithDo(DoNode* doNode)
{
	StNode* node = createStNodeWithout();
	node->doNode = doNode;
	return node;
}

StNode* createStNodeWithDoWh(DoWhNode* doWhNode)
{
	StNode* node = createStNodeWithout();
	node->doWhNode = doWhNode;
	return node;
}
StNode* createStNodeWithIf(IfNode* ifNode)
{
	StNode* node = createStNodeWithout();
	node->ifNode = ifNode;
	return node;
}
StNode* createStNodeWithAsgn(AsgnNode* asgnNode)
{
	StNode* node = createStNodeWithout();
	node->asgnNode = asgnNode;
	return node;
}
StNode* createStNodeWithSay(SayNode* sayNode)
{
	StNode* node = createStNodeWithout();
	node->sayNode = sayNode;
	return node;
}

void addToStsNode(StsNode* stsNode,StNode* stNode)
{
	stsNode->stNode[stsNode->ptr++]=stNode;
	return ;
}

AsgnNode* createAsgnNode(VarNode* var, IRNode* exprVal)
{
	var->declared = 1;
	AsgnNode* node = malloc(sizeof(AsgnNode));
	node->var = var;
	node->exprVal = exprVal;
	return node;
}

SayNode* createSayNode(IRNode* exprVal)
{
	SayNode* node = malloc(sizeof(SayNode));
	node->exprVal = exprVal;
	return node;
}

IRNode* createIRNode(char* operator,IRNode* operand1,IRNode* operand2,char* count,Literal* literal,VarNode* var)
{
	IRNode* node = malloc(sizeof(IRNode));
	node->operator=operator;
	node->operand1=operand1;
	node->operand2=operand2;
	node->count=count;
	node->literal=literal;
	node->var=var;
	return node;
}

VarNode* createVarNode(char* str,int byteOffset)
{
	VarNode* node = malloc(sizeof(VarNode));
	node->varname = strdup(str);
	node->nptr=NULL;
	node->byteOffset = byteOffset++;
	node->declared = 0;
	return node;
}