#include"common.h"
#include"translate.h"
#include"FuncTable.h"
#include"IdTable.h"

#define out_file "out1.ir"

extern void optimize();

void pre_occupy_func()
{
	Type return_type = (Type)malloc(sizeof(struct Type_));
	return_type->kind = BASIC;
	return_type->u.basic = 0;
	int para_amount = 0;
	FieldList parameter = NULL;
	WriteFuncTable("read" , return_type , para_amount , parameter);
	
	return_type = (Type)malloc(sizeof(struct Type_));
	return_type->kind = BASIC;
	return_type->u.basic = 0;
	FuncTable[1].para_amount = 1;
	parameter = (FieldList)malloc(sizeof(struct FieldList_));
	parameter->name = NULL;
	parameter->type = (Type)malloc(sizeof(struct Type_));
	parameter->type->kind = BASIC;
	parameter->type->u.basic = 0;
	parameter->lineno = -1;
	parameter->next = NULL;
	WriteFuncTable("write" , return_type , para_amount , parameter);
}

void outputInterCode()
{
	FILE* des = fopen(out_file , "w"); 
	if(!des)
	{
		perror(out_file);
		return;
	}
	optimize();

	struct InterCodes* intercode_p = code_head;
	while(intercode_p->next != code_head)
	{
		intercode_p = intercode_p -> next;
		switch(intercode_p->code.kind)
		{
case LABEL:		outputlabel(intercode_p , des);		break;
case FUNCTION:		outputfunction(intercode_p , des);	break;
case ADD:
case SUB:
case MUL:
case DIV:		outputalop(intercode_p , des);		break;
case ADD_ASSIGN:
case REF_ASSIGN:
case ASSIGN_REF:
case ASSIGN:		outputassign(intercode_p , des);	break;
case GOTO:		outputgoto(intercode_p , des);		break;
case RELOP_GOTO: 	outputrelopgoto(intercode_p , des);	break;
case RETURN:		outputreturn(intercode_p , des);	break;
case DEC:		outputdec(intercode_p , des);		break;
case ARG:		outputarg(intercode_p , des);		break;
case CALLFUNC:		outputcallfunc(intercode_p , des);	break;
case PARAM:		outputparam(intercode_p , des);		break;
case READ:		outputread(intercode_p , des);		break;
case WRITE:		outputwrite(intercode_p , des);		break;
default:	fprintf(stderr , "%s , %d , %d \n" , __FILE__ , __LINE__ , intercode_p->code.kind);
		}
	}

	fclose(des);
}

void outputoperand(Operand op , FILE* des)
{
	switch(op->kind)
	{
		case VARIABLE: 		fprintf(des , "v%d" , op->u.var_no);break;
		case TEMP:		fprintf(des , "t%d" , op->u.temp_no);break;
		case CONSTANT:		fprintf(des , "#%d" , op->u.value);break;
		case FUNC:		fprintf(des , "%s" , op->u.func_name);break;
		case LABEL_SIGN:	fprintf(des , "label%d" , op->u.label_no);break;
		case ADDRESS:		fprintf(des , "&v%d" , op->u.var_no);break;
		case READ_ADDRESS:	fprintf(des , "*t%d" , op->u.temp_no);break;
		default:break;
	}
}

void outputlabel(struct InterCodes* intercode_p , FILE* des)
{
	fprintf(des , "LABEL label%d :\n" , intercode_p->code.u.label.x->u.label_no);
}

void outputfunction(struct InterCodes* intercode_p , FILE* des)
{
	fprintf(des , "FUNCTION %s :\n" , intercode_p->code.u.function.f->u.func_name);
}

void outputalop(struct InterCodes* intercode_p , FILE* des)
{
	outputoperand(intercode_p->code.u.alop.x , des);
	fprintf(des , " := ");
	outputoperand(intercode_p->code.u.alop.y , des);
	switch(intercode_p->code.kind)
	{
		case ADD: fprintf(des , " + ");break;
		case SUB: fprintf(des , " - ");break;
		case MUL: fprintf(des , " * ");break;
		case DIV: fprintf(des , " / ");break;
		default: break;
	}
	outputoperand(intercode_p->code.u.alop.z , des);
	fprintf(des , "\n");
}

void outputassign(struct InterCodes* intercode_p , FILE* des)
{
	outputoperand(intercode_p->code.u.assignop.x , des);
	fprintf(des , " := ");
	outputoperand(intercode_p->code.u.assignop.y , des);
	fprintf(des , "\n");
}

void outputgoto(struct InterCodes* intercode_p , FILE* des)
{
	fprintf(des , "GOTO label%d\n" , intercode_p->code.u.gotolabel.x->u.label_no);
}

void outputrelopgoto(struct InterCodes* intercode_p , FILE* des)
{
	fprintf(des , "IF ");
	outputoperand(intercode_p->code.u.relopgoto.x ,des);
	fprintf(des , " ");
	output_relop(intercode_p->code.u.relopgoto.relop , des);
	fprintf(des , " ");
	outputoperand(intercode_p->code.u.relopgoto.y ,des);
	fprintf(des , " ");
	fprintf(des , "GOTO ");
	outputoperand(intercode_p->code.u.relopgoto.z ,des);
	fprintf(des , "\n");
}

