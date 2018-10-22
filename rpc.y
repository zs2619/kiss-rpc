%{
#include <stdio.h>
#include <string>
#include "Global.h"
#include "DefType.h"
#include "Program.h"
%}

/**
 * Base datatype keywords
*/
%token tok_bool
%token tok_uint8
%token tok_int8
%token tok_uint16
%token tok_int16
%token tok_uint32
%token tok_int32
%token tok_int64
%token tok_float
%token tok_string
%token tok_void

/**
 * Complex type keywords
 */
%token tok_enum
%token tok_struct
%token tok_array
%token tok_map

/**
 *  language keywords
 */
%token tok_service
%token tok_group

%token tok_include
%union {
	std::string*	 str_;
	float			 number_;
	DefType*		 type_;
	EnumDefType*	 enumType_;
	StructDefType*	 structType_;
	FieldDefType*	 fieldType_;
	SimpleDefType*	 simpleType_;
	MapDefType*		 mapType_;
	ArrayDefType*	 arrayType_;
	FuctionDefType*	 funType_;
	GroupDefType*	 groupType_;
	ServiceDefType*  serviceType_;
	std::vector<FuctionDefType*>* funTypes_;
	std::vector<GroupDefType*>* groupTypes_;

}

%token<str_>     tok_identifier
%token<str_>     tok_fileName

%type<str_>		 EnumValue

%type<enumType_>	EnumFieldList
%type<enumType_>	Enum

%type<structType_>		Struct
%type<structType_>		StructFieldList
%type<fieldType_>		StructField

%type<type_>		FieldType
%type<simpleType_>	SimpleDefType
%type<type_>		ContainerDefType
%type<type_>		ValueType

%type<fieldType_>	FunctionField
%type<structType_>	FunctionFieldList
%type<funType_>		Function
%type<serviceType_>	ServiceContent

%type<groupType_>	Group

%type<funTypes_ >	Functions
%type<groupTypes_ >	Groups

%type<type_ >	FunctionResult

%type<mapType_>		MapContainer
%type<arrayType_>	ArrayContainer
%type<serviceType_>	Service

%start Program

%%
Program :Header DefinitionList
		 |Header

Header:  Header HeaderDef |

HeaderDef :tok_include '\"' tok_fileName '\"'
		{
			if(Program::inst()->addIncludeFile(*$3))
			{

				switchBuf((Program::inst()->inputDir_+(*$3)).c_str());
			}
		}

DefinitionList: DefinitionList Definition|Definition

Definition:Struct|Enum|Service

Service: tok_service tok_identifier '{' ServiceContent '}' ';'
		{
			$4->name_=*$2;
			$4->fileName_=curFileName;
			if(!Program::inst()->services_.addDef($4))
			{
				yyerror("service name repeat: \"%s\"\n", (*$2).c_str());
			}
		}

ServiceContent: Groups Functions
				{
				$$=new ServiceDefType;

				$$->funs_=*$2;
				$$->groups_=*$1;

				}


Groups :Groups Group
		{
			$$->push_back($2);

		}
		|
		{
			$$=new std::vector<GroupDefType*>;
		}

Group: tok_group '{' Functions'}'
		{
			 $$=new GroupDefType;
			 $$->funs_=*$3;
		}

Functions: Functions Function 
		{
			$$->push_back($2);
		}
		|
		{
			$$=new std::vector<FuctionDefType*>;
		}

Function: FunctionResult	tok_identifier '(' FunctionFieldList ')' Separator
		{
			$$ = new FuctionDefType;
			$$->result_=$1;
			$$->name_=*$2;
			$$->argrs_=$4;

		}

FunctionResult :FieldType
				{
				$$=$1;
				}
				|tok_void
				{
				$$= new VoidDefType;
				}

FunctionFieldList: FunctionFieldList FunctionField
		{
			$$=$1;
			if(!$$->addStructValue($2))
			{
				yyerror("fun argument repeat: \"%s\"\n", (*$2).name_.c_str());
			}
		}
		|FunctionField
		{
			$$ = new StructDefType;
			if(!$$->addStructValue($1))
			{
				yyerror("fun argument repeat: \"%s\"\n", (*$1).name_.c_str());
			}
		}
		|
		{
			$$ = new StructDefType;
		}

FunctionField: FieldType tok_identifier Separator
		{
			$$= new FieldDefType;
			$$->type_=$1;
			$$->name_=*$2;
		}

