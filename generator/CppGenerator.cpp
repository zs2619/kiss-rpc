//==============================================
/**
	@date:		2012:3:29  
	@file: 		CppGenerator.cpp
	@author: 	zhangshuai
*/
//==============================================
#include "CppGenerator.h"

CppGenerator::CppGenerator( Program* pro,const std::string& name ) :Generator(pro,name)
{

}

void CppGenerator::generateProgram()
{
	std::string headerName=program_->outputDir_+program_->fileName_+".h";
	std::string srcName=program_->outputDir_+program_->fileName_+".cpp";
	headerFile_.open(headerName.c_str());
	srcFile_.open(srcName.c_str());
	generateEnumHeader();
	generateEnumSrc();

	generateStructHeader();
	generateStructSrc();

	headerFile_.close();
	srcFile_.close();
}

void CppGenerator::generateEnumHeader()
{
	std::vector<EnumDefType*>::iterator it=program_->enums_.defs_.begin();
	std::vector<EnumDefType*>::iterator it_end=program_->enums_.defs_.end();
	while(it!=it_end)
	{
		headerFile_<<"enum "<<(*it)->name_<<std::endl;
		headerFile_<<"{ "<<std::endl;
		std::vector<std::string>::iterator it_inner=(*it)->defs_.begin();
		indent_up();
		while(it_inner!=(*it)->defs_.end())
		{
			headerFile_<<indent()<<*it_inner<<","<<std::endl;
			++it_inner;
		}
		indent_down();
		headerFile_<<"} "<<std::endl;
		headerFile_<<"extern EnumMap enum("<<(*it)->name_<<");"<<std::endl;
		headerFile_<<std::endl;
		++it;
	}
}

void CppGenerator::generateEnumSrc()
{
	std::vector<EnumDefType*>::iterator it=program_->enums_.defs_.begin();
	std::vector<EnumDefType*>::iterator it_end=program_->enums_.defs_.end();
	while(it!=it_end)
	{
		srcFile_<<"EnumMap"<<" enum("<<(*it)->name_<<")(enum("<<(*it)->name_<<"Init));"<<std::endl;
		srcFile_<<"void "<<(*it)->name_<<"Init(EnumMap* e)"<<std::endl;
		srcFile_<<"{ "<<std::endl;
		std::vector<std::string>::iterator it_inner=(*it)->defs_.begin();
		indent_up();
		while(it_inner!=(*it)->defs_.end())
		{
			srcFile_<<indent()<<"e.addEnumValue("<<*it_inner<<")"<<";"<<std::endl;
			++it_inner;
		}
		indent_down();
		srcFile_<<"} "<<std::endl;
		srcFile_<<std::endl;
		++it;
	}
}

void CppGenerator::generateStructHeader()
{
	std::vector<StructDefType*>::iterator it=program_->structs_.defs_.begin();
	std::vector<StructDefType*>::iterator it_end=program_->structs_.defs_.end();
	while(it!=it_end)
	{
		headerFile_<<"struct "<<(*it)->name_<<std::endl;
		headerFile_<<"{ "<<std::endl;
		indent_up();
		//构造 析构函数
		headerFile_<<indent()<<(*it)->name_<<"()"<<std::endl;
		headerFile_<<indent()<<"~"<<(*it)->name_<<"()"<<std::endl;

		//属性
		std::vector<FieldDefType*>::iterator it_inner=(*it)->members_.begin();
		while(it_inner!=(*it)->members_.end())
		{
			FieldDefType*& t=*it_inner;
			defineField(t);
			++it_inner;
		}
		//序列化函数
		headerFile_<<std::endl;
		headerFile_<<indent()<<"//serialize"<<std::endl;
		headerFile_<<indent()<<"bool serialize(IProtoclo* __P__); "<<std::endl;
		
		//反序列化函数
		headerFile_<<std::endl;
		headerFile_<<indent()<<"//deSerialize"<<std::endl;
		headerFile_<<indent()<<"bool deSerialize(IProtoclo* __P__);"<<std::endl;

		indent_down();
		headerFile_<<"} //struct;"<<std::endl;
		headerFile_<<std::endl;
		++it;
	}

}

