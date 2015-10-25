#include"Seman.h"

bool CheckStructTable(struct CharactInfoEntry_Struct* p);

bool IsHomoStruct(FieldList target , FieldList origin);

bool IsHomoType(Type target , Type origin);

void WriteStructTable(struct CharactInfoEntry_Struct* p , int lineNumber);

void FillFieldList(FieldList target , FieldList origin);

void FillType(Type target , Type origin);

