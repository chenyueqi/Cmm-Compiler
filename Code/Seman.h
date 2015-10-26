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

void FillFieldList(FieldList target , FieldList origin);
void FillType(Type target , Type origin);




void CurrentExtDef(struct tree_node* p);	

Type CurrentSpecifier(struct tree_node* p);

void CurrentExtDecList(Type inh , struct tree_node* p);

Type CurrentStructSpecifier(struct tree_node* p);

void CurrentExtDecList(Type inh , struct tree_node* p);

void CurrentVarDec(Type inh , struct tree_node* p);

FieldList CurrentDefList(struct tree_node* p);
#endif
