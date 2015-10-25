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



/* the information of struct type*/
struct CharactInfoEntry_Struct
{
	int valid;
	int lineNumber;
	char* Struct_name;
	FieldList entry;
};
struct CharactInfoEntry_Struct StructTable[10];



/*the information of id*/
struct CharactInfoEntry_Id
{
	int valid;
	int lineNumber;
	FieldList entry;
};
struct CharactInfoEntry_Id IdTable[10];



/* the information of function*/
struct CharactInfoEntry_Func
{
	int valid;// if current entry is valid , 1 ,else 0
	char* func_name;
	int lineNumber;
	Type return_type;
	int para_amount;
	Type* parameter;
};

struct CharactInfoEntry_Func FuncTable[10];

void Creat_Struct_or_Id(struct tree_node* p);