void CppGenerator::generateStructSrc()
{
	std::vector<StructDefType*>::iterator it=program_->structs_.defs_.begin();
	std::vector<StructDefType*>::iterator it_end=program_->structs_.defs_.end();
	while(it!=it_end)
	{
		//构造 析构函数
		srcFile_<<indent()<<(*it)->name_<<"::"<<(*it)->name_<<"()"<<std::endl;
		std::vector<FieldDefType*>::iterator it_inner;
		it_inner=(*it)->members_.begin();
		bool frist=true;
		while(it_inner!=(*it)->members_.end())
		{
			FieldDefType*& t=*it_inner;
			if (t->type_->is_enum()||t->type_->is_simple_type())
			{
				if (frist)
				{
					srcFile_<<":"<<t->name_<<"("<<DefaultValue(t->type_)<<")"<<std::endl;
					frist=false;
				}
				else
				{
					srcFile_<<","<<t->name_<<"("<<DefaultValue(t->type_)<<")"<<std::endl;
				}
			}
			++it_inner;
		}

		srcFile_<<"{ "<<std::endl;
		srcFile_<<"} "<<std::endl;
		srcFile_<<indent()<<(*it)->name_<<"::""~"<<(*it)->name_<<"()"<<std::endl;
		srcFile_<<"{ "<<std::endl;
		srcFile_<<"} "<<std::endl;

		//序列化函数
		srcFile_<<std::endl;
		srcFile_<<indent()<<"//serialize"<<std::endl;
		srcFile_<<indent()<<"bool "<<(*it)->name_<<"::serialize(IProtoclo* __P__) "<<std::endl;
		srcFile_<<"{ "<<std::endl;
		it_inner=(*it)->members_.begin();
		indent_up();
		while(it_inner!=(*it)->members_.end())
		{
			serializeField((*it_inner)->type_,(*it_inner)->name_);
			srcFile_<<std::endl;
			++it_inner;
		}
		srcFile_<<indent()<<"return true"<<std::endl;
		indent_down();
		srcFile_<<"}// serialize"<<std::endl;
		
		//反序列化函数
		srcFile_<<std::endl;
		srcFile_<<indent()<<"//deSerialize"<<std::endl;
		srcFile_<<indent()<<"bool "<<(*it)->name_<<"::deSerialize(IProtoclo* __P__)"<<std::endl;
		srcFile_<<"{ "<<std::endl;
		it_inner=(*it)->members_.begin();
		indent_up();
		while(it_inner!=(*it)->members_.end())
		{
			deSerializeField((*it_inner)->type_,(*it_inner)->name_);
			srcFile_<<std::endl;
			++it_inner;
		}
		srcFile_<<indent()<<"return true; "<<std::endl;
		indent_down();

		srcFile_<<"}//deSerialize "<<std::endl;

		srcFile_<<std::endl;
		++it;
	}

}

void CppGenerator::defineField( FieldDefType* t )
{
	headerFile_<<indent()<<typeName(t->type_)<<indent()<<t->name_<<";"<<std::endl;
}
std::string CppGenerator::DefaultValue( DefType* t )
{
	if (t->is_simple_type())
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

		case	SimpleDefType::int64Type : return "0";
		case	SimpleDefType::floatType : return "0.0";
		default          : assert(0&&"type error"); return "";
		}
	}else if (t->is_enum())
	{
		 return "0";
	}
	assert(0&&"type error"); 
	return "";
}

std::string CppGenerator::typeName( DefType* t )
{
	if(t->is_array())
	{
		ArrayDefType* array=(ArrayDefType*)t;
		std::string temp="std::vetcotr<"; 
		temp=temp+typeName(array->valueDef_) +"> ";
		return temp;
	}else if(t->is_map())
	{
		MapDefType* map=(MapDefType*)t;
		std::string temp="std::map<";
		temp=temp+typeName(map->keyDef_)+","+typeName(map->valueDef_)+">";
		return temp;
	}else if (t->is_simple_type())
	{
		SimpleDefType* s=(SimpleDefType*)t;
		switch (s->t_)
		{
		case	SimpleDefType::boolType : return "bool";
		case	SimpleDefType::uint8Type : return "uint8";
		case	SimpleDefType::int8Type : return "int8";
		case	SimpleDefType::uint16Type : return "uint16";
		case	SimpleDefType::int16Type : return "int16";

		case	SimpleDefType::uint32Type : return "uint32";
		case	SimpleDefType::int32Type : return "int32";

		case	SimpleDefType::int64Type : return "int46";
		case	SimpleDefType::floatType : return "float";
		case	SimpleDefType::stringType : return "std::string";
		default          : assert(0&&"type error"); return "";
		}
	}
	else if(t->is_struct())
	{
		 return t->name_;
	}
	else if (t->is_enum())
	{
		 return t->name_;
	}
	assert(0&&"type error"); 
	return "";
}

