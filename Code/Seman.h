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
	int lineno;
	FieldList next;
};


//void FillFieldList(FieldList target , FieldList origin);
void FillType(Type target , Type origin);
bool IsSameName(char* name );
void IsSameNameInStructure(FieldList field);
bool IsSameType(Type target , Type origin);
bool IsSameStructure(FieldList target , FieldList origin);
bool IsSameNameInField(FieldList , char*);




void CurrentProgram(struct tree_node* p);

void CurrentExtDefList(struct tree_node* p);

void CurrentExtDef(struct tree_node* p);	

void CurrentExtDecList(Type inh , struct tree_node* p);

Type CurrentSpecifier(struct tree_node* p);

Type CurrentStructSpecifier(struct tree_node* p);

char* CurrentOptTag(struct tree_node* p);

char* CurrentTag(struct tree_node* p);

FieldList CurrentVarDec(Type type , struct tree_node* p);

void CurrentFunDec(Type inh , struct tree_node* p , bool);

FieldList CurrentVarList(struct tree_node* p , int* para_amount);

Type CurrentParamDec(struct  tree_node* p);

FieldList CurrentDef(struct tree_node* p);

FieldList  CurrentDefList(struct tree_node* p);

FieldList CurrentDecList(Type type , struct tree_node* p);

FieldList CurrentDec(Type type , struct tree_node* p);

void CurrentCompSt(Type type , struct tree_node* p , bool* return_right);

void CurrentStmtList(Type type , struct tree_node* p , bool* return_right);

void CurrentStmt(Type type , struct tree_node* p , bool* return_right);

void CurrentDefList_1(struct tree_node* p);

void CurrentDef_1(struct tree_node* p);

void CurrentDecList_1(Type type, struct tree_node* p);

void CurrentDec_1(Type type , struct tree_node* p);

FieldList CurrentVarDec_1(Type type , struct tree_node* p);

void CurrentReturnExp(Type return_type , struct tree_node* p , bool* return_right);

Type CurrentExp(struct tree_node* p);

bool IsLeft(struct tree_node*p);

FieldList CurrentArgs(struct tree_node*);
#endif
