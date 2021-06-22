int generateCodeForIR(IRNode* irNode,int label, FILE* fp);
int generateCodeForDoWh(DoWhNode* doWhNode,int label, FILE* fp);
int generateCodeForIf(IfNode* ifNode, int label, FILE* fp);
int generateCodeForElse(ElseNode* elseNode, int label, FILE* fp);
int generateCodeForSts(StsNode* stsNode,int label, FILE* fp);
int generateCodeForSt(StNode* stNode,int label, FILE* fp);
int generateCodeForAsgn(AsgnNode* asgnNode, int label, FILE* fp);
int generateCodeForSay(SayNode* sayNode, int label, FILE* fp);

int generateCodeForIR(IRNode* irNode,int label, FILE* fp)
{
	if(irNode==NULL)
	{
		return label;
	}
	if(irNode->operator==NULL)
	{
		if(irNode->var==NULL)
		{
			fprintf(fp, "mov BX, %d\n",(int)irNode->literal->numData);
		}
		else
		{
			fprintf(fp, "mov BX, [ss:%d]\n",irNode->var->byteOffset*2);
		}
		fprintf(fp, "push BX\n");
		return label;
	}
	else
	{
		int isCompare=0;
		label = generateCodeForIR(irNode->operand2, label, fp);
		label = generateCodeForIR(irNode->operand1, label, fp);
		int labelOffset = label;
		fprintf(fp, "pop BX\n");
		fprintf(fp, "mov AX,BX\n");
		fprintf(fp, "pop BX\n");
		switch(irNode->operator[0])
		{
			case '+':
			fprintf(fp, "add ");
			break;
			case '-':
			fprintf(fp, "sub ");
			break;
			case '*':
			fprintf(fp, "mul ");
			break;
			case '/':
			fprintf(fp, "div ");
			break;
			case '%':
			fprintf(fp, "div ");
			break;
			default:
			fprintf(fp, "cmp ");
			label+=3;
			isCompare = 1;
			break;
		}
		fprintf(fp, "AX, BX\n");
		//TODO create procedure for pushing 1 
		switch(irNode->operator[0])
		{
			case '>':
				if(sizeof(irNode->operator)/sizeof(char*)==1)
				{
					//>
					fprintf(fp, "jg L%d\n",labelOffset+1);
					fprintf(fp, "jng L%d\n",labelOffset+2);
				} 
				else
				{
					//>=
					fprintf(fp, "jge L%d\n",labelOffset+1);
					fprintf(fp, "jnge L%d\n",labelOffset+2);
				}
			break;
			case '<':
				if(sizeof(irNode->operator)/sizeof(char*)==1)
				{
					//<
					fprintf(fp, "jl L%d\n",labelOffset+1);
					fprintf(fp, "jnl L%d\n",labelOffset+2);
				} 
				else
				{
					//<=
					fprintf(fp, "jle L%d\n",labelOffset+1);
					fprintf(fp, "jnle L%d\n",labelOffset+2);
				} 
			break;
			case '=':
			//==
			fprintf(fp, "je L%d\n",labelOffset+1);
			fprintf(fp, "jne L%d\n",labelOffset+2);
			break;
			default:
				fprintf(fp, "mov BX, AX\n");
				fprintf(fp, "push BX\n");
				break;
			break;
		}
		if(isCompare)
		{
			//for push 1 or 0 
			fprintf(fp, "L%d:\n",labelOffset+1);
			fprintf(fp, "mov BX,1d\n");
			fprintf(fp, "push BX\n");
			fprintf(fp, "jmp L%d\n",labelOffset+3);
			fprintf(fp, "L%d:\n",labelOffset+2);
			fprintf(fp, "mov BX,0d\n");
			fprintf(fp, "push BX\n");
			fprintf(fp, "L%d:\n",labelOffset+3);
		}
		return label;
	}
	return label;	
}

int generateCodeForDo(DoNode* doNode, int label, FILE* fp)
{
	int labelOffset = label;
	label+=2;
	AsgnNode* asgnNode = doNode->asgnNode;
	label = generateCodeForAsgn(asgnNode, label, fp);

	fprintf(fp, "L%d:\n", labelOffset+1);

	IRNode* toIrNode = doNode->toExprVal;
	label = generateCodeForIR(toIrNode, label, fp);
	fprintf(fp, "pop BX\n");
	fprintf(fp, "cmp BX,word ptr [ss:%d]\n",asgnNode->var->byteOffset*2);
	fprintf(fp, "jl L%d\n", labelOffset+2);
	label = generateCodeForSts(doNode->stsNode,label, fp);

	IRNode* byIrNode = doNode->byExprVal;
	label = generateCodeForIR(byIrNode, label, fp);
	fprintf(fp, "pop BX\n");
	fprintf(fp, "add word ptr [ss:%d],BX\n",asgnNode->var->byteOffset*2);
	fprintf(fp, "jmp L%d\n", labelOffset+1);
	fprintf(fp, "L%d:\n",labelOffset+2);
   	return label;
}

