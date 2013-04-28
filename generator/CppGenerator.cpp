//==============================================
/**
	@date:		2012:3:29  
	@file: 		CppGenerator.cpp
	@author: 	zhangshuai
*/
//==============================================
#include "CppGenerator.h"
#include "../md5.h"

CppGenerator::CppGenerator( Program* pro,const std::string& name ) :Generator(pro,name)
{

}

void CppGenerator::generateProgram()
{
	std::string headerName=program_->outputDir_+program_->baseName_+".h";
	std::string srcName=program_->outputDir_+program_->baseName_+".cpp";
	headerFile_.open(headerName.c_str());
	srcFile_.open(srcName.c_str());
	headerFile_<<"#ifndef	__"<<program_->baseName_<<"_H__"<<std::endl;
	headerFile_<<"#define	__"<<program_->baseName_<<"_H__"<<std::endl;
	headerFile_<<std::endl;
	// include 
	headerFile_<<"#include \"IProtocol.h\""<<std::endl;
	headerFile_<<"#include \"Common.h\""<<std::endl;
	genIncludeHeader(headerFile_);
	srcFile_<<"#include\""<<program_->baseName_+".h"<<"\""<<std::endl;
	headerFile_<<std::endl;

	generateEnumHeader();
	generateEnumSrc();

	generateStructHeader();
	generateStructSrc();

	generateServiceHeader();
	generateServiceSrc();
	headerFile_<<"#endif"<<std::endl;

	headerFile_.close();
	srcFile_.close();
}

void CppGenerator::generateEnumHeader()
{
	if (program_->enums_.defs_.empty())
		return;
	headerFile_<<"#include\"EnumMap.h\""<<std::endl;
	std::vector<EnumDefType*>::iterator it=program_->enums_.defs_.begin();
	std::vector<EnumDefType*>::iterator it_end=program_->enums_.defs_.end();
	while(it!=it_end)
	{
		if((*it)->fileName_!=program_->fileName_)
		{
			++it; continue; 
		}

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
		headerFile_<<"}; "<<std::endl;
		headerFile_<<"extern EnumMap Enum("<<(*it)->name_<<");"<<std::endl;
		headerFile_<<std::endl;
		++it;
	}
}

void CppGenerator::generateEnumSrc()
{
	if (program_->enums_.defs_.empty())
		return;
	std::vector<EnumDefType*>::iterator it=program_->enums_.defs_.begin();
	std::vector<EnumDefType*>::iterator it_end=program_->enums_.defs_.end();
	while(it!=it_end)
	{
		if((*it)->fileName_!=program_->fileName_)
		{
			++it; continue; 
		}
		srcFile_<<"void "<<(*it)->name_<<"Init(EnumMap* e)"<<std::endl;
		srcFile_<<"{ "<<std::endl;
		std::vector<std::string>::iterator it_inner=(*it)->defs_.begin();
		indent_up();
		while(it_inner!=(*it)->defs_.end())
		{
			srcFile_<<indent()<<"e->addEnumValue(\""<<*it_inner<<"\")"<<";"<<std::endl;
			++it_inner;
		}
		indent_down();
		srcFile_<<"} "<<std::endl;
		srcFile_<<"EnumMap"<<" Enum("<<(*it)->name_<<")("<<(*it)->name_<<"Init);"<<std::endl;
		srcFile_<<std::endl;
		++it;
	}
}

void CppGenerator::generateStructHeader()
{
	if (program_->structs_.defs_.empty())
		return;
	std::vector<StructDefType*>::iterator it=program_->structs_.defs_.begin();
	std::vector<StructDefType*>::iterator it_end=program_->structs_.defs_.end();
	while(it!=it_end)
	{
		if((*it)->fileName_!=program_->fileName_)
		{
			++it; continue; 
		}
		headerFile_<<"struct "<<(*it)->name_<<std::endl;
		headerFile_<<"{ "<<std::endl;
		indent_up();
		//构造 析构函数
		headerFile_<<indent()<<(*it)->name_<<"();"<<std::endl;
		headerFile_<<indent()<<"virtual ~"<<(*it)->name_<<"();"<<std::endl;
		//fingerprint
		headerFile_<<indent()<<"static const char* strFingerprint;"<<std::endl;

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
		headerFile_<<indent()<<"void serialize(IProtocol* __P__); "<<std::endl;
		
		//反序列化函数
		headerFile_<<std::endl;
		headerFile_<<indent()<<"//deSerialize"<<std::endl;
		headerFile_<<indent()<<"bool deSerialize(IProtocol* __P__);"<<std::endl;

		//json序列化
		if(Program::inst()->json_)
		{
			headerFile_<<std::endl;
			headerFile_<<indent()<<"//serialize json"<<std::endl;
			headerFile_<<indent()<<"void serializeJson(std::stringstream& __json__);"<<std::endl;
		}

		indent_down();
		headerFile_<<"} ;//struct"<<std::endl;
		headerFile_<<std::endl;
		++it;
	}

}

