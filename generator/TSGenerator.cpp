
#include"TSGenerator.h"
#include "../misc/md5.h"
TSGenerator::TSGenerator( Program* pro,const std::string& name ) :Generator(pro,name)
{

}

void TSGenerator::generateProgram()
{
	generateEnum();
	generateStruct();
	generateService();
}
void TSGenerator::generateEnum()
{
	Context*  generateContext = program_->getGenerateContext();
	if (generateContext->ns_.enums_.defs_.empty())
		return;

	for(auto& it :program_->enums_.defs_)
	{
		std::string name=program_->getOutputDir()+ it->name_+".ts";
		tsFile_.open(name.c_str());
		tsFile_<<"namespace "<<generateContext->ns_.name_<<"{"<<std::endl;
		tsFile_<<"export enum "<<it->name_<<std::endl;
		tsFile_<<"{ "<<std::endl;
		indent_up();
		for (auto& it_inner :it->defs_)
		{
			tsFile_<<indent()<<it_inner<<","<<std::endl;
		}
		indent_down();
		tsFile_<<"}//enum"<<std::endl;
		tsFile_<<"}//namespace"<<std::endl;
		tsFile_.close();
	}
}
void TSGenerator::generateStruct()
{
	Context*  generateContext = program_->getGenerateContext();
	if (generateContext->ns_.structs_.defs_.empty())
		return;

	for(auto& it :generateContext->ns_.structs_.defs_)
	{
		std::string name=program_->getOutputDir()+ it->name_+".ts";
		tsFile_.open(name.c_str());
		tsFile_<<"namespace "<<generateContext->ns_.name_<<"{"<<std::endl;
		tsFile_<<"export class "<<it->name_<<std::endl;
		tsFile_<<"{ "<<std::endl;
		indent_up();
		tsFile_<<indent()<<"public static strFingerprint:string=\""<<md5(it->getFingerPrint())<<"\""<<std::endl;
		for(auto& it_inner :it->members_)
		{
			FieldDefType*& t=it_inner;
			tsFile_<<indent()<<"public "<<t->name_<<":"<<typeName(t->type_) <<"="<<defaultValue(t->type_)<<std::endl;
		}
		indent_down();
		tsFile_<<"}//class"<<std::endl;
		tsFile_<<"}//namespace" << std::endl;

		tsFile_.close();
	}
}
void TSGenerator::generateService()
{

}

std::string TSGenerator::typeName(DefType* t,bool isAgr)
{
	if(t->is_simple_type())
	{
		SimpleDefType* s=(SimpleDefType*)t;
		switch (s->t_)
		{
			case	SimpleDefType::boolType : return "boolean";
			case	SimpleDefType::byteType : return "number";
			case	SimpleDefType::int8Type : return "number";
			case	SimpleDefType::int16Type : return "number";
			case	SimpleDefType::int32Type : return "number";
			case	SimpleDefType::int64Type : return "number";
			case	SimpleDefType::floatType : return "number";
			case	SimpleDefType::stringType : return "string";
			case   SimpleDefType::binaryType :  return "Uint8Array" ;
			default : assert(0&&"type error"); return "";
		}
	}
	else if(t->is_array())
	{
		ArrayDefType* array=(ArrayDefType*)t;
		std::string temp= typeName(array->valueDef_) +"[]";
		return temp;
	}
	else if(t->is_struct())
	{
		 return t->name_;
	}
	else if (t->is_enum())
	{
		 return t->name_;
	}
	else if(t->is_map())
	{
		MapDefType* map=(MapDefType*)t;
		std::string temp="Map<"+typeName(map->keyDef_)+","+typeName(map->valueDef_)+">";
		return temp;
	}
	assert(0&&"type error"); 
	return "";
}

std::string TSGenerator::defaultValue( DefType* t )
{
	if (t->is_simple_type())
	{
		SimpleDefType* s=(SimpleDefType*)t;
		switch (s->t_)
		{
			case SimpleDefType::boolType : return "false";
			case SimpleDefType::byteType : return "0";
			case SimpleDefType::int8Type : return "0";
			case SimpleDefType::int16Type : return "0";
			case SimpleDefType::int32Type : return "0";
			case SimpleDefType::int64Type : return "0";
			case SimpleDefType::floatType : return "0.0";
			case SimpleDefType::stringType : return "\"\"";
			case SimpleDefType::binaryType: return "null";
			default : assert(0&&"type error"); return "";
		}
	}
	else if (t->is_enum())
	{
		return "0";
	}
	else if (t->is_struct())
	{
		return "new "+t->name_+"()";
	}
	else if(t->is_array())
	{
		return "[]";
	}
	else if(t->is_map())
	{
		MapDefType* map=(MapDefType*)t;
		std::string temp="new Map<"+typeName(map->keyDef_)+","+typeName(map->valueDef_)+">()";
		return temp;
	}
	
	assert(0&&"type error"); 
	return "";

}