void outputreturn(struct InterCodes* intercode_p , FILE* des)
{
	fprintf(des , "RETURN ");
	outputoperand(intercode_p->code.u.ret.x , des);
	fprintf(des , "\n");
}

void outputdec(struct InterCodes* intercode_p , FILE* des)
{
	fprintf(des , "DEC ");
	outputoperand(intercode_p->code.u.dec.x , des);
	fprintf(des , " %d " , intercode_p->code.u.dec.size);
	fprintf(des , "\n");
}


void outputarg(struct InterCodes* intercode_p , FILE* des)
{
	fprintf(des , "ARG ");
	outputoperand(intercode_p->code.u.arg.x , des);
	fprintf(des , "\n");
}

void outputcallfunc(struct InterCodes* intercode_p , FILE* des)
{
	outputoperand(intercode_p->code.u.callfunc.x , des);
	fprintf(des , " := CALL ");
	outputoperand(intercode_p->code.u.callfunc.f , des);
	fprintf(des , "\n");
}

void outputparam(struct InterCodes* intercode_p , FILE* des)
{
	fprintf(des , "PARAM ");
	outputoperand(intercode_p->code.u.param.x , des);
	fprintf(des , "\n");
}

void outputread(struct InterCodes* intercode_p , FILE* des)
{
	fprintf(des , "READ ");
	outputoperand(intercode_p->code.u.read.x , des);
	fprintf(des , "\n");
}

void outputwrite(struct InterCodes* intercode_p , FILE* des)
{
	fprintf(des , "WRITE ");
	outputoperand(intercode_p->code.u.write.x , des);
	fprintf(des , "\n");
}

void initial_InterCodes()
{
	code_head = (struct InterCodes*)malloc(sizeof(struct InterCodes));
	code_head->next = code_head;
	code_head->prev = code_head;
	current_code = code_head;
	variable_num = 0;
	temp_num = 0;
	label_num = 0;
}

void insertcode(struct InterCodes* new_code)
{
	new_code->prev = current_code;
	new_code->next = current_code->next;
	current_code->next = new_code;
	current_code = new_code;
	code_head->prev = current_code;
}

int get_relop(struct tree_node* p )
{
	if(!strcmp(p->unit_name , ">"))
		return 0;
	else if(!strcmp(p->unit_name , "<"))
		return 1;
	else if(!strcmp(p->unit_name , ">="))
		return 2;
	else if(!strcmp(p->unit_name , "<="))
		return 3;
	else if(!strcmp(p->unit_name , "=="))
		return 4;
	else if(!strcmp(p->unit_name , "!="))
		return 5;
	else 
		return -1;
}

void output_relop(Operand op1 , FILE* des)
{
	switch(op1->u.relop)
	{
		case 0: fprintf(des , ">"); break;
		case 1: fprintf(des , "<"); break;
		case 2: fprintf(des , ">="); break;
		case 3: fprintf(des , "<="); break;
		case 4: fprintf(des , "=="); break;
		case 5: fprintf(des , "!="); break;
	}
}


void translate(struct tree_node* p)
{
	if(p == NULL)
		return;
	if(!strcmp(p->token_name , "FunDec"))
	{
		translate_function(p);
		return;
	}

/*	else if(!strcmp(p->token_name , "Exp"))
	{
		Operand op1;
		op1 = (Operand)malloc(sizeof(struct Operand_));
		translate_exp(p, op1);
		return ;
	}
	else if(!strcmp(p->token_name , "Stmt"))
	{
		translate_stmt(p);
		return;
	}
*/
	else if(!strcmp(p->token_name , "CompSt"))
	{
		translate_compst(p);
	}
	else
	{
		int i = 0;
		for(i = 0 ; i < p->children_num ; i++)
			translate(p->children[i]);
		return;
	}
}

void translate_function(struct tree_node* p)//FunDec->ID LP VarList RP
{
	char* func_name = p->children[0]->unit_name;
	struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
	new_code->code.kind = FUNCTION;
	new_code->code.u.function.f = (Operand)malloc(sizeof(struct Operand_));
	new_code->code.u.function.f->kind = FUNC;
	new_code->code.u.function.f->u.func_name = (char*)malloc(sizeof(char) * (strlen(func_name) + 1));
	strcpy(new_code->code.u.function.f->u.func_name , func_name);
	insertcode(new_code);

	if(p->children_num == 4)
		translate_varlist(p->children[2]);
	return;
}

void translate_exp(struct tree_node* p , Operand place)
{
	if(p->children_num == 1)
	{
		if(!strcmp(p->children[0]->token_name , "INT"))//Exp -> INT
		{
/*			place->kind = TEMP;
			place->u.temp_no = ++temp_num;

			Operand right;
			right = (Operand)malloc(sizeof(struct Operand_));
			right->kind = CONSTANT;
			right->u.value = atoi(p->children[0]->unit_name);
			struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = ASSIGN;
			new_code->code.u.assignop.x = place;
			new_code->code.u.assignop.y = right;

			insertcode(new_code);
			return;
*/
			place->kind = CONSTANT;
			place->u.value = atoi(p->children[0]->unit_name);
		}
		else if(!strcmp(p->children[0]->token_name , "FLOAT"))//Exp -> FLOAT
		{
			/* it seems never appear*/
			return;
		}
		else if(!strcmp(p->children[0]->token_name , "ID"))//Exp -> ID
		{

			int rank = lookup_idtable_rank(p->children[0]->unit_name);
			if(IdTable[rank].type->kind == STRUCT && IdTable[rank].param_or_not != 1)// 如果ID是一个结构体，并且不是当前函数的参数
			{
				place->kind = TEMP;
				place->u.temp_no = ++temp_num;

				Operand right = (Operand)malloc(sizeof(struct Operand_));
				right->kind = ADDRESS;
				right->u.var_no = IdTable[rank].var_no;
				struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
				new_code->code.kind = ASSIGN;
				new_code->code.u.assignop.x = place;
				new_code->code.u.assignop.y = right;
				insertcode(new_code);
			}
			else
			{
				place->kind = VARIABLE;
				place->u.var_no = IdTable[rank].var_no;
			}
			return;
		}
		else
			return;

	}
	else if(p->children_num == 2)
	{
		if(!strcmp(p->children[0]->token_name , "MINUS"))//Exp -> MINUS Exp
		{
			Operand t1;
			t1 = (Operand)malloc(sizeof(struct Operand_));
			translate_exp(p->children[1] , t1);

			place->kind = TEMP;
			place->u.temp_no = ++temp_num;

			Operand mid;
			mid = (Operand)malloc(sizeof(struct Operand_));
			mid->kind = CONSTANT;
			mid->u.value = 0;

			struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = SUB;
			new_code->code.u.alop.x = place;
			new_code->code.u.alop.y = mid;
			new_code->code.u.alop.z = t1;

			insertcode(new_code);

			return;
		}
		else if(!strcmp(p->children[0]->token_name , "NOT"))//Exp -> NOT Exp
			goto here;
		else
			return;

	}
	else if(p->children_num == 3)
	{
		if(!strcmp(p->children[1]->token_name , "ASSIGNOP"))//Exp -> Exp ASSIGNOP Exp
		{
			Operand t1 = (Operand)malloc(sizeof(struct Operand_));
			translate_exp(p->children[0] , t1);

			Operand t2 = (Operand)malloc(sizeof(struct Operand_));
			translate_exp(p->children[2] , t2);

			struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = ASSIGN;
			new_code->code.u.assignop.x = t1;
			new_code->code.u.assignop.y = t2;
			insertcode(new_code);

			place->kind = TEMP;
			place->u.temp_no = ++temp_num;
			new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = ASSIGN;
			new_code->code.u.assignop.x = place;
			new_code->code.u.assignop.y = t1;//根据经验，使用后面的一个优化更加方便
			insertcode(new_code);
			return;
		}
		else if(!strcmp(p->children[1]->token_name , "PLUS") || !strcmp(p->children[1]->token_name , "MINUS") || !strcmp(p->children[1]->token_name , "STAR") || !strcmp(p->children[1]->token_name , "DIV"))
		{
			Operand t1 = (Operand)malloc(sizeof(struct Operand_));
			translate_exp(p->children[0] , t1);

			Operand t2 = (Operand)malloc(sizeof(struct Operand_));
			translate_exp(p->children[2] , t2);

			place->kind = TEMP;
			place->u.temp_no = ++temp_num;

			struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));

			if(!strcmp(p->children[1]->token_name , "PLUS"))
				new_code->code.kind = ADD;
			else if(!strcmp(p->children[1]->token_name , "MINUS"))
				new_code->code.kind = SUB;
			else if(!strcmp(p->children[1]->token_name , "STAR"))
				new_code->code.kind = MUL;
			else
				new_code->code.kind = DIV;

			new_code->code.u.alop.x = place;
			new_code->code.u.alop.y = t1;
			new_code->code.u.alop.z = t2;

			insertcode(new_code);
		}
		else if(!strcmp(p->children[1]->token_name , "RELOP") || !strcmp(p->children[1]->token_name , "AND") || !strcmp(p->children[1]->token_name , "OR"))
		{
here:			
			place->kind = TEMP;
			place->u.temp_no = ++temp_num;

			Operand label1;
			label1 = (Operand)malloc(sizeof(struct Operand_));
			label1->kind = LABEL_SIGN;
			label1->u.label_no = ++label_num;

			Operand label2;
			label2 = (Operand)malloc(sizeof(struct Operand_));
			label2->kind = LABEL_SIGN;
			label2->u.label_no = ++label_num;


			Operand t1 = (Operand)malloc(sizeof(struct Operand_));
			t1->kind = CONSTANT;
			t1->u.value = 0;

			//code0
			struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = ASSIGN;
			new_code->code.u.assignop.x = place;
			new_code->code.u.assignop.y = t1;
			insertcode(new_code);
			
			//code1
			translate_cond(p , label1 , label2);

			//code2
			new_code = (struct  InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = LABEL;
			new_code->code.u.label.x = label1;
			insertcode(new_code);

			//code2
			t1 = (Operand)malloc(sizeof(struct Operand_));
			t1->kind = CONSTANT;
			t1->u.value = 1;

			new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = ASSIGN;
			new_code->code.u.assignop.x = place;
			new_code->code.u.assignop.y = t1;
			insertcode(new_code);

			new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = LABEL;
			new_code->code.u.label.x = label2;
			insertcode(new_code);

			return ;
		}
		else if(!strcmp(p->children[1]->token_name , "DOT"))//Exp -> Exp DOT ID
		{
			Operand op1;
			op1 = (Operand)malloc(sizeof(struct Operand_));
			translate_exp_dot_id(p , op1);

			Operand op2 = (Operand)malloc(sizeof(struct Operand_));
			op2->kind = TEMP;
			op2->u.temp_no = ++temp_num;

			struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = ASSIGN;
			new_code->code.u.assignop.x = op2;
			new_code->code.u.assignop.y = op1;
			insertcode(new_code);

			place->kind = READ_ADDRESS;	
			place->u.temp_no = op2->u.temp_no;
		}
		else if(!strcmp(p->children[0]->token_name , "ID"))//Exp -> ID LP RP
		{
			struct CharactInfoEntry_Func* function = lookup_functable(p->children[0]->unit_name); 
			if(!strcmp(function->func_name , "read"))
			{
				place->kind = TEMP;
				place->u.temp_no = ++temp_num;

				struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
				new_code->code.kind = READ;
				new_code->code.u.read.x = place;
				insertcode(new_code);
				return;
			}
			else
			{
				place->kind = TEMP;
				place->u.temp_no = ++temp_num;

				Operand func = (Operand)malloc(sizeof(struct Operand_));
				func->kind = FUNC;
				func->u.func_name = (char*)malloc(sizeof(char) * (strlen(function->func_name) + 1));
				strcpy(func->u.func_name , function->func_name);

				struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
				new_code->code.kind = CALLFUNC;
				new_code->code.u.callfunc.x = place;
				new_code->code.u.callfunc.f = func;
				insertcode(new_code);
				return;
			}


		}
		else if(!strcmp(p->children[0]->token_name , "LP"))
		{
			translate_exp(p->children[1] , place);
			return;
		}
		else 
			return;

	}
	else if(p->children_num == 4)
	{
		if(!strcmp(p->children[1]->token_name , "LB"))//Exp -> Exp LB Exp RB
		{
			if(!strcmp(p->children[0]->children[0]->token_name , "Exp"))//第一个Exp有可能是结构体
			{
				Operand op = (Operand)malloc(sizeof(struct Operand_));	
				translate_exp(p->children[0] , op);
				op->kind = TEMP;

				Operand t1 = (Operand)malloc(sizeof(struct Operand_));	

				Operand t2 = (Operand)malloc(sizeof(struct Operand_));	
				t2->kind = TEMP;
				t2->u.temp_no = ++temp_num;

				Operand t3 = (Operand)malloc(sizeof(struct Operand_));	
				t3->kind = TEMP;
				t3->u.temp_no = ++temp_num;

				char* name = p->children[0]->children[2]->unit_name;
				int rank = lookup_idtable_rank(name);
				int size = get_size_type(IdTable[rank].type->u.array.elem);

				translate_exp(p->children[2] , t1);


				Operand t4 = (Operand)malloc(sizeof(struct Operand_));
				t4->kind = CONSTANT;
				t4->u.value = size;

				struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
				new_code->code.kind = MUL;
				new_code->code.u.alop.x = t2;
				new_code->code.u.alop.y = t1;
				new_code->code.u.alop.z = t4;
				insertcode(new_code);

				new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
				new_code->code.kind = ADD;
				new_code->code.u.alop.x = t3;
				new_code->code.u.alop.y = op;
				new_code->code.u.alop.z = t2;
				insertcode(new_code);

				place->kind = READ_ADDRESS;
				place->u.temp_no = t3->u.temp_no;
				return;
			}
			else
			{
				Operand t1 = (Operand)malloc(sizeof(struct Operand_));	

				Operand t2 = (Operand)malloc(sizeof(struct Operand_));	
				t2->kind = TEMP;
				t2->u.temp_no = ++temp_num;

				Operand t3 = (Operand)malloc(sizeof(struct Operand_));	
				t3->kind = TEMP;
				t3->u.temp_no = ++temp_num;

				char* name = p->children[0]->children[0]->unit_name;
				int rank = lookup_idtable_rank(name);
				int size = get_size_type(IdTable[rank].type->u.array.elem);

				translate_exp(p->children[2] , t1);


				Operand t4 = (Operand)malloc(sizeof(struct Operand_));
				t4->kind = CONSTANT;
				t4->u.value = size;

				struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
				new_code->code.kind = MUL;
				new_code->code.u.alop.x = t2;
				new_code->code.u.alop.y = t1;
				new_code->code.u.alop.z = t4;
				insertcode(new_code);

				Operand op = (Operand)malloc(sizeof(struct Operand_));
				op->kind = ADDRESS;
				op->u.var_no = IdTable[rank].var_no;

				new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
				new_code->code.kind = ADD;
				new_code->code.u.alop.x = t3;
				new_code->code.u.alop.y = op;
				new_code->code.u.alop.z = t2;
				insertcode(new_code);

				place->kind = READ_ADDRESS;
				place->u.temp_no = t3->u.temp_no;
			}
		}
		else if(!strcmp(p->children[0]->token_name , "ID"))//Exp -> ID LP Args Rp
		{
			struct CharactInfoEntry_Func* function = lookup_functable(p->children[0]->unit_name); 
			struct Arg* arg_list = NULL;
			translate_args(p->children[2] , &arg_list);

			if(!strcmp(function->func_name , "write"))
			{
				struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
				new_code->code.kind = WRITE;
				new_code->code.u.write.x = arg_list->op;
				insertcode(new_code);
			}
			else
			{
				struct Arg* arg_p = arg_list;
		       		while(arg_p != NULL)
				{
					struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
					new_code->code.kind = ARG;
					new_code->code.u.arg.x = arg_p->op;
					insertcode(new_code);

					arg_p = arg_p->next;
				}	

				place->kind = TEMP;
				place->u.temp_no = ++temp_num;

				Operand func = (Operand)malloc(sizeof(struct Operand_));
				func->kind = FUNC;
				func->u.func_name = (char*)malloc(sizeof(char) * (strlen(function->func_name) + 1));
				strcpy(func->u.func_name , function->func_name);

				struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
				new_code->code.kind = CALLFUNC;
				new_code->code.u.callfunc.x = place;
				new_code->code.u.callfunc.f = func;
				insertcode(new_code);
				return;
			}
		}
		else 
			return;
	}
	else 
		return;
}

