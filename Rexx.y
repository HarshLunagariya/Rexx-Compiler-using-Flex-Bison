%{ 
#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nodes.h"
#include "methods.h"
#include "debug.h"
#include "codeGen.h"
extern struct VarNode *symtable;
int cnt=0;
int yywrap();
int yyerror();
void printvars();
int temp;
%}
%union 
{
  int ival;
  float fval;
  char* sval;
  struct Literal *lit;
  struct IRNode *irNode;
  struct VarNode *nval;
  struct DoNode *doNode;
  struct DoWhNode* doWhNode;
  struct IfNode* ifNode;
  struct ElseNode* elseNode;
  struct StsNode* stsNode;
  struct StNode* stNode;
  struct AsgnNode* asgnNode;
  struct SayNode* sayNode;
};

%token <ival> NUM
%token <fval> FLOAT 
%token <sval> STRING
%token <nval> ID
%type <irNode> E
%type <irNode> F
%type <irNode> G
%type <irNode> H
%type <doNode> DOST
%type <doWhNode> DOWHST
%type <ifNode> IFST
%type <elseNode> ELSEST
%type <stsNode> STS
%type <stNode> ST
%type <asgnNode> ASGNST
%type <sayNode> SAYST

%token IF ELSE THEN FOR WHILE DO EXIT BY END TO SAY
%token ASSIGN PLUS MINUS MULTIPLY DIVIDE MODULO 
%token EQ NEQ LEQ GEQ LTHEN GTHEN
%token RNDOP RNDCL
%token DOT COMMA ENDST
%%
PROG : STS EXIT ENDST {FILE *fp = fopen("C:\\Tasm 1.4\\Tasm\\code.asm", "a");generateCodeForSts($1,0,fp);fclose(fp);}
     ;
DOST : DO ASGNST TO E BY E ENDST STS END ENDST {$$ = createDoNode($2,$4,$6,$8,NULL);}
     | DO ASGNST BY E ENDST STS END ENDST {$$ = createDoNode($2,NULL,$4,$6,NULL);}
     | DO ASGNST TO E ENDST STS END ENDST {$$ = createDoNode($2,$4,NULL,$6,NULL);}
     | DO ASGNST ENDST STS END ENDST {$$ = createDoNode($2,NULL,NULL,$4,NULL);}
     | DO E ENDST STS END ENDST {$$ = createDoNode(NULL,NULL,NULL,$4,$2);}
     ;
DOWHST : DO WHILE RNDOP E RNDCL ENDST STS END ENDST {$$ = createDoWhNode($4,$7);}
      ;
IFST  : IF RNDOP E RNDCL THEN ENDST DO ENDST STS END ENDST {$$ = createIfNode($3,$9,NULL);}
      | IF RNDOP E RNDCL THEN ENDST DO ENDST STS END ENDST ELSEST {$$ = createIfNode($3,$9,$12);} 
      ;
ELSEST  : ELSE ENDST DO ENDST STS END ENDST {$$ = createElseNode($5,NULL);}
        | ELSE ENDST IFST {$$ = createElseNode(NULL,$3);}
        ;
STS : ST STS            {$$=$2; addToStsNode($$,$1);}
    | ST                {$$ = createStsNode(); addToStsNode($$,$1);}
    ;
ST  : DOST              {$$ = createStNodeWithDo($1);}
    | DOWHST            {$$ = createStNodeWithDoWh($1);}
    | IFST              {$$ = createStNodeWithIf($1);}
    | ASGNST ENDST      {$$ = createStNodeWithAsgn($1);}
    | SAYST             {$$ = createStNodeWithSay($1);}
    | ENDST             {$$ = createStNodeWithout();}
    ;
SAYST : SAY E ENDST     {$$ = createSayNode($2);}
      ;

ASGNST: ID ASSIGN E {$$ = createAsgnNode($1,$3);} 
      ; 
E : E LTHEN F       {$$ = createIRNode(strdup("<"),$1,$3,stoi(++cnt),NULL,NULL);}
  | E GTHEN F       {$$ = createIRNode(strdup(">"),$1,$3,stoi(++cnt),NULL,NULL);}
  | E GEQ F         {$$ = createIRNode(strdup(">="),$1,$3,stoi(++cnt),NULL,NULL);}
  | E LEQ F         {$$ = createIRNode(strdup("<="),$1,$3,stoi(++cnt),NULL,NULL);}
  | E EQ F          {$$ = createIRNode(strdup("=="),$1,$3,stoi(++cnt),NULL,NULL);}
  | F               {$$=$1;}
  ;
F : F PLUS G        {$$ = createIRNode(strdup("+"),$1,$3,stoi(++cnt),NULL,NULL);}
  | F MINUS G       {$$ = createIRNode(strdup("-"),$1,$3,stoi(++cnt),NULL,NULL);}
  | G               {$$=$1;}
  ;
G : G MULTIPLY H   {$$ = createIRNode(strdup("*"),$1,$3,stoi(++cnt),NULL,NULL);}
  | G DIVIDE H     {$$ = createIRNode(strdup("/"),$1,$3,stoi(++cnt),NULL,NULL);}
  | G MODULO H     {$$ = createIRNode(strdup("/"),$1,$3,stoi(++cnt),NULL,NULL);}
  | H              {$$ = $1;}
  ;
H : NUM            {$$ = createIRNode(NULL,NULL,NULL,stoi(++cnt),createLiteral($1,NULL),NULL);}
  | FLOAT          {$$ = createIRNode(NULL,NULL,NULL,stoi(++cnt),createLiteral($1,NULL),NULL);}
  | STRING         {$$ = createIRNode(NULL,NULL,NULL,stoi(++cnt),createLiteral(0,$1),NULL);}
  | ID             {if($1->declared==0){declarationerror($1);} $$ = createIRNode(NULL,NULL,NULL,stoi(++cnt),NULL,$1);}
  | RNDOP E RNDCL  {$$ = $2;}
  ;
%% 
 
int yywrap(){
  return 1;
}
int main(){
  yyparse();
  return 0;
}
int yyerror(){
  printf("\nSyntax error!\n");
  return 0;
}
void printvars()
{
  printf("\nSYMBOL TABLE\n");
  struct VarNode *temp;
  temp=symtable;
  while (temp!=NULL)
  {
    printf("Name: %s\n",temp->varname);
    //printLiteral(temp->literal);
    printf("_____________________\n");
    temp=temp->nptr;
  }
}