void CppGenerator::serializeField( DefType* t ,const std::string& fieldName )
{
	if (t->is_struct())
	{
		srcFile_<<indent()<<fieldName<<"->serialize(__P__);"<<std::endl;
	}
	else if (t->is_simple_type())
	{
		SimpleDefType* s=(SimpleDefType*)t;
		switch (s->t_)
		{
		case	SimpleDefType::boolType : 
			{
				srcFile_<<indent()<<"__P__->writeBool("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::uint8Type : 
			{
				srcFile_<<indent()<<"__P__->writeUint8("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::int8Type : 
			{
				srcFile_<<indent()<<"__P__->writeInt8("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::uint16Type :
			{
				srcFile_<<indent()<<"__P__->writeUInt16("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::int16Type :
			{
				srcFile_<<indent()<<"__P__->writeInt16("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::uint32Type :
			{
				srcFile_<<indent()<<"__P__->writeUInt32("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::int32Type :
			{
				srcFile_<<indent()<<"__P__->writeInt32("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::int64Type :
			{
				srcFile_<<indent()<<"__P__->writeInt64("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::floatType :
			{
				srcFile_<<indent()<<"__P__->writeFloat("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::stringType :
			{
				srcFile_<<indent()<<"__P__->writeString("<<fieldName<<");"<<std::endl;
				break;
			}

		}
	}
	else if(t->is_array())
	{
		srcFile_<<indent()<<"__P__->writeInt16("<<fieldName<<".size());"<<std::endl;
		std::string temp="_i_"+fieldName+"_";
		srcFile_<<indent()<<"for (int "<<temp<<"=0;"<<temp<<"<"<<fieldName<<".size();"<<temp<<"++)"<<std::endl;
		srcFile_<<indent()<<"{"<<std::endl;
		indent_up();
		std::string tempAgr=fieldName+"["+temp+"]";
		serializeField(((ArrayDefType*)t)->valueDef_,tempAgr);
		indent_down();
		srcFile_<<indent()<<"}"<<std::endl;

	}else if (t->is_enum())
	{
		srcFile_<<indent()<<"__P__->writeInt16("<<fieldName<<");"<<std::endl;

	}else if(t->is_map())
	{
		srcFile_<<indent()<<"__P__->writeInt16("<<fieldName<<".size());"<<std::endl;
		std::string temp="_it_"+fieldName+"_";
		srcFile_<<indent()<<typeName(t)<<"::iterator "<<temp<<" = "<<fieldName<<".begin();"<<std::endl;
		srcFile_<<indent()<<"while("<<temp<<"!="<<fieldName<<".end())"<<std::endl;
		srcFile_<<indent()<<"{"<<std::endl;
		indent_up();
		std::string tempfirst=temp+"->frist";
		std::string tempsecond=temp+"->second";
		serializeField(((MapDefType*)t)->keyDef_,tempfirst);
		serializeField(((MapDefType*)t)->valueDef_,tempsecond);
		srcFile_<<indent()<<"++"<<temp<<";"<<std::endl;
		indent_down();
		srcFile_<<indent()<<"}"<<std::endl;
	}
}

void CppGenerator::deSerializeField( DefType* t ,const std::string& fieldName )
{
	if (t->is_struct())
	{
		srcFile_<<indent()<<"if(!"<<fieldName<<"->deSerialize(__P__))return false;"<<std::endl;
	}
	else if (t->is_simple_type())
	{
		SimpleDefType* s=(SimpleDefType*)t;
		switch (s->t_)
		{
		case	SimpleDefType::boolType : 
			{
				srcFile_<<indent()<<"if(!"<<"__P__->readBool("<<fieldName<<"))return false;"<<std::endl;
				break;
			} 
		case	SimpleDefType::uint8Type : 
			{
				srcFile_<<indent()<<"if(!"<<"__P__->readUint8("<<fieldName<<"))return false;"<<std::endl;
				break;
			} 
		case	SimpleDefType::int8Type : 
			{
				srcFile_<<indent()<<"if(!"<<"__P__->readInt8("<<fieldName<<"))return false;"<<std::endl;
				break;
			} 
		case	SimpleDefType::uint16Type :
			{
				srcFile_<<indent()<<"if(!"<<"__P__->readUInt16("<<fieldName<<"))return false;"<<std::endl;
				break;
			} 
		case	SimpleDefType::int16Type :
			{
				srcFile_<<indent()<<"if(!"<<"__P__->readInt16("<<fieldName<<"))return false;"<<std::endl;
				break;
			} 
		case	SimpleDefType::uint32Type :
			{
				srcFile_<<indent()<<"if(!"<<"__P__->readUInt32("<<fieldName<<"))return false;"<<std::endl;
				break;
			} 
		case	SimpleDefType::int32Type :
			{
				srcFile_<<indent()<<"if(!"<<"__P__->readInt32("<<fieldName<<"))return false;"<<std::endl;
				break;
			} 
		case	SimpleDefType::int64Type :
			{
				srcFile_<<indent()<<"if(!"<<"__P__->readInt64("<<fieldName<<"))return false;"<<std::endl;
				break;
			} 
		case	SimpleDefType::floatType :
			{
				srcFile_<<indent()<<"if(!"<<"__P__->readFloat("<<fieldName<<"))return false;"<<std::endl;
				break;
			} 
		case	SimpleDefType::stringType :
			{
				srcFile_<<indent()<<"if(!"<<"__P__->readString("<<fieldName<<"))return false;"<<std::endl;
				break;
			}
		}
	}
	else if(t->is_array())
	{
		std::string size="_n_"+fieldName+"_array";
		srcFile_<<indent()<<"int "<<size<<"=0;"<<std::endl;
		srcFile_<<indent()<<"if(!"<<"__P__->readInt16("<<size<<"))return false;"<<std::endl;
		srcFile_<<indent()<<"fieldName.resize( "<<size<<");"<<std::endl;
		std::string count="_i_"+fieldName+"_";
		srcFile_<<indent()<<"for (int "<<count<<"=0;"<<count<<"<"<<size<<";"<<count<<"++)"<<std::endl;
		srcFile_<<indent()<<"{"<<std::endl;
		indent_up();
		std::string tempAgr="fieldName["+count+"]";
		deSerializeField(((ArrayDefType*)t)->valueDef_,tempAgr);
		indent_down();
		srcFile_<<indent()<<"}"<<std::endl;

	}else if (t->is_enum())
	{
		srcFile_<<indent()<<"if(!"<<"__P__->readInt16("<<fieldName<<"))return false;"<<std::endl;

	}else if(t->is_map())
	{
		std::string size="_n_"+fieldName+"_map_";
		srcFile_<<indent()<<"int "<<size<<"=0;"<<std::endl;
		srcFile_<<indent()<<"__P__->readInt16("<<size<<")return false;"<<std::endl;
		std::string count="_i_"+fieldName+"_";
		srcFile_<<indent()<<"for (int "<<count<<"=0;"<<count<<"<"<<size<<";"<<count<<"++)"<<std::endl;
		srcFile_<<indent()<<"{"<<std::endl;
		indent_up();
		std::string firstType=typeName(((MapDefType*)t)->keyDef_);
		std::string firstValue=" _first_map_"+fieldName;
		srcFile_<<indent()<<firstType<<" "<<firstValue<<";"<<std::endl;
		
		std::string secondType=typeName(((MapDefType*)t)->valueDef_);
		std::string secondValue=" _second_map_"+fieldName;
		srcFile_<<indent()<<secondType<<" "<<secondValue<<";"<<std::endl;

		deSerializeField(((MapDefType*)t)->keyDef_,firstValue);
		deSerializeField(((MapDefType*)t)->valueDef_,secondValue);
		srcFile_<<indent()<<fieldName<<"["<<firstValue<<"]="<< secondValue<<std::endl;
		indent_down();
		srcFile_<<indent()<<"}"<<std::endl;
	}

}
