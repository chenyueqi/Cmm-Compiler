#include"common.h"
#include"Seman.h"


void Seman_analysis(struct tree_node* p)
{
#ifdef DEBUG
	printf("here is semantic analysis !\n");
#endif

	if(!strcmp(p->token_name , "StructSpecifier"))
		Creat_Struct_or_Id(p);
	
	display_tree(p , 0);
}

void Creat_Struct_or_Id(struct tree_node* p)
{
	if(p->children_num == 5)// creat a new type of struct
	{
		

	}
	else if(p->children_num == 2)//creat a new id
	{

	}
}
