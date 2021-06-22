%{
#include<stdio.h>
#include "nodes.h"
#include "Rexx.tab.h"

extern struct VarNode *symtable;
struct VarNode *symtable=NULL;
VarNode* addSymbol(char* str);
int byteOffset = 0;
%}
identifier 		[a-zA-Z_][a-zA-Z0-9_]*
digit 			[0-9]
int 			{digit}+
float           {digit}*"."{digit}+
string			["].*["]|['].*[']
multiComment	"/*"([^*]|[*][^/])*"*/"

%%
"say"			{return SAY;}
"if" 			{return IF;}
"else" 			{return ELSE;}
"then"			{return THEN;}
"while" 		{return WHILE;}
"do"			{return DO;}
"exit"			{printvars();return EXIT;}
"by"			{return BY;}
"end"			{return END;}
"to"			{return TO;}

"=" 			{return ASSIGN;}
"+"				{return PLUS;}
"-"				{return MINUS;}
"*"				{return MULTIPLY;}
"/"				{return DIVIDE;}
"%"				{return MODULO;}

"==" 			{return EQ;}
"!=" 			{return NEQ;}
"<=" 			{return LEQ;}
">=" 			{return GEQ;}
"<"				{return LTHEN;}
">"				{return GTHEN;}

"("				{return RNDOP;}
")"				{return RNDCL;}

"." 			{return DOT;}
"," 			{return COMMA;}

{int}			{yylval.ival=atoi(yytext);return NUM;}
{float}			{yylval.fval=atof(yytext);return FLOAT;}
{string}		{yylval.sval=yytext;return STRING;}
{identifier}	{yylval.nval=addSymbol(yytext);return ID;}
{multiComment}
\n 				{return ENDST;}                     
\r                      
[ \t]+                  
%%

VarNode* addSymbol(char* str)
{
	VarNode *temp=symtable, *newnode=NULL;
	if (temp==NULL)
	{
		newnode = createVarNode(str,byteOffset++);
		symtable=newnode;
	}
	else
	{
		while (temp->nptr !=NULL )
			{
				if (strcmp(temp->varname,str)==0)
					break;
				else	
					temp=temp->nptr;
			}
			if (strcmp(temp->varname,str)!=0)
			{
					newnode = createVarNode(str,byteOffset++);
					temp->nptr=newnode;
			}
			else
			{
				newnode=temp;
			}
	}
	return newnode;
}