void translate_cond(struct tree_node* p , Operand label_true , Operand label_false)
{
	if(p->children_num == 2)//Exp -> NOT Exp
	{
		if(!strcmp(p->children[0]->token_name , "NOT"))
		{
			translate_cond(p->children[1] , label_false , label_true);
			return;
		}
		else
		{
			Operand t1 = (Operand)malloc(sizeof(struct Operand_));
			translate_exp(p , t1);

			Operand relop = (Operand)malloc(sizeof(struct Operand_));
			relop->kind = RELOP;
			relop->u.relop = 5;//!=

			Operand t2 = (Operand)malloc(sizeof(struct Operand_));
			t2->kind = CONSTANT;
			t2->u.value = 0;
	
			struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = RELOP_GOTO;
			new_code->code.u.relopgoto.x = t1;
			new_code->code.u.relopgoto.y = t2;
			new_code->code.u.relopgoto.z = label_true;
			new_code->code.u.relopgoto.relop = relop;
			insertcode(new_code);
	
			new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = GOTO;
			new_code->code.u.gotolabel.x = label_false;
			insertcode(new_code);
		}
	}
	else if(p->children_num == 3)
	{
		if(!strcmp(p->children[1]->token_name , "AND"))
		{
			Operand label1;
			label1 = (Operand)malloc(sizeof(struct Operand_));
			label1->kind = LABEL_SIGN;
			label1->u.label_no = ++label_num;

			translate_cond(p->children[0] , label1 , label_false);
	
			struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
			new_code = (struct  InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = LABEL;
			new_code->code.u.label.x = label1;

			insertcode(new_code);

			translate_cond(p->children[2] , label_true , label_false);
			return;
		}
		else if(!strcmp(p->children[1]->token_name , "OR"))
		{
			Operand label1;
			label1 = (Operand)malloc(sizeof(struct Operand_));
			label1->kind = LABEL_SIGN;
			label1->u.label_no = ++label_num;

			translate_cond(p->children[0] , label_true , label1);

			struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
			new_code = (struct  InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = LABEL;
			new_code->code.u.label.x = label1;

			insertcode(new_code);

			translate_cond(p->children[2] , label_true , label_false);
			return;
		}
		else if(!strcmp(p->children[1]->token_name , "RELOP"))
		{
			Operand t1 = (Operand)malloc(sizeof(struct Operand_));
			translate_exp(p->children[0] , t1);

			Operand t2 = (Operand)malloc(sizeof(struct Operand_));
			translate_exp(p->children[2] , t2);

			Operand relop = (Operand)malloc(sizeof(struct Operand_));
			relop->kind = RELOP;
			relop->u.relop = get_relop(p->children[1]);

			struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = RELOP_GOTO;
			new_code->code.u.relopgoto.x = t1;
			new_code->code.u.relopgoto.y = t2;
			new_code->code.u.relopgoto.z = label_true;
			new_code->code.u.relopgoto.relop = relop;

			insertcode(new_code);

			new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = GOTO;
			new_code->code.u.gotolabel.x = label_false;

			insertcode(new_code);

			return;
		}
		else
		{
			Operand t1 = (Operand)malloc(sizeof(struct Operand_));
			translate_exp(p , t1);

			Operand relop = (Operand)malloc(sizeof(struct Operand_));
			relop->kind = RELOP;
			relop->u.relop = 5;//!=

			Operand t2 = (Operand)malloc(sizeof(struct Operand_));
			t2->kind = CONSTANT;
			t2->u.value = 0;
	
			struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = RELOP_GOTO;
			new_code->code.u.relopgoto.x = t1;
			new_code->code.u.relopgoto.y = t2;
			new_code->code.u.relopgoto.z = label_true;
			new_code->code.u.relopgoto.relop = relop;
			insertcode(new_code);
	
			new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = GOTO;
			new_code->code.u.gotolabel.x = label_false;
			insertcode(new_code);
			return ;
		}

	}
	else
	{
		Operand t1 = (Operand)malloc(sizeof(struct Operand_));
		translate_exp(p , t1);

		Operand relop = (Operand)malloc(sizeof(struct Operand_));
		relop->kind = RELOP;
		relop->u.relop = 5;//!=

		Operand t2 = (Operand)malloc(sizeof(struct Operand_));
		t2->kind = CONSTANT;
		t2->u.value = 0;

		struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
		new_code->code.kind = RELOP_GOTO;
		new_code->code.u.relopgoto.x = t1;
		new_code->code.u.relopgoto.y = t2;
		new_code->code.u.relopgoto.z = label_true;
		new_code->code.u.relopgoto.relop = relop;
		insertcode(new_code);

		new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
		new_code->code.kind = GOTO;
		new_code->code.u.gotolabel.x = label_false;
		insertcode(new_code);

		return;
	}
}

void translate_stmt(struct tree_node* p)
{
	if(p->children_num == 1)//Stmt -> CompSt
	{
		translate_compst(p->children[0]);
	}
	else if(p->children_num == 2)// Stmt -> Exp SEMI
	{
		Operand op1;
		op1 = (Operand)malloc(sizeof(struct Operand_));
		translate_exp(p->children[0], op1);
	}
	else if(p->children_num == 3)// Stmt -> RETURN Exp SEMI
	{
		Operand op1;
		op1 = (Operand)malloc(sizeof(struct Operand_));
		translate_exp(p->children[1] , op1);

		struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
		new_code->code.kind = RETURN;
		new_code->code.u.ret.x = op1;
		insertcode(new_code);

		return;
	}
	else if(p->children_num == 5)
	{
		if(!strcmp(p->children[0]->token_name , "IF"))//Stmt -> IF LP Exp RP Stmt
		{
			Operand label1 = (Operand)malloc(sizeof(struct Operand_));
			label1->kind = LABEL_SIGN;
			label1->u.label_no = ++label_num;

			Operand label2 = (Operand)malloc(sizeof(struct Operand_));
			label2->kind = LABEL_SIGN;
			label2->u.label_no = ++label_num;

			translate_cond(p->children[2] , label1 , label2);

			struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
			new_code = (struct  InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = LABEL;
			new_code->code.u.label.x = label1;
			insertcode(new_code);

			translate_stmt(p->children[4]);

			new_code = (struct  InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = LABEL;
			new_code->code.u.label.x = label2;
			insertcode(new_code);

			return;
		}
		else if(!strcmp(p->children[0]->token_name , "WHILE"))//Stmt -> WHILE LP Exp RP Stmt
		{
			Operand label1 = (Operand)malloc(sizeof(struct Operand_));
			label1->kind = LABEL_SIGN;
			label1->u.label_no = ++label_num;

			Operand label2 = (Operand)malloc(sizeof(struct Operand_));
			label2->kind = LABEL_SIGN;
			label2->u.label_no = ++label_num;

			Operand label3 = (Operand)malloc(sizeof(struct Operand_));
			label3->kind = LABEL_SIGN;
			label3->u.label_no = ++label_num;

			struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
			new_code = (struct  InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = LABEL;
			new_code->code.u.label.x = label1;
			insertcode(new_code);

			translate_cond(p->children[2] , label2 , label3);

			new_code = (struct  InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = LABEL;
			new_code->code.u.label.x = label2;
			insertcode(new_code);

			translate_stmt(p->children[4]);

			new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = GOTO;
			new_code->code.u.gotolabel.x = label1;
			insertcode(new_code);

			new_code = (struct  InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = LABEL;
			new_code->code.u.label.x = label3;
			insertcode(new_code);

			return ;
		}
		else
			return;

	}
	else if(p->children_num == 7)//Stmt -> IF LP Exp RP Stmt ELSE Stmt
	{
		Operand label1 = (Operand)malloc(sizeof(struct Operand_));
		label1->kind = LABEL_SIGN;
		label1->u.label_no = ++label_num;

		Operand label2 = (Operand)malloc(sizeof(struct Operand_));
		label2->kind = LABEL_SIGN;
		label2->u.label_no = ++label_num;

		Operand label3 = (Operand)malloc(sizeof(struct Operand_));
		label3->kind = LABEL_SIGN;
		label3->u.label_no = ++label_num;
		
		//code1
		translate_cond(p->children[2] , label1 , label2);

		struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
		new_code = (struct  InterCodes*)malloc(sizeof(struct InterCodes));
		new_code->code.kind = LABEL;
		new_code->code.u.label.x = label1;
		insertcode(new_code);

		//code2
		translate_stmt(p->children[4]);

		new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
		new_code->code.kind = GOTO;
		new_code->code.u.gotolabel.x = label3;
		insertcode(new_code);

		new_code = (struct  InterCodes*)malloc(sizeof(struct InterCodes));
		new_code->code.kind = LABEL;
		new_code->code.u.label.x = label2;
		insertcode(new_code);

		//code3
		translate_stmt(p->children[6]);

		new_code = (struct  InterCodes*)malloc(sizeof(struct InterCodes));
		new_code->code.kind = LABEL;
		new_code->code.u.label.x = label3;
		insertcode(new_code);

		return ;
	}
	else
		return;
}

void translate_compst(struct tree_node* p)//LC DefList StmtList RC
{
	translate_deflist(p->children[1]);
	translate_stmtlist(p->children[2]);
}

void translate_deflist(struct tree_node* p)
{
	if(p == NULL)//DefList -> e
		return ;
	else//DefList -> Def DefList
	{
		translate_def(p->children[0]);
		translate_deflist(p->children[1]);
		return;
	}
}

void translate_stmtlist(struct tree_node* p)
{
	if(p == NULL)
		return;
	else
	{
		translate_stmt(p->children[0]);
		translate_stmtlist(p->children[1]);
		return;
	}
}

void translate_def(struct tree_node* p)
{
	translate_declist(p->children[1]);//Def -> Specifier DecList SEMI
}

void translate_declist(struct tree_node* p)
{
	if(p->children_num == 1)//DecList -> Dec
	{
		translate_dec(p->children[0]);
		return;

	}
	else if(p->children_num == 3)//DecList -> Dec COMMA DecList
	{
		translate_dec(p->children[0]);
		translate_declist(p->children[2]);
		return;
	}
	else
		return;
}

void translate_dec(struct tree_node* p)
{
	if(p->children_num == 1)//Dec -> VarDec
	{
		char *id_name = NULL;
		if(p->children[0]->children_num == 1)//VarDec -> ID
			id_name = p->children[0]->children[0]->unit_name;
		else//VarDec -> VarDec LB INT RB
		{
			if(p->children[0]->children[0]->children_num == 1)
				id_name = p->children[0]->children[0]->children[0]->unit_name;
			else//高维数组
			{
				fprintf(stderr , "Cannot translate: Code contains variables or multi-dimensional array type or parameters of array type.\n");
				return;
			}
		}
		int rank = lookup_idtable_rank(id_name);
		if(IdTable[rank].type->kind == STRUCT)// judge the type of VarDec
		{
			int size = get_size_type(IdTable[rank].type);
			Operand place = (Operand)malloc(sizeof(struct Operand_));
			place->kind = VARIABLE;
			place->u.var_no = IdTable[rank].var_no;

			struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = DEC;
			new_code->code.u.dec.x = place;
			new_code->code.u.dec.size = size;
			insertcode(new_code);

			return;
		}
		else if(IdTable[rank].type->kind == ARRAY)//如果是一维数组，也要分配空间
		{
			int size = get_size_type(IdTable[rank].type);
			Operand place = (Operand)malloc(sizeof(struct Operand_));
			place->kind = VARIABLE;
			place->u.var_no = IdTable[rank].var_no;

			struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = DEC;
			new_code->code.u.dec.x = place;
			new_code->code.u.dec.size = size;
			insertcode(new_code);

			return;
		}
		else
			return;
	}
	else if(p->children_num == 3)// Dec -> VarDec ASSIGNOP Exp
	{

		char* id_name = p->children[0]->children[0]->unit_name;
		int var_no = lookup_idtable(id_name);
		Operand t1 = (Operand)malloc(sizeof(struct Operand_));
		translate_exp(p->children[2] , t1);

		Operand place = (Operand)malloc(sizeof(struct Operand_));
		place->kind = VARIABLE;
		place->u.var_no = var_no;

		struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
		new_code->code.kind = ASSIGN;
		new_code->code.u.assignop.x = place;
		new_code->code.u.assignop.y = t1;

		insertcode(new_code);
		return;
	}
	else
		return;
}

void translate_args(struct tree_node* p , struct Arg** arg_list)
{

	Operand t1 = (Operand)malloc(sizeof(struct Operand_));
	translate_exp(p->children[0] , t1);
	/*参数是结构体的情况交给translate_exp来考虑*/

	if(*arg_list == NULL)
	{
		*arg_list = (struct Arg*)malloc(sizeof(struct Arg));
		(*arg_list)->op = t1;
		(*arg_list)->next = NULL;
	}
	else
	{
		struct Arg* current_arg = (struct Arg*)malloc(sizeof(struct Arg));
		current_arg->next = *arg_list;
		current_arg->op = t1;
		*arg_list = current_arg;
	}
	if(p->children_num == 3)//Args -> Exp COMMA Args
		translate_args(p->children[2] , arg_list);
	else 
		return;
}

void translate_varlist(struct tree_node* p)
{
	translate_paramdec(p->children[0]);
	if(p->children_num == 3)//VarList -> ParamDec COMMA VarList
	{
		translate_varlist(p->children[2]);
	}
}

void translate_paramdec(struct tree_node* p)
{
	translate_vardec(p->children[1]);//ParamDec -> Specifier VarDec
}

void translate_vardec(struct tree_node* p)//only used in paramdec
{
	if(p->children_num == 4)//VarDec -> VarDec LB INT RB 数组不作为参数传入
	{
		fprintf(stderr , "Cannot translate: Code contains variables or multi-dimensional array type or parameters of array type.\n");
		return;
	}
	else
	{
		Operand t1 = (Operand)malloc(sizeof(struct Operand_));
		int rank = lookup_idtable_rank(p->children[0]->unit_name);
		if(IdTable[rank].type->kind == STRUCT)// judge the type of param
			IdTable[rank].param_or_not = 1;//作为参数传入的结构体变量是地址,把他当作变量来看
		t1->kind = VARIABLE;
		t1->u.var_no = IdTable[rank].var_no;

		struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
		new_code->code.kind = PARAM;
		new_code->code.u.param.x = t1;
		insertcode(new_code);
	}
}

int get_size_type(Type type)
{
	int sum_size = 0;
	if(type->kind == BASIC)
		sum_size += 4;
	else if(type->kind == ARRAY)
	{
/*		if(type->u.array.elem->kind == ARRAY)
		{
			fprintf(stderr , "Cannot translate: Code contains variables or multi-dimensional array type or parameters of array type.\n");
			exit(1);
		}
		else
*/
		sum_size = sum_size + type->u.array.size * get_size_type(type->u.array.elem);
	}
	else if(type->kind == STRUCT)
		sum_size += get_size_structure(type->u.structure);
	else
		return 0;
	return sum_size;
}

int get_size_structure(FieldList fieldlist)
{
	int sum_size = 0;
	FieldList field = fieldlist;
	while(field != NULL)
	{
		sum_size += get_size_type(field->type);
		field = field->next;
	}
	return sum_size;
}

FieldList translate_exp_dot_id(struct tree_node* p , Operand place)
{
	FieldList subfield = NULL;
	if(p->children_num == 3)//Exp -> Exp DOT ID
	{
		int size = 0;
		Operand t2 = (Operand)malloc(sizeof(struct Operand_));
		subfield = translate_exp_dot_id(p->children[0] , t2);
		while(subfield != NULL)
		{
			if(!strcmp(subfield->name , p->children[2]->unit_name))
				break;
			else
				size = size + get_size_type(subfield->type);
			subfield = subfield->next;
		}
		
		Operand t1 = (Operand)malloc(sizeof(struct Operand_));
		t1->kind = CONSTANT;
		t1->u.value = size;

		place->kind = TEMP;
		place->u.temp_no = ++temp_num;

		struct InterCodes* new_code = (struct InterCodes* )malloc(sizeof(struct InterCodes));
		new_code->code.kind = ADD;
		new_code->code.u.alop.x = place;
		new_code->code.u.alop.y = t2;
		new_code->code.u.alop.z = t1;
		insertcode(new_code);

		return subfield;
	}
	else if(p->children_num == 4)//Exp -> Exp LB Exp RB
	{
		Operand t0 = (Operand)malloc(sizeof(struct Operand_));
		subfield = translate_exp_dot_id(p->children[0] , t0);
		int size = 0;
		while(subfield != NULL)
		{
			if(!strcmp(subfield->name , p->children[2]->unit_name))
				break;
			else
				size = size + get_size_type(subfield->type);
			subfield = subfield->next;
		}

		Operand t1 = (Operand)malloc(sizeof(struct Operand_));	
		translate_exp(p->children[2] , t1);

		Operand t2 = (Operand)malloc(sizeof(struct Operand_));
		t2->kind = CONSTANT;
		int rank = lookup_idtable_rank(p->children[0]->children[2]->unit_name);
		t2->u.value = get_size_type(IdTable[rank].type);

		Operand t3 = (Operand)malloc(sizeof(struct Operand_));
		t3->kind = TEMP;
		t3->u.temp_no = ++temp_num;

		struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
		new_code->code.kind = MUL;
		new_code->code.u.alop.x = t3;
		new_code->code.u.alop.y = t1;
		new_code->code.u.alop.z = t2;
		insertcode(new_code);

		place->kind = TEMP;
		place->u.temp_no = ++temp_num;

		new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
		new_code->code.kind = ADD;
		new_code->code.u.alop.x = place;
		new_code->code.u.alop.y = t0;
		new_code->code.u.alop.z = t3;
		insertcode(new_code);

		return subfield;
	}
	else//Exp -> ID
	{
		translate_exp(p , place);
		int rank = lookup_idtable_rank(p->children[0]->unit_name);
		Type type = IdTable[rank].type;
		subfield = type->u.structure;
		return subfield;
	}
}
