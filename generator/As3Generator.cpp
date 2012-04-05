//==============================================
/**
	@date:		2012:3:29  
	@file: 		As3Generator.cpp
	@author: 	zhangshuai
*/
//==============================================
#include "As3Generator.h"

void As3Generator::generateProgram()
{
	generateEnum();
	generateStruct();
}

void As3Generator::generateEnum()
{
	std::vector<EnumDefType*>::iterator it=program_->enums_.defs_.begin();
	std::vector<EnumDefType*>::iterator it_end=program_->enums_.defs_.end();
	while(it!=it_end)
	{
		std::string name=program_->outputDir_+(*it)->name_+".as";
		as3File_.open(name.c_str());
		as3File_<<"public class "<<(*it)->name_<<std::endl;
		as3File_<<"{"<<std::endl;
		indent_up();
		int i=0;
		std::vector<std::string>::iterator it_inner=(*it)->defs_.begin();
		while(it_inner!=(*it)->defs_.end())
		{
			as3File_<<indent()<<"public const static var "<<*it_inner<<":uint ="<<i<<";"<<std::endl;
			i++;
			++it_inner;
		}
		indent_down();
		as3File_<<"}"<<std::endl;
		as3File_.close();
		++it;
	}
}
std::string As3Generator::DefaultValue( DefType* t )
{
	if(t->is_array())
	{
		ArrayDefType* array=(ArrayDefType*)t;
		std::string temp=" new Vetcotr.<"; 
		temp=temp+typeName(array->valueDef_) +"> ";
		return temp;
	}else if(t->is_map())
	{
		MapDefType* map=(MapDefType*)t;
		std::string temp="new Object";
		return temp;
	}else if (t->is_simple_type())
	{
		SimpleDefType* s=(SimpleDefType*)t;
		switch (s->t_)
		{
		case	SimpleDefType::boolType : return "false";

		case	SimpleDefType::uint8Type : return "0";
		case	SimpleDefType::int8Type : return "0";
		case	SimpleDefType::uint16Type : return "0";
		case	SimpleDefType::int16Type : return "0";

		case	SimpleDefType::uint32Type : return "0";
		case	SimpleDefType::int32Type : return "0";

		case	SimpleDefType::int64Type : return "new Bigint";
		case	SimpleDefType::floatType : return "0";
		case	SimpleDefType::stringType : return "new String";
		default          : assert(0&&"type error"); return "";
		}
	}
	else if(t->is_struct())
	{
		std::string temp =" new ";
		temp+=t->name_;
		return temp;
	}
	else if (t->is_enum())
	{
		 return "0";
	}
	assert(0&&"type error"); 
	return "";

}

std::string As3Generator::typeName( DefType* t )
{
	if(t->is_array())
	{
		ArrayDefType* array=(ArrayDefType*)t;
		std::string temp="Vetcotr.<"; 
		temp=temp+typeName(array->valueDef_) +"> ";
		return temp;
	}else if(t->is_map())
	{
		MapDefType* map=(MapDefType*)t;
		std::string temp="Object";
		return temp;
	}else if (t->is_simple_type())
	{
		SimpleDefType* s=(SimpleDefType*)t;
		switch (s->t_)
		{
		case	SimpleDefType::boolType : return "Boolean";

		case	SimpleDefType::uint8Type : return "uint";
		case	SimpleDefType::int8Type : return "int";
		case	SimpleDefType::uint16Type : return "uint";
		case	SimpleDefType::int16Type : return "int";

		case	SimpleDefType::uint32Type : return "uint";
		case	SimpleDefType::int32Type : return "int";

		case	SimpleDefType::int64Type : return "Bigint";
		case	SimpleDefType::floatType : return "Number";
		case	SimpleDefType::stringType : return "String";
		default          : assert(0&&"type error"); return "";
		}
	}
	else if(t->is_struct())
	{
		 return t->name_;
	}
	else if (t->is_enum())
	{
		 return "int";
	}
	assert(0&&"type error"); 
	return "";

}

void As3Generator::generateStruct()
{
	std::vector<StructDefType*>::iterator it=program_->structs_.defs_.begin();
	std::vector<StructDefType*>::iterator it_end=program_->structs_.defs_.end();
	while(it!=it_end)
	{
		std::string name=program_->outputDir_+(*it)->name_+".as";
		as3File_.open(name.c_str());

		as3File_<<"public class"<<(*it)->name_<<std::endl;
		as3File_<<"{ "<<std::endl;

		//属性
		indent_up();
		as3File_<<indent()<<"//prop"<<std::endl;
		std::vector<FieldDefType*>::iterator it_inner=(*it)->members_.begin();
		while(it_inner!=(*it)->members_.end())
		{
			FieldDefType*& t=*it_inner;
			as3File_<<indent()<<"public var "<<t->name_<<":"<<typeName(t->type_)
					<<" = "<<DefaultValue(t->type_)<<";"<<std::endl;
			++it_inner;
		}

		//序列化函数
		as3File_<<std::endl;
		as3File_<<indent()<<"//serialize"<<std::endl;
		as3File_<<indent()<<"public bool serialize(IProtoclo __P__) "<<std::endl;
		as3File_<<indent()<<"{ "<<std::endl;
		indent_up();
		//序列化属性

		indent_down();
		as3File_<<indent()<<"}//serialize "<<std::endl;
		
		//反序列化函数
		as3File_<<std::endl;
		as3File_<<indent()<<"//deSerialize"<<std::endl;
		as3File_<<indent()<<"public bool deSerialize(IProtoclo __P__)"<<std::endl;
		as3File_<<indent()<<"{ "<<std::endl;
		indent_up();
		//反序列化属性

		indent_down();
		as3File_<<indent()<<"}// deSerialize"<<std::endl;

		indent_down();

		as3File_<<"}//class"<<std::endl;
		as3File_.close();
		++it;
	}
}