void CppGenerator::generateStructSrc()
{
	if (program_->structs_.defs_.empty())
		return;

	std::vector<StructDefType*>::iterator it=program_->structs_.defs_.begin();
	std::vector<StructDefType*>::iterator it_end=program_->structs_.defs_.end();
	while(it!=it_end)
	{
		if((*it)->fileName_!=program_->fileName_)
		{
			++it; continue; 
		}	
		//fingerprint
		srcFile_<<indent()<<"const char* "<<(*it)->name_<<"::"<<"strFingerprint=\""<<md5((*it)->getFingerPrint())<<"\";"<<std::endl;

		srcFile_<<indent()<<(*it)->name_<<"::"<<(*it)->name_<<"()"<<std::endl;
		std::vector<FieldDefType*>::iterator it_inner;
		it_inner=(*it)->members_.begin();
		bool frist=true;
		while(it_inner!=(*it)->members_.end())
		{
			FieldDefType*& t=*it_inner;
			if (t->type_->is_enum()||(t->type_->is_simple_type()&&((SimpleDefType*)(t->type_))->t_!=SimpleDefType::stringType))
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
		srcFile_<<indent()<<"void "<<(*it)->name_<<"::serialize(IProtocol* __P__) "<<std::endl;
		srcFile_<<"{ "<<std::endl;
		indent_up();
		serializeFields(*it);
		indent_down();
		srcFile_<<"}// serialize"<<std::endl;
		
		//反序列化函数
		srcFile_<<std::endl;
		srcFile_<<indent()<<"//deSerialize"<<std::endl;
		srcFile_<<indent()<<"bool "<<(*it)->name_<<"::deSerialize(IProtocol* __P__)"<<std::endl;
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

		if (Program::inst()->json_)
		{
			//序列化json
			srcFile_<<std::endl;
			srcFile_<<indent()<<"//serialize"<<std::endl;
			srcFile_<<indent()<<"void "<<(*it)->name_<<"::serializeJson(std::stringstream& __json__)"<<std::endl;
			srcFile_<<"{ "<<std::endl;
			it_inner=(*it)->members_.begin();
			indent_up();
			serializeJsonFields(*it);
			indent_down();
			srcFile_<<"}// serializeJson"<<std::endl;
		}

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
		std::string temp("(0)");
		temp=t->name_+temp;
		return temp;
	}
	assert(0&&"type error"); 
	return "";
}

std::string CppGenerator::typeName(DefType* t,bool isAgr)
{
	if(t->is_array())
	{
		ArrayDefType* array=(ArrayDefType*)t;
		std::string temp="std::vector<"; 
		temp=temp+typeName(array->valueDef_) +"> ";
		if (isAgr)
		{
			temp+="&";
		}
		return temp;
	}else if(t->is_map())
	{
		MapDefType* map=(MapDefType*)t;
		std::string temp="std::map<";
		temp=temp+typeName(map->keyDef_)+","+typeName(map->valueDef_)+">";
		if (isAgr)
		{
			temp+="&";
		}
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

		case	SimpleDefType::int64Type : return "int64";
		case	SimpleDefType::floatType : return "float";
		case	SimpleDefType::stringType : 
			{
				if (isAgr)
				{
					return "const std::string&";
				}
				else
				{
					return "std::string";
				}
			}
		default          : assert(0&&"type error"); return "";
		}
	}
	else if(t->is_struct())
	{
		if (isAgr)
		{
			return t->name_+"&";
		}
		else
		{
			return t->name_;
		}
	}
	else if (t->is_enum())
	{
		 return t->name_;
	}
	assert(0&&"type error"); 
	return "";
}