/*
while loop

L1:
if(E<0)
	jmp L2
	
	perfrom Sts
	jmp L1

L2:

*/
int generateCodeForDoWh(DoWhNode* doWhNode,int label, FILE* fp)
{
	IRNode* irNode = doWhNode->exprVal;
	int labelOffset = label;
	label+=2;
	fprintf(fp, "L%d:\n", labelOffset+1);
	label = generateCodeForIR(irNode, label, fp);
	fprintf(fp, "pop BX\n"); //TODO: if (E<0)
	fprintf(fp, "cmp BX,0d\n");
	fprintf(fp, "jle L%d\n", labelOffset+2);
	label = generateCodeForSts(doWhNode->stsNode,label, fp);
	fprintf(fp, "jmp L%d\n", labelOffset+1);
	fprintf(fp, "L%d:\n",labelOffset+2);
   	return label;
}

/*
if(E>0) 
	jmp L1
//else part
a=0
jmp L2
L1
	a=1
L2
*/

int generateCodeForIf(IfNode* ifNode, int label, FILE* fp)
{
	if(ifNode==NULL)
		return label;
	IRNode* irNode = ifNode->exprVal;
	int labelOffset = label;
	label+=2;
	label = generateCodeForIR(irNode, label,fp);
	fprintf(fp, "pop BX\n"); //TODO: if (E>0)
	fprintf(fp, "cmp BX,0d\n");
	fprintf(fp, "jg L%d\n", labelOffset+1);
	label = generateCodeForElse(ifNode->elseNode,label,fp);
	fprintf(fp, "jmp L%d\n", labelOffset+2);
	fprintf(fp, "L%d:\n", labelOffset+1);
	label = generateCodeForSts(ifNode->stsNode,label,fp);
	fprintf(fp, "L%d:\n",labelOffset+2);
   	return label;
}

int generateCodeForElse(ElseNode* elseNode, int label, FILE* fp)
{
	if(elseNode == NULL)
		return label;
	if(elseNode->stsNode)
		label = generateCodeForSts(elseNode->stsNode,label,fp);
	else
		label = generateCodeForIf(elseNode->ifNode,label,fp);
	return label;
}

int generateCodeForSts(StsNode* stsNode,int label, FILE* fp)
{
	if(stsNode==NULL)
		return label;
	for(int i=stsNode->ptr-1;i>=0;i--)
	{
		if(stsNode->stNode[i])
		{
			label = generateCodeForSt(stsNode->stNode[i], label,fp);
		}
	}
	return label;
}

int generateCodeForSt(StNode* stNode,int label, FILE* fp)
{
	if(stNode==NULL)
		return label;
	if(stNode->doNode!=NULL)
	{
		return generateCodeForDo(stNode->doNode, label, fp);
	}
	else if(stNode->doWhNode!=NULL)
	{
		return generateCodeForDoWh(stNode->doWhNode, label, fp);
	}
	else if(stNode->ifNode!=NULL)
	{
		return generateCodeForIf(stNode->ifNode, label, fp);
	}
	else if(stNode->asgnNode)
	{
		return generateCodeForAsgn(stNode->asgnNode, label, fp);
	}
	else if(stNode->sayNode)
	{
		return generateCodeForSay(stNode->sayNode, label, fp);
	}
	return label;
}

int generateCodeForAsgn(AsgnNode* asgnNode, int label, FILE* fp)
{
	IRNode* irNode = asgnNode->exprVal;
	label = generateCodeForIR(irNode, label, fp);
	fprintf(fp, "pop BX\n");
	fprintf(fp, "mov word ptr [ss:%d],BX\n",(asgnNode->var->byteOffset)*2);
	return label;
}

int generateCodeForSay(SayNode* sayNode, int label, FILE* fp)
{
	IRNode* irNode = sayNode->exprVal;
	label = generateCodeForIR(irNode, label, fp);
	fprintf(fp, "pop BX\n");
	fprintf(fp, "mov AX, BX\n");
	fprintf(fp, "call print\n");
	return label;
}