Struct: tok_struct tok_identifier  '{' StructFieldList '}' ';'
		{
			$4->name_=*$2;
			$4->fileName_=curFileName;
			if(!Program::inst()->structs_.addDef($4))
			{
				yyerror("struct name repeat: \"%s\"\n", (*$2).c_str());
			}
		}

StructFieldList: StructFieldList	StructField
		 {
			$$=$1;
			if(!$$->addStructValue($2))
			{
				yyerror("struct value repeat: \"%s\"\n", (*$1).name_.c_str());
			}
		 }
		|StructField
		{
			$$ = new StructDefType;

			if(!$$->addStructValue($1))
			{
				yyerror("struct value repeat: \"%s\"\n", (*$1).name_.c_str());
			}
		}

StructField: FieldType tok_identifier Separator
		{
			$$= new FieldDefType;
			$$->type_=$1;
			$$->name_=*$2;
		}

Separator : ';'|','|


FieldType: tok_identifier 
			{
				if(Program::inst()->structs_.findDefByName(*$1))
				{
					$$= new StructDefType;
					$$->name_=*$1;
				}
				else if(Program::inst()->enums_.findDefByName(*$1))
				{
					$$= new EnumDefType;
					$$->name_=*$1;
				}
				else
				{
					yyerror("no define: \"%s\"\n", (*$1).c_str());
				}
			}
			| SimpleDefType 
			{
				$$=$1;
			}
			|ContainerDefType 
			{
				$$=$1;
			}

ContainerDefType: MapContainer 
			 {
			    $$=$1;
			 }
			 |ArrayContainer
			 {
			    $$=$1;
			 }
MapContainer:tok_map '<'SimpleDefType',' ValueType'>'
		{
			$$= new MapDefType;
			$$->keyDef_=$3;
			$$->valueDef_=$5;
		}

ArrayContainer:tok_array'<'ValueType'>'
		{
			$$= new ArrayDefType;
			$$->valueDef_= $3;
		}

ValueType: tok_identifier 
		 {
			 $$= new StructDefType;
			 $$->name_=*$1;
			 if(!Program::inst()->structs_.findDefByName($$->name_))
			 {
				yyerror("struct no define: \"%s\"\n", (*$1).c_str());
			 }
		 }
		 |SimpleDefType 
		 {
			 $$= $1;
		 }

Enum: tok_enum	tok_identifier '{'EnumFieldList'}' ';'
		{
			$4->name_=*$2;
			$4->fileName_=curFileName;
			if(!Program::inst()->enums_.addDef($4))
			{
				yyerror("enum name repeat: \"%s\"\n", (*$2).c_str());
			}
		}
EnumFieldList: EnumFieldList EnumValue 
		{
			$$=$1;
			if(!$$->addEnumValue(*$2))
			{
				yyerror("enum value repeat: \"%s\"\n", (*$2).c_str());
			}
		}
		|EnumValue
		{
			$$ = new EnumDefType;
			if(!$$->addEnumValue(*$1))
			{
				yyerror("enum value repeat: \"%s\"\n", (*$1).c_str());
			}
		}
EnumValue:tok_identifier ','
		{
			$$=$1;
		}


SimpleDefType: 
tok_bool
{
	$$= new SimpleDefType;
	$$->t_=SimpleDefType::boolType;
}
|tok_uint8
{
	$$= new SimpleDefType;
	$$->t_=SimpleDefType::uint8Type;
}
|tok_int8
{
	$$= new SimpleDefType;
	$$->t_=SimpleDefType::int8Type;
}
|tok_uint16
{
	$$= new SimpleDefType;
	$$->t_=SimpleDefType::uint16Type;
}
|tok_int16
{
	$$= new SimpleDefType;
	$$->t_=SimpleDefType::int16Type;
}
|tok_uint32
{
	$$= new SimpleDefType;
	$$->t_=SimpleDefType::uint32Type;
}
|tok_int32
{
	$$= new SimpleDefType;
	$$->t_=SimpleDefType::int32Type;
}
|tok_int64
{
	$$= new SimpleDefType;
	$$->t_=SimpleDefType::int64Type;
}
|tok_float
{
	$$= new SimpleDefType;
	$$->t_=SimpleDefType::floatType;
}
|tok_string
{
	$$= new SimpleDefType;
	$$->t_=SimpleDefType::stringType;
}

%%