void CppGenerator::serializeJsonField( DefType* t ,const std::string& key, const std::string& value)
{
	if (t->is_struct())
	{
		srcFile_<<indent()<<value<<".serializeJson(__json__);";
	}
	else if (t->is_simple_type())
	{
		SimpleDefType* s=(SimpleDefType*)t;
		switch (s->t_)
		{
		case	SimpleDefType::boolType : 
			{
				srcFile_<<indent()<<"__json__<<\"\\\""<<key<<"\\\":\""<<"<<("<<value<<"?\"true\":\"false\")";
				break;
			} 
		case	SimpleDefType::uint8Type : 
			{
				srcFile_<<indent()<<"__json__<<\"\\\""<<key<<"\\\":\""<<"<<int32("<<value<<")";
				break;
			} 
		case	SimpleDefType::int8Type : 
			{
				srcFile_<<indent()<<"__json__<<\"\\\""<<key<<"\\\":\""<<"<<int32("<<value<<")";
				break;
			} 
		case	SimpleDefType::uint16Type :
			{
				srcFile_<<indent()<<"__json__<<\"\\\""<<key<<"\\\":\""<<"<<"<<value;
				break;
			} 
		case	SimpleDefType::int16Type :
			{
				srcFile_<<indent()<<"__json__<<\"\\\""<<key<<"\\\":\""<<"<<"<<value;
				break;
			} 
		case	SimpleDefType::uint32Type :
			{
				srcFile_<<indent()<<"__json__<<\"\\\""<<key<<"\\\":\""<<"<<"<<value;
				break;
			} 
		case	SimpleDefType::int32Type :
			{
				srcFile_<<indent()<<"__json__<<\"\\\""<<key<<"\\\":\""<<"<<"<<value;
				break;
			} 
		case	SimpleDefType::int64Type :
			{
				srcFile_<<indent()<<"__json__<<\"\\\""<<key<<"\\\":\""<<"<<"<<value;
				break;
			} 
		case	SimpleDefType::floatType :
			{
				srcFile_<<indent()<<"__json__<<\"\\\""<<key<<"\\\":\""<<"<<"<<value;
				break;
			} 
		case	SimpleDefType::stringType :
			{
				srcFile_<<indent()<<"__json__<<\"\\\""<<key<<"\\\":\\\"\""<<"<<"<<value<<"<<\"\\\"\"";
				break;
			}
		}
	}
	else if(t->is_array())
	{
		srcFile_<<indent()<<"__json__<<\"\\\""<<key<<"\\\":[\";"<<std::endl;
		indent_up();
		std::string temp="_i_"+key+"_";
		std::string tempAgr=key+"["+temp+"]";
		srcFile_<<indent()<<"for (size_t "<<temp<<"=0;"<<temp<<"<"<<key<<".size();"<<temp<<"++)"<<std::endl;
		srcFile_<<indent()<<"{"<<std::endl;
		indent_up();
		srcFile_<<indent()<<"if("<<temp<<"==("<<key<<".size()-1))"<<std::endl;
		srcFile_<<indent()<<"{"<<std::endl;
		indent_up();
		DefType* valueDef=((ArrayDefType*)t)->valueDef_;
		if (!valueDef->is_struct())
		{
			srcFile_<<indent()<<"__json__<<\""<<"{\";"<<std::endl;
		}
		serializeJsonField(valueDef,key,tempAgr);
		if (!valueDef->is_struct())
		{
			srcFile_<<";"<<std::endl;
			srcFile_<<indent()<<"__json__<<\""<<"}\";"<<std::endl;
		}
		srcFile_<<std::endl;

		indent_down();
		srcFile_<<indent()<<"}else"<<std::endl;
		srcFile_<<indent()<<"{"<<std::endl;
		indent_up();
		if (!valueDef->is_struct())
		{
			srcFile_<<indent()<<"__json__<<\""<<"{\";"<<std::endl;
		}
		serializeJsonField(valueDef,key,tempAgr);
		if (!valueDef->is_struct())
		{
			srcFile_<<";"<<std::endl;
			srcFile_<<indent()<<"__json__<<\"},\";"<<std::endl;
		}
		else
		{
			srcFile_<<std::endl<<indent()<<"__json__<<\",\";"<<std::endl;

		}
		indent_down();
		srcFile_<<indent()<<"}//if"<<std::endl;

		indent_down();
		srcFile_<<indent()<<"}//for"<<std::endl;
		indent_down();
		srcFile_<<indent()<<"__json__<<\"]\"";
	} 
	else if (t->is_enum())
	{
		srcFile_<<indent()<<"__json__<<\"\\\""<<key<<"\\\":\""<<"<<int32("<<value<<")";
	}
}
void CppGenerator::serializeField( DefType* t ,const std::string& fieldName )
{
	if (t->is_struct())
	{
		srcFile_<<indent()<<fieldName<<".serialize(__P__);"<<std::endl;
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
				srcFile_<<indent()<<"__P__->writeUInt8("<<fieldName<<");"<<std::endl;
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
		srcFile_<<indent()<<"__P__->writeUInt16("<<fieldName<<".size());"<<std::endl;
		std::string temp="_i_"+fieldName+"_";
		srcFile_<<indent()<<"for (size_t "<<temp<<"=0;"<<temp<<"<"<<fieldName<<".size();"<<temp<<"++)"<<std::endl;
		srcFile_<<indent()<<"{"<<std::endl;
		indent_up();
		std::string tempAgr=fieldName+"["+temp+"]";
		serializeField(((ArrayDefType*)t)->valueDef_,tempAgr);
		indent_down();
		srcFile_<<indent()<<"}"<<std::endl;

	}else if (t->is_enum())
	{
		srcFile_<<indent()<<"__P__->writeInt16((int16)"<<fieldName<<");"<<std::endl;

	}else if(t->is_map())
	{
		srcFile_<<indent()<<"__P__->writeUInt16("<<fieldName<<".size());"<<std::endl;
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
		srcFile_<<indent()<<"if(!"<<fieldName<<".deSerialize(__P__))return false;"<<std::endl;
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
				srcFile_<<indent()<<"if(!"<<"__P__->readUInt8("<<fieldName<<"))return false;"<<std::endl;
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
		srcFile_<<indent()<<"uint16 "<<size<<"=0;"<<std::endl;
		srcFile_<<indent()<<"if(!"<<"__P__->readUInt16("<<size<<"))return false;"<<std::endl;
		srcFile_<<indent()<<fieldName<<".resize( "<<size<<");"<<std::endl;
		std::string count="_i_"+fieldName+"_";
		srcFile_<<indent()<<"for (size_t "<<count<<"=0;"<<count<<"<"<<size<<";"<<count<<"++)"<<std::endl;
		srcFile_<<indent()<<"{"<<std::endl;
		indent_up();
		std::string tempAgr=fieldName+"["+count+"]";
		deSerializeField(((ArrayDefType*)t)->valueDef_,tempAgr);
		indent_down();
		srcFile_<<indent()<<"}"<<std::endl;

	}else if (t->is_enum())
	{
		std::string tempName=" __temp_enum__"+fieldName;
		srcFile_<<indent()<<"int16 "<<tempName<<" =0;"<<std::endl;
		srcFile_<<indent()<<"if(!"<<"__P__->readInt16((int16&)"<<tempName<<"))return false;"<<std::endl;
		srcFile_<<indent()<<fieldName<<"=("<<typeName(t)<<")"<<tempName<<";"<<std::endl;

	}else if(t->is_map())
	{
		std::string size="_n_"+fieldName+"_map_";
		srcFile_<<indent()<<"int "<<size<<"=0;"<<std::endl;
		srcFile_<<indent()<<"__P__->readUInt16("<<size<<")return false;"<<std::endl;
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

void CppGenerator::generateServiceHeader()
{
	genServiceStubHeader();
	genServiceProxyHeader();

}
void CppGenerator::generateServiceSrc()
{
	genServiceStubSrc();
	genServiceProxySrc();
}

void CppGenerator::genServiceStubHeader()
{
	if (program_->services_.defs_.empty())
		return;
	std::vector<ServiceDefType*>::iterator it=program_->services_.defs_.begin();
	std::vector<ServiceDefType*>::iterator it_end=program_->services_.defs_.end();
	while(it!=it_end)
	{
		if((*it)->fileName_!=program_->fileName_)
		{
			++it; continue; 
		}
		std::string className=(*it)->name_+"Stub";
		headerFile_<<"class "<<className<<std::endl;
		headerFile_<<"{ "<<std::endl;
		headerFile_<<"public: "<<std::endl;
		indent_up();

		//fingerprint
		headerFile_<<indent()<<"static const char* strFingerprint;"<<std::endl;

		headerFile_<<indent()<<"enum {"<<std::endl;
		indent_up();
		std::vector<FuctionDefType*>::iterator it_inner=(*it)->funs_.begin();
		while(it_inner!=(*it)->funs_.end())
		{
			FuctionDefType*& t=*it_inner;
			headerFile_<<indent()<<t->name_<<"_Id,"<<std::endl;
			++it_inner;
		}
		indent_down();
		headerFile_<<indent()<<"};"<<std::endl;

		//构造 析构函数
		headerFile_<<indent()<<className<<"(IProtocol* p=NULL):__P__(p){}"<<std::endl;
		headerFile_<<indent()<<"virtual ~"<<className<<"(){}"<<std::endl;
		//函数声明
		genFunStubDeclare(*it);

		headerFile_<<indent()<<"IProtocol* __P__;"<<std::endl;
		indent_down();
		headerFile_<<"};//class"<<std::endl;
		headerFile_<<std::endl;
		++it;
	}
}

void CppGenerator::genServiceStubSrc()
{
	if (program_->services_.defs_.empty())
		return;
	//构造 析构
	std::vector<ServiceDefType*>::iterator it=program_->services_.defs_.begin();
	std::vector<ServiceDefType*>::iterator it_end=program_->services_.defs_.end();
	while(it!=it_end)
	{
		if((*it)->fileName_!=program_->fileName_)
		{
			++it; continue; 
		}
		//fingerprint
		srcFile_<<indent()<<"const char* "<<(*it)->name_<<"Stub::"<<"strFingerprint=\""<<md5((*it)->getFingerPrint())<<"\";"<<std::endl;
		int i=0;
		std::vector<FuctionDefType*>::iterator it_inner=(*it)->funs_.begin();
		while(it_inner!=(*it)->funs_.end())
		{
			FuctionDefType*& t=*it_inner;
			srcFile_<<indent()<<"void "<<(*it)->name_<<+"Stub"<<"::"<<t->name_<<"(";
			genFunAgrList(srcFile_,t->argrs_);
			srcFile_<<")"<<std::endl;
			srcFile_<<indent()<<"{"<<std::endl;
			indent_up();
			//序列化
			srcFile_<<indent()<<"__P__->writeMsgBegin();"<<std::endl;
			srcFile_<<indent()<<"__P__->writeUInt16("<<i++<<");"<<std::endl;
			serializeFields(t->argrs_);
			srcFile_<<indent()<<"__P__->writeMsgEnd();"<<std::endl;
			indent_down();
			srcFile_<<indent()<<"}"<<std::endl;
			++it_inner;
		}
		++it;
	}
}

void CppGenerator::genServiceProxyHeader()
{
	if (program_->services_.defs_.empty())
		return;
	std::vector<ServiceDefType*>::iterator it=program_->services_.defs_.begin();
	std::vector<ServiceDefType*>::iterator it_end=program_->services_.defs_.end();
	while(it!=it_end)
	{
		if((*it)->fileName_!=program_->fileName_)
		{
			++it; continue; 
		}
		std::string className=(*it)->name_+"Proxy";
		headerFile_<<indent()<<"class "<<className<<std::endl;
		headerFile_<<indent()<<"{ "<<std::endl;
		headerFile_<<indent()<<"public: "<<std::endl;
		indent_up();

		//fingerprint
		headerFile_<<indent()<<"static const char* strFingerprint;"<<std::endl;

		//构造 析构函数
		headerFile_<<indent()<<className<<"(IProtocol* p=NULL):__P__(p){}"<<std::endl;
		headerFile_<<indent()<<"virtual ~"<<className<<"(){}"<<std::endl;
		//函数声明
		genFunProxyDeclare(*it);
		//接口
		genInterfaceDeclare(*it);
		//dispatch
		headerFile_<<indent()<<"static bool dispatch(IProtocol* p,"<<className<<"* __C__);"<<std::endl;
		headerFile_<<std::endl;
		headerFile_<<indent()<<"IProtocol* __P__;"<<std::endl;
		indent_down();
		headerFile_<<indent()<<"};//class"<<std::endl;
		headerFile_<<std::endl;
		++it;
	}

}

void CppGenerator::genServiceProxySrc()
{
	if (program_->services_.defs_.empty())
		return;
	std::vector<ServiceDefType*>::iterator it=program_->services_.defs_.begin();
	std::vector<ServiceDefType*>::iterator it_end=program_->services_.defs_.end();
	while(it!=it_end)
	{
		if((*it)->fileName_!=program_->fileName_)
		{
			++it; continue; 
		}
		//fingerprint
		srcFile_<<indent()<<"const char* "<<(*it)->name_<<"Proxy::"<<"strFingerprint=\""<<md5((*it)->getFingerPrint())<<"\";"<<std::endl;
		//dispatch
		std::string className=(*it)->name_+"Proxy";
		srcFile_<<indent()<<"bool "<<className<<"::dispatch(IProtocol* __P__,"<<className<<"* __C__)"<<std::endl;
		srcFile_<<indent()<<"{"<<std::endl;
		indent_up();
		srcFile_<<indent()<<"uint16 id=0;"<<std::endl;
		srcFile_<<indent()<<"if(!__P__->readUInt16(id)) return false;"<<std::endl;
		srcFile_<<indent()<<"switch (id)"<<std::endl;
		srcFile_<<indent()<<"{"<<std::endl;
		indent_up();
		int i=0;
		std::vector<FuctionDefType*>::iterator it_inner=(*it)->funs_.begin();
		while(it_inner!=(*it)->funs_.end())
		{
			FuctionDefType*& t=*it_inner;
			srcFile_<<indent()<<"case  "<<i++<<" :"<<std::endl;
			srcFile_<<indent()<<"{"<<std::endl;
			indent_up();
			srcFile_<<indent()<<"return recv_"<<t->name_<<"(__P__,__C__);"<<std::endl;
			indent_down();
			srcFile_<<indent()<<"}"<<std::endl;
			++it_inner;
		}
		srcFile_<<indent()<<"default:"<<std::endl;
		srcFile_<<indent()<<"{"<<std::endl;
		srcFile_<<indent()<<"	return false;"<<std::endl;
		srcFile_<<indent()<<"}"<<std::endl;
		indent_down();
		srcFile_<<indent()<<"}//switch"<<std::endl;
		indent_down();
		srcFile_<<indent()<<"}//dispatch"<<std::endl;
		//
		it_inner=(*it)->funs_.begin();
		while(it_inner!=(*it)->funs_.end())
		{
			FuctionDefType*& t=*it_inner;
			srcFile_<<indent()<<"bool "<<className<<"::"<<"recv_"<<t->name_<<"(IProtocol* __P__,"<<className<<"* __C__)"<<std::endl;
			srcFile_<<indent()<<"{"<<std::endl;
			indent_up();
			//反序列化
			deSerializeFields(t->argrs_);
			srcFile_<<indent()<<"return __C__->"<<t->name_<<"(";
			genFunAgrList(srcFile_,t->argrs_,true);
			srcFile_<<");"<<std::endl;
			indent_down();
			srcFile_<<indent()<<"}"<<std::endl;
			++it_inner;
		}
		++it;
	}
}

void CppGenerator::genFunAgrList( std::ofstream& stream,StructDefType* agrList,bool onlyValue)
{
	std::vector<FieldDefType*>::iterator it_inner=agrList->members_.begin();
	bool frist=true;
	while(it_inner!=agrList->members_.end())
	{
		FieldDefType*& t=*it_inner;
		if (frist)
		{
			if (onlyValue)
			{
				stream<<t->name_;
			}
			else
			{
				stream<<typeName(t->type_,true)<<"  "<<t->name_;
			}
			frist=false;
		}
		else
		{
			if (onlyValue)
			{
				stream<<","<<t->name_;
			}
			else
			{
				stream<<","<<typeName(t->type_,true)<<"  "<<t->name_;
			}
		}
		++it_inner;
	}
}

void CppGenerator::genFunStubDeclare( ServiceDefType* service )
{
	std::vector<FuctionDefType*>::iterator it_inner=service->funs_.begin();
	while(it_inner!=service->funs_.end())
	{
		FuctionDefType*& t=*it_inner;
		headerFile_<<indent()<<"void "<<t->name_<<"(";
		genFunAgrList(headerFile_,t->argrs_);
		headerFile_<<");"<<std::endl;
		++it_inner;
	}
}

void CppGenerator::genFunProxyDeclare( ServiceDefType* service )
{
	std::vector<FuctionDefType*>::iterator it_inner=service->funs_.begin();
	while(it_inner!=service->funs_.end())
	{
		FuctionDefType*& t=*it_inner;
		headerFile_<<indent()<<"virtual bool "<<t->name_<<"(";
		genFunAgrList(headerFile_,t->argrs_);
		headerFile_<<")=0;"<<std::endl;
		++it_inner;
	}
	std::string className=service->name_+"Proxy";
	headerFile_<<std::endl;
	it_inner=service->funs_.begin();
	while(it_inner!=service->funs_.end())
	{
		FuctionDefType*& t=*it_inner;
		headerFile_<<indent()<<"static bool "<<"recv_"<<t->name_<<"(IProtocol* p,"<<className<<"* __C__);"<<std::endl;;
		++it_inner;
	}
}

void CppGenerator::serializeFields( StructDefType* t )
{
	std::vector<FieldDefType*>::iterator it_inner=t->members_.begin();
	while(it_inner!=t->members_.end())
	{
		serializeField((*it_inner)->type_,(*it_inner)->name_);
		srcFile_<<std::endl;
		++it_inner;
	}
}
void CppGenerator::serializeJsonFields( StructDefType* t )
{
	srcFile_<<indent()<<"__json__<<\"{\";"<<std::endl;
	indent_up();
	std::vector<FieldDefType*>::iterator it_inner=t->members_.begin();
	for (size_t i=0;i<t->members_.size();i++)
	{
		FieldDefType* fdt=(t->members_)[i];
		if (i==(t->members_.size()-1))
		{
			if ((fdt->type_)->is_struct())
			{
				srcFile_<<indent()<<"__json__<<\"\\\""<<fdt->name_<<"\\\":\";"<<std::endl;
				indent_up();
			}
			serializeJsonField(fdt->type_,fdt->name_,fdt->name_);
			if ((fdt->type_)->is_struct())
			{
				indent_down();
			}
			else
			{
				srcFile_<<";"<<std::endl;
			}
		}
		else 
		{
			if ((fdt->type_)->is_struct())
			{
				srcFile_<<indent()<<"__json__<<\"\\\""<<fdt->name_<<"\\\":\";"<<std::endl;
				indent_up();
			}
			serializeJsonField(fdt->type_,fdt->name_,fdt->name_);
			if ((fdt->type_)->is_struct())
			{
				srcFile_<<std::endl<<indent()<<"__json__<<\",\";"<<std::endl;
				indent_down();
			}
			else
			{
				srcFile_<<"<<\",\";"<<std::endl;
			}
		}
	}
	indent_down();
	srcFile_<<indent()<<"__json__<<\"}\";"<<std::endl;
}


void CppGenerator::deSerializeFields( StructDefType* t )
{
	std::vector<FieldDefType*>::iterator it_inner=t->members_.begin();
	while(it_inner!=t->members_.end())
	{
		srcFile_<<indent()<<typeName((*it_inner)->type_)<<" "<<(*it_inner)->name_<<";"<<std::endl;
		deSerializeField((*it_inner)->type_,(*it_inner)->name_);
		srcFile_<<std::endl;
		++it_inner;
	}

}

void CppGenerator::genIncludeHeader( std::ofstream& stream )
{
	std::vector<std::string>::iterator it=	program_->include_.begin();
	while(it!=program_->include_.end())
	{
		size_t found=(*it).find(".");
		if (found==std::string::npos)
			found=0;
		std::string baseName=(*it).substr(0,found);
		stream<<"#include \""<<baseName<<".h\""<<std::endl;
		++it;
	}
}

void CppGenerator::genInterfaceDeclare( ServiceDefType* service )
{
	std::string IFName=program_->outputDir_+program_->baseName_+"IF.h";
	ifFile_.open(IFName.c_str());
	std::vector<FuctionDefType*>::iterator it_inner=service->funs_.begin();
	while(it_inner!=service->funs_.end())
	{
		FuctionDefType*& t=*it_inner;
		ifFile_<<indent()<<"virtual bool "<<t->name_<<"(";
		genFunAgrList(ifFile_,t->argrs_);
		ifFile_<<");"<<std::endl;
		++it_inner;
	}
	ifFile_.close();
}
