#ifndef __SEMAN__
#define __SEMAN__


typedef struct Type_* Type;
typedef struct FieldList_* FieldList;
typedef enum Bool bool;

enum Bool {FALSE , TRUE};

struct Type_
{
	enum{BASIC , ARRAY , STRUCT} kind;
	union
	{
		int basic;//if Int basic = 0 , if Float basic = 1
		struct{Type elem; int size;}array;
		FieldList structure;
	}u;
};

struct FieldList_
{
	char* name;
	Type type;
	FieldList next;
};


//void FillFieldList(FieldList target , FieldList origin);
void FillType(Type target , Type origin);
bool IsSameName(char* name);



void CurrentProgram(struct tree_node* p);

void CurrentExtDefList(struct tree_node* p);

void CurrentExtDef(struct tree_node* p);	

Type CurrentSpecifier(struct tree_node* p);

char* CurrentTag(struct tree_node* p);

char* CurrentOptTag(struct tree_node* p);

void CurrentExtDecList(Type inh , struct tree_node* p);

Type CurrentStructSpecifier(struct tree_node* p);

Type CurrentVarDec(Type inh , struct tree_node* p , bool InStructure);

FieldList CurrentDefList(struct tree_node* p , bool InStructure);

Type CurrentDef(struct tree_node* p , bool InStructure);

void CurrentDecList(Type inh , struct tree_node* p , bool InStructure);

void CurrentDec(Type inh , struct tree_node* p , bool InStructure);

void CurrentFunDec(Type inh , struct tree_node* p);

FieldList CurrentVarList(struct tree_node* p , int* para_amount);

Type CurrentParamDec(struct  tree_node* p);
#endif
