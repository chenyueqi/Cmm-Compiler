#include"common.h"
#include"translate.h"
#include"FuncTable.h"
#include"IdTable.h"

#define out_file "out1.ir"

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
		case VARIABLE: 	fprintf(des , "v%d" , op->u.var_no);break;
		case TEMP:	fprintf(des , "t%d" , op->u.temp_no);break;
		case CONSTANT:	fprintf(des , "#%d" , op->u.value);break;
	}
}

void outputlabel(struct InterCodes* intercode_p , FILE* des)
{
	fprintf(des , "LABEL lable%d :\n" , intercode_p->code.u.label.x->u.label_no);
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
	fprintf(des , "GOTO lable%d : \n" , intercode_p->code.u.gotolabel.x->u.label_no);
}

void outputrelopgoto(struct InterCodes* intercode_p , FILE* des)
{
	fprintf(des , "IF");
	outputoperand(intercode_p->code.u.relopgoto.x ,des);
	fprintf(des , " ");
	output_relop(intercode_p->code.u.relopgoto.relop , des);
	fprintf(des , " ");
	outputoperand(intercode_p->code.u.relopgoto.y ,des);
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

}


void outputarg(struct InterCodes* intercode_p , FILE* des)
{

}

void outputcallfunc(struct InterCodes* intercode_p , FILE* des)
{
	
}

void outputparam(struct InterCodes* intercode_p , FILE* des)
{
	
}

void outputread(struct InterCodes* intercode_p , FILE* des)
{
	
}

void outputwrite(struct InterCodes* intercode_p , FILE* des)
{
	
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
	else if(!strcmp(p->token_name , "Exp"))
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
	else
	{
		int i = 0;
		for(i = 0 ; i < p->children_num ; i++)
			translate(p->children[i]);
		return;
	}

}

void translate_function(struct tree_node* p)
{
	char* func_name = p->children[0]->unit_name;
	struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
	new_code->code.kind = FUNCTION;
	new_code->code.u.function.f = (Operand)malloc(sizeof(struct Operand_));
	new_code->code.u.function.f->kind = FUNC;
	new_code->code.u.function.f->u.func_name = (char*)malloc(sizeof(char) * (strlen(func_name) + 1));
	strcpy(new_code->code.u.function.f->u.func_name , func_name);

	insertcode(new_code);
	return;
}

void translate_exp(struct tree_node* p , Operand place)
{
	if(p->children_num == 1)
	{
		if(!strcmp(p->children[0]->token_name , "INT"))//Exp -> INT
		{
			place->kind = TEMP;
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
		}
		else if(!strcmp(p->children[0]->token_name , "FLOAT"))//Exp -> FLOAT
		{
			/* it seems never appear*/
			return;
		}
		else if(!strcmp(p->children[0]->token_name , "ID"))//Exp -> ID
		{
			place->kind = TEMP;
			place->u.temp_no = ++temp_num;

			Operand right;
			right = (Operand)malloc(sizeof(struct Operand_));
			right->kind = VARIABLE;
			right->u.var_no = lookup(p->children[0]->unit_name);
			struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = ASSIGN;
			new_code->code.u.assignop.x = place;
			new_code->code.u.assignop.y = right;

			insertcode(new_code);
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
			char* id_name = p->children[0]->children[0]->unit_name;
			int var_no = lookup(id_name);
			Operand t1 = (Operand)malloc(sizeof(struct Operand_));
			translate_exp(p->children[2] , t1);

			place->kind = VARIABLE;
			place->u.var_no = var_no;

			struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = ASSIGN;
			new_code->code.u.assignop.x = place;
			new_code->code.u.assignop.y = t1;

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

			struct InterCodes* new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = ASSIGN;
			new_code->code.u.assignop.x = place;
			new_code->code.u.assignop.y = t1;
			
			//code0
			insertcode(new_code);
			
			//code1
			translate_cond(p , label1 , label2);

			new_code = (struct  InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = LABEL;
			new_code->code.u.label.x = label1;

			//code2
			insertcode(new_code);

			t1 = (Operand)malloc(sizeof(struct Operand_));
			t1->kind = CONSTANT;
			t1->u.value = 1;

			new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = ASSIGN;
			new_code->code.u.assignop.x = place;
			new_code->code.u.assignop.y = t1;
			
			//code2
			insertcode(new_code);

			new_code = (struct InterCodes*)malloc(sizeof(struct InterCodes));
			new_code->code.kind = LABEL;
			new_code->code.u.label.x = label2;

			insertcode(new_code);

			return ;
		}
		else if(!strcmp(p->children[1]->token_name , "DOT"))
		{

		}
		else 
			return;

	}
	else if(p->children_num == 4)
	{
		if(!strcmp(p->children[1]->token_name , "LB"))
		{
			fprintf(stderr , "Cannot translate: Code contains variables or parameter of structions type\n");
			exit(1);
		}

	}
	else 
		return;
}

void translate_cond(struct tree_node* p , Operand label_true , Operand label_false)
{
	if(p->children_num == 2)//Exp -> NOT Exp
	{
		translate_cond(p->children[1] , label_false , label_true);
		return;
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
		translate_cond(p->children[2] , label2 , label3);

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

void translate_compst(struct tree_node* p)
{

}

