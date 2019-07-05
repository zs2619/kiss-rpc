//==============================================
/**
	@date:		2012:3:29  
	@file: 		CppGenerator.cpp
	@author: 	zhangshuai
*/
//==============================================
#include "CppGenerator.h"
#include "../misc/md5.h"
#include "../misc/misc.h"

CppGenerator::CppGenerator( Program* pro,const std::string& name ) :Generator(pro,name)
{
}

void CppGenerator::generateProgram()
{
	std::string headerName=program_->getOutputDir()+program_->getBaseName()+".h";
	std::string srcName=program_->getOutputDir()+program_->getBaseName()+".cpp";
	headerFile_.open(headerName.c_str());
	srcFile_.open(srcName.c_str());
	headerFile_<<"#ifndef	__"<<misc::stringToUpper(program_->getBaseName())<<"_H__"<<std::endl;
	headerFile_<<"#define	__"<<misc::stringToUpper(program_->getBaseName())<<"_H__"<<std::endl;
	headerFile_<<std::endl;
	// include 

	Context*  generateContext = program_->getGenerateContext();
	headerFile_<<"#include <functional>"<<std::endl;
	headerFile_<<"#include \"rpc/Common.h\""<<std::endl;
	headerFile_<<"#include \"rpc/Protocol.h\""<<std::endl;
	if (!generateContext->ns_.services_.defs_.empty())
	{
		headerFile_<<"#include \"rpc/ServiceStub.h\""<<std::endl;
		headerFile_<<"#include \"rpc/ServiceProxy.h\""<<std::endl;
	}
	genIncludeHeader(headerFile_);

	headerFile_<<"namespace "<<generateContext->ns_.name_<<"{"<<std::endl;

	srcFile_<<"#include\""<<program_->getBaseName()+".h"<<"\""<<std::endl;
	headerFile_<<std::endl;

	generateEnumHeader();
	generateEnumSrc();

	generateStructHeader();
	generateStructSrc();

	generateServiceHeader();
	generateServiceSrc();
	headerFile_<<"}///namespace"<<std::endl;
	headerFile_<<"#endif"<<std::endl;

	headerFile_.close();
	srcFile_.close();
}

void CppGenerator::generateEnumHeader()
{
	Context*  generateContext = program_->getGenerateContext();
	if (generateContext->ns_.enums_.defs_.empty())
		return;

	for(auto& it :generateContext->ns_.enums_.defs_)
	{
		headerFile_<<"enum "<<it->name_<<std::endl;
		headerFile_<<"{ "<<std::endl;
		indent_up();
		for (auto& it_inner :it->defs_)
		{
			headerFile_<<indent()<<it_inner<<","<<std::endl;
		}
		indent_down();
		headerFile_<<"}; "<<std::endl;
		headerFile_<<"extern rpc::EnumMap RpcEnum("<<it->name_<<");"<<std::endl;
		headerFile_<<std::endl;
	}
}

void CppGenerator::generateEnumSrc()
{
	Context*  generateContext = program_->getGenerateContext();
	if (generateContext->ns_.enums_.defs_.empty())
		return;

	for(auto& it :generateContext->ns_.enums_.defs_)
	{
		srcFile_<<"void "<<it->name_<<"Init(rpc::EnumMap* e)"<<std::endl;
		srcFile_<<"{ "<<std::endl;
		indent_up();
		for (auto& it_inner :it->defs_)
		{
			srcFile_<<indent()<<"e->addEnumValue(\""<<it_inner<<"\")"<<";"<<std::endl;
		}
		indent_down();
		srcFile_<<"} "<<std::endl;
		srcFile_<<"rpc::EnumMap"<<" RpcEnum("<<it->name_<<")("<<it->name_<<"Init);"<<std::endl;
		srcFile_<<std::endl;
	}
}

void CppGenerator::generateStructHeader()
{
	Context*  generateContext = program_->getGenerateContext();
	if (generateContext->ns_.structs_.defs_.empty())
		return;

	for(auto& it :generateContext->ns_.structs_.defs_)
	{
		headerFile_<<"class "<<it->name_<<std::endl;
		headerFile_<<"{ "<<std::endl;
		headerFile_<<"public: "<<std::endl;
		indent_up();
		headerFile_<<indent()<<it->name_<<"();"<<std::endl;
		headerFile_<<indent()<<"virtual ~"<<it->name_<<"();"<<std::endl;
		//fingerprint
		headerFile_<<indent()<<"static const char* strFingerprint;"<<std::endl;

		for(auto& it_inner :it->members_)
		{
			FieldDefType*& t=it_inner;
			defineField(t);
		}
		headerFile_<<std::endl;
		headerFile_<<indent()<<"//serialize"<<std::endl;
		headerFile_<<indent()<<"void serialize(rpc::IProtocol* __P__) const ;"<<std::endl;
		
		headerFile_<<std::endl;
		headerFile_<<indent()<<"//deSerialize"<<std::endl;
		headerFile_<<indent()<<"bool deSerialize(rpc::IProtocol* __P__);"<<std::endl;

		if(Program::inst()->option_.json_)
		{
			headerFile_<<std::endl;
			headerFile_<<indent()<<"//serialize json"<<std::endl;
			headerFile_<<indent()<<"void serializeJson(std::stringstream& __json__);"<<std::endl;
		}

		indent_down();
		headerFile_<<"} ;//struct"<<std::endl;
		headerFile_<<std::endl;
	}
}

void CppGenerator::generateStructSrc()
{
	Context*  generateContext = program_->getGenerateContext();
	if (generateContext->ns_.structs_.defs_.empty())
		return;

	for(auto& it :generateContext->ns_.structs_.defs_)
	{
		//fingerprint
		srcFile_<<indent()<<"const char* "<<generateContext->ns_.name_<<"::"<<it->name_<<"::"<<"strFingerprint=\""<<md5(it->getFingerPrint())<<"\";"<<std::endl;

		srcFile_<<indent()<<generateContext->ns_.name_<<"::"<<it->name_<<"::"<<it->name_<<"()"<<std::endl;
		bool first=true;
		for(auto& it_inner :it->members_)
		{
			FieldDefType*& t=it_inner;
			if (t->type_->is_struct()||t->type_->is_array()||t->type_->is_map())
			{
				continue;
			}
			if (t->type_->is_simple_type())
			{
				if ( ((SimpleDefType*)(t->type_))->t_==SimpleDefType::stringType
					  ||((SimpleDefType*)(t->type_))->t_==SimpleDefType::binaryType)
				{
					continue;
				}
			}
		
			if (first)
			{
				srcFile_<<":"<<t->name_<<"("<<DefaultValue(t->type_)<<")"<<std::endl;
				first=false;
			}
			else
			{
				srcFile_<<","<<t->name_<<"("<<DefaultValue(t->type_)<<")"<<std::endl;
			}
			
		}
		srcFile_<<"{ "<<std::endl;
		srcFile_<<"} "<<std::endl;

		srcFile_<<indent()<<generateContext->ns_.name_<<"::"<<it->name_<<"::""~"<<it->name_<<"()"<<std::endl;
		srcFile_<<"{ "<<std::endl;
		srcFile_<<"} "<<std::endl;

		srcFile_<<std::endl;
		srcFile_<<indent()<<"//serialize"<<std::endl;
		srcFile_<<indent()<<"void "<<generateContext->ns_.name_<<"::"<<it->name_<<"::serialize(rpc::IProtocol* __P__) const "<<std::endl;
		srcFile_<<"{ "<<std::endl;
		indent_up();
		serializeFields(it,"__P__");
		indent_down();
		srcFile_<<"}// serialize"<<std::endl;
		
		srcFile_<<std::endl;
		srcFile_<<indent()<<"//deSerialize"<<std::endl;
		srcFile_<<indent()<<"bool "<<generateContext->ns_.name_<<"::"<<it->name_<<"::deSerialize(rpc::IProtocol* __P__)"<<std::endl;
		srcFile_<<"{ "<<std::endl;
		indent_up();
		for(auto& it_inner :it->members_)
		{
			deSerializeField(it_inner->type_,it_inner->name_,"__P__");
			srcFile_<<std::endl;
		}
		srcFile_<<indent()<<"return true; "<<std::endl;
		indent_down();

		srcFile_<<"}//deSerialize "<<std::endl;

		if (Program::inst()->option_.json_)
		{
			srcFile_<<std::endl;
			srcFile_<<indent()<<"//serialize"<<std::endl;
			srcFile_<<indent()<<"void "<<it->name_<<"::serializeJson(std::stringstream& __json__)"<<std::endl;
			srcFile_<<"{ "<<std::endl;
			indent_up();
			serializeJsonFields(it);
			indent_down();
			srcFile_<<"}// serializeJson"<<std::endl;
		}

		srcFile_<<std::endl;
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
			case SimpleDefType::boolType : return "false";
			case SimpleDefType::byteType : return "0";
			case SimpleDefType::int8Type : return "0";
			case SimpleDefType::int16Type : return "0";
			case SimpleDefType::int32Type : return "0";
			case SimpleDefType::int64Type : return "0";
			case SimpleDefType::floatType : return "0.0";
			default : assert(0&&"type error"); return "";
		}
	}
	else if (t->is_enum())
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
	}
	else if(t->is_map())
	{
		MapDefType* map=(MapDefType*)t;
		std::string temp="std::map<";
		temp=temp+typeName(map->keyDef_)+","+typeName(map->valueDef_)+">";
		if (isAgr)
		{
			temp+="&";
		}
		return temp;
	}
	else if (t->is_simple_type())
	{
		SimpleDefType* s=(SimpleDefType*)t;
		switch (s->t_)
		{
			case	SimpleDefType::boolType : return "bool";
			case	SimpleDefType::byteType : return "rpc::byte";
			case	SimpleDefType::int8Type : return "rpc::int8";
			case	SimpleDefType::int16Type : return "rpc::int16";
			case	SimpleDefType::int32Type : return "rpc::int32";
			case	SimpleDefType::int64Type : return "rpc::int64";
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
			case   SimpleDefType::binaryType : 
			{
				return "std::vector<byte>";
			}
			default : assert(0&&"type error"); return "";
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
		case	SimpleDefType::int8Type : 
			{
				srcFile_<<indent()<<"__json__<<\"\\\""<<key<<"\\\":\""<<"<<int32("<<value<<")";
				break;
			} 
		case	SimpleDefType::int16Type :
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
void CppGenerator::serializeField( DefType* t ,const std::string& fieldName,const std::string& prefix )
{
	if (t->is_struct())
	{
		srcFile_<<indent()<<fieldName<<".serialize("<<prefix<<");"<<std::endl;
	}
	else if (t->is_simple_type())
	{
		SimpleDefType* s=(SimpleDefType*)t;
		switch (s->t_)
		{
		case	SimpleDefType::boolType : 
			{
				srcFile_<<indent()<<prefix<<"->writeBool("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::binaryType : 
			{
				srcFile_<<indent()<<prefix<<"->writeBinary("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::byteType : 
			{
				srcFile_<<indent()<<prefix<<"->writeByte("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::int8Type : 
			{
				srcFile_<<indent()<<prefix<<"->writeInt8("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::int16Type :
			{
				srcFile_<<indent()<<prefix<<"->writeInt16("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::int32Type :
			{
				srcFile_<<indent()<<prefix<<"->writeInt32("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::int64Type :
			{
				srcFile_<<indent()<<prefix<<"->writeInt64("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::floatType :
			{
				srcFile_<<indent()<<prefix<<"->writeFloat("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::stringType :
			{
				srcFile_<<indent()<<prefix<<"->writeString("<<fieldName<<");"<<std::endl;
				break;
			}
		}
	}
	else if(t->is_array())
	{
		srcFile_<<indent()<<prefix<<"->writeInt32(rpc::int32("<<fieldName<<".size()));"<<std::endl;
		std::string temp="_i_"+fieldName+"_";
		srcFile_<<indent()<<"for (size_t "<<temp<<"=0;"<<temp<<"<"<<fieldName<<".size();"<<temp<<"++)"<<std::endl;
		srcFile_<<indent()<<"{"<<std::endl;
		indent_up();
		std::string tempAgr=fieldName+"["+temp+"]";
		serializeField(((ArrayDefType*)t)->valueDef_,tempAgr,prefix);
		indent_down();
		srcFile_<<indent()<<"}"<<std::endl;

	}
	else if (t->is_enum()) 
	{
		srcFile_<<indent()<<prefix<<"->writeInt16((rpc::int16)"<<fieldName<<");"<<std::endl;
	}
	else if(t->is_map()) 
	{
		srcFile_<<indent()<<prefix<<"->writeInt32(rpc::int32("<<fieldName<<".size()));"<<std::endl;
		std::string temp="_it_"+fieldName+"_";
		srcFile_<<indent()<<typeName(t)<<"::const_iterator "<<temp<<" = "<<fieldName<<".begin();"<<std::endl;
		srcFile_<<indent()<<"while("<<temp<<"!="<<fieldName<<".end())"<<std::endl;
		srcFile_<<indent()<<"{"<<std::endl;
		indent_up();
		std::string tempfirst=temp+"->first";
		std::string tempsecond=temp+"->second";
		serializeField(((MapDefType*)t)->keyDef_,tempfirst,prefix);
		serializeField(((MapDefType*)t)->valueDef_,tempsecond,prefix);
		srcFile_<<indent()<<"++"<<temp<<";"<<std::endl;
		indent_down();
		srcFile_<<indent()<<"}"<<std::endl;
	}
}

void CppGenerator::deSerializeField( DefType* t ,const std::string& fieldName ,const std::string& prefix )
{
	if (t->is_struct())
	{
		srcFile_<<indent()<<"if(!"<<fieldName<<".deSerialize("<<prefix<<"))return false;"<<std::endl;
	}
	else if (t->is_simple_type())
	{
		SimpleDefType* s=(SimpleDefType*)t;
		switch (s->t_)
		{
			case	SimpleDefType::boolType : 
			{
				srcFile_<<indent()<<"if(!"<<prefix<<"->readBool("<<fieldName<<"))return false;"<<std::endl;
				break;
			} 
			case    SimpleDefType::byteType :
			{
				srcFile_<<indent()<<"if(!"<<prefix<<"->readByte("<<fieldName<<"))return false;"<<std::endl;
				break;
			}
			case	SimpleDefType::int8Type : 
			{
				srcFile_<<indent()<<"if(!"<<prefix<<"->readInt8("<<fieldName<<"))return false;"<<std::endl;
				break;
			} 
			case	SimpleDefType::int16Type :
			{
				srcFile_<<indent()<<"if(!"<<prefix<<"->readInt16("<<fieldName<<"))return false;"<<std::endl;
				break;
			} 
			case	SimpleDefType::int32Type :
			{
				srcFile_<<indent()<<"if(!"<<prefix<<"->readInt32("<<fieldName<<"))return false;"<<std::endl;
				break;
			} 
			case	SimpleDefType::int64Type :
			{
				srcFile_<<indent()<<"if(!"<<prefix<<"->readInt64("<<fieldName<<"))return false;"<<std::endl;
				break;
			} 
			case	SimpleDefType::floatType :
			{
				srcFile_<<indent()<<"if(!"<<prefix<<"->readFloat("<<fieldName<<"))return false;"<<std::endl;
				break;
			} 
			case	SimpleDefType::stringType :
			{
				srcFile_<<indent()<<"if(!"<<prefix<<"->readString("<<fieldName<<"))return false;"<<std::endl;
				break;
			}
			case	SimpleDefType::binaryType :
			{
				srcFile_<<indent()<<"if(!"<<prefix<<"->readBinary("<<fieldName<<"))return false;"<<std::endl;
				break;
			}
		}
	}
	else if(t->is_array())
	{
		std::string size="_n_"+fieldName+"_array";
		srcFile_<<indent()<<"rpc::int32 "<<size<<"=0;"<<std::endl;
		srcFile_<<indent()<<"if(!"<<prefix<<"->readInt32("<<size<<"))return false;"<<std::endl;
		srcFile_<<indent()<<fieldName<<".resize( "<<size<<");"<<std::endl;
		std::string count="_i_"+fieldName+"_";
		srcFile_<<indent()<<"for (rpc::int32 "<<count<<"=0;"<<count<<"<"<<size<<";"<<count<<"++)"<<std::endl;
		srcFile_<<indent()<<"{"<<std::endl;
		indent_up();
		std::string tempAgr=fieldName+"["+count+"]";
		deSerializeField(((ArrayDefType*)t)->valueDef_,tempAgr,prefix);
		indent_down();
		srcFile_<<indent()<<"}"<<std::endl;

	}
	else if (t->is_enum())
	{
		std::string tempName=" __temp_enum__"+fieldName;
		srcFile_<<indent()<<"rpc::int16 "<<tempName<<" =0;"<<std::endl;
		srcFile_<<indent()<<"if(!"<<prefix<<"->readInt16((rpc::int16&)"<<tempName<<"))return false;"<<std::endl;
		srcFile_<<indent()<<fieldName<<"=("<<typeName(t)<<")"<<tempName<<";"<<std::endl;

	}
	else if(t->is_map())
	{
		std::string size="_n_"+fieldName+"_map_";
		srcFile_<<indent()<<"rpc::int32 "<<size<<"=0;"<<std::endl;
		srcFile_<<indent()<<"if(!"<<prefix<<"->readInt32("<<size<<"))return false;"<<std::endl;
		std::string count="_i_"+fieldName+"_";
		srcFile_<<indent()<<"for (rpc::int32 "<<count<<"=0;"<<count<<"<"<<size<<";"<<count<<"++)"<<std::endl;
		srcFile_<<indent()<<"{"<<std::endl;
		indent_up();
		std::string firstType=typeName(((MapDefType*)t)->keyDef_);
		std::string firstValue=" _first_map_"+fieldName;
		srcFile_<<indent()<<firstType<<" "<<firstValue<<";"<<std::endl;
		
		std::string secondType=typeName(((MapDefType*)t)->valueDef_);
		std::string secondValue=" _second_map_"+fieldName;
		srcFile_<<indent()<<secondType<<" "<<secondValue<<";"<<std::endl;

		deSerializeField(((MapDefType*)t)->keyDef_,firstValue,prefix);
		deSerializeField(((MapDefType*)t)->valueDef_,secondValue,prefix);
		srcFile_<<indent()<<fieldName<<"["<<firstValue<<"]="<< secondValue<<";"<<std::endl;
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
	Context*  generateContext = program_->getGenerateContext();
	if (generateContext->ns_.services_.defs_.empty())
		return;

	for(auto& it:generateContext->ns_.services_.defs_)
	{
		std::string className=it->name_+"Stub";
		headerFile_<<"class "<<className<<": public rpc::ServiceStub"<<std::endl;
		headerFile_<<"{ "<<std::endl;
		headerFile_<<"public: "<<std::endl;
		indent_up();

		//fingerprint
		headerFile_<<indent()<<"static const char* strFingerprint;"<<std::endl;
		headerFile_<<indent()<<"static const char* getObjName;"<<std::endl;

		headerFile_<<indent()<<"enum {"<<std::endl;
		indent_up();

		for(auto& it_inner:it->funs_)
		{
			FuctionDefType*& t=it_inner;
			headerFile_<<indent()<<t->name_<<"_Id,"<<std::endl;
		}
		indent_down();
		headerFile_<<indent()<<"};"<<std::endl;

		headerFile_<<indent()<<className<<"(const rpc::Connection* conn):ServiceStub(conn){}"<<std::endl;
		headerFile_<<indent()<<"virtual ~"<<className<<"(){}"<<std::endl;

		headerFile_<<indent()<< "int invokeAsync(rpc::int16 msgId,const rpc::IProtocol* p,"<<"const std::string& functionName);"<<std::endl;
		headerFile_<<indent()<< "virtual bool dispatch(std::shared_ptr<rpc::RpcMsg> msg);" <<std::endl;

		genFunStubDeclare(it);

		indent_down();
		headerFile_<<"};//class"<<std::endl;
		headerFile_<<std::endl;
	}
}

void CppGenerator::genServiceStubSrc()
{
	Context*  generateContext = program_->getGenerateContext();
	if (generateContext->ns_.services_.defs_.empty())
		return;

	for(auto& it:generateContext->ns_.services_.defs_)
	{
		//fingerprint
		srcFile_<<indent()<<"const char* "<<generateContext->ns_.name_<<"::"<<it->name_<<"Stub::"<<"strFingerprint=\""<<md5(it->getFingerPrint())<<"\";"<<std::endl;
		srcFile_<<indent()<<"const char* "<<generateContext->ns_.name_<<"::"<<it->name_<<"Stub::"<<"getObjName=\""<<generateContext->ns_.name_<<"."<<it->name_<<"\";"<<std::endl;
		srcFile_ <<indent()<< "int  "<<generateContext->ns_.name_<<"::"<<it->name_
			<<"Stub::"<<"invokeAsync(rpc::int16 msgId,const rpc::IProtocol* p ,const std::string& functionName) {" << std::endl;
		indent_up();
		srcFile_ <<indent()<<"std::shared_ptr<rpc::RpcMsg> msg = std::make_shared<rpc::RpcMsg>();" << std::endl;
		srcFile_ <<indent()<<"msg->serviceName_= getObjName ;" << std::endl;
		srcFile_ <<indent()<<"msg->functionName_= functionName ;" << std::endl;
		srcFile_ <<indent()<<"msg->requestMsg_.msgId = msgId;"<<std::endl;
		srcFile_ <<indent()<<"msg->requestMsg_.buff = p->getBuffer();"<<std::endl;
		srcFile_ <<indent()<<"return invoke(msg);" << std::endl;
		indent_down();
		srcFile_ <<indent()<< "}" << std::endl;

		srcFile_ <<indent()<< "bool  "<<generateContext->ns_.name_<<"::"<<it->name_<<"Stub::"<<"dispatch(std::shared_ptr<rpc::RpcMsg> msg) {" << std::endl;
		indent_up();
		srcFile_ << indent() << "auto id=msg->responseMsg_.msgId;" << std::endl;
		srcFile_ << indent() << "switch (id)" << std::endl;
		srcFile_ << indent() << "{" << std::endl;
		int i = 0;
		indent_up();
		for(auto& it_inner:it->funs_)
		{
			FuctionDefType*& t = it_inner;
			if (!t->result_->is_void())
			{
				srcFile_ << indent() << "case "<<i<<":"<< std::endl;
				srcFile_ << indent() << "{" << std::endl;
				indent_up();
				srcFile_ << indent() << "std::unique_ptr<rpc::IProtocol> __P__(chan_->getProtocol()->createProtoBuffer());" << std::endl;
				srcFile_ << indent() << "__P__->setBuffer(msg->responseMsg_.buff);" << std::endl;
				srcFile_ << indent() << typeName(t->result_) << " ret ;" << std::endl;

				deSerializeField(t->result_, "ret","__P__.get()");

				srcFile_ << indent() << "int cbRet = "<<t->name_<<"CallBack(ret);" << std::endl;
				srcFile_ << indent() << "return true;" << std::endl;
				indent_down();
				srcFile_ << indent() << "}" << std::endl;
			}
			i++;
		}

		srcFile_ << indent() << "default:" << std::endl;
		srcFile_ << indent() << "{" << std::endl;
		indent_up();
		srcFile_ << indent() << "return false;" << std::endl;
		indent_down();
		srcFile_ << indent() << "}" << std::endl;
		indent_down();
		srcFile_ << indent() << "}" << std::endl;

		indent_down();
		srcFile_ <<indent()<< "}" << std::endl;

		i=0;
		for(auto& it_inner:it->funs_)
		{
			FuctionDefType*& t=it_inner;
			srcFile_<<indent()<<"int "<<generateContext->ns_.name_<<"::"<<it->name_<<+"Stub"<<"::"<<t->name_<<"(";
			genFunAgrList(srcFile_,t->argrs_);
			if (!t->result_->is_void())
			{
				if (!t->argrs_->members_.empty()) {
					srcFile_ << ",";
				}
				srcFile_ <<"std::function<int("<<typeName(t->result_)<<")> cb";
			}
			srcFile_<<")"<<std::endl;
			srcFile_<<indent()<<"{"<<std::endl;
			indent_up();
			srcFile_<<indent()<<"std::unique_ptr<rpc::IProtocol> __P__(chan_->getProtocol()->createProtoBuffer());"<<std::endl;
			serializeFields(t->argrs_,"__P__.get()");

			if (!t->result_->is_void())
			{
				srcFile_ << indent() <<t->name_<< "CallBack = cb;" << std::endl;
			}
			srcFile_<<indent()<<"return invokeAsync("<<i<<",__P__.get()"<<",\"" << t->name_<<"\");"<<std::endl;
			indent_down();

			srcFile_<<indent()<<"}"<<std::endl;
			i++;
		}
	}
}

void CppGenerator::genServiceProxyHeader()
{
	Context*  generateContext = program_->getGenerateContext();
	if (generateContext->ns_.services_.defs_.empty())
		return;

	for (auto& it : generateContext->ns_.services_.defs_)
	{
		std::string className=it->name_+"ProxyIF";
		headerFile_<<indent()<<"class "<<className<<": public rpc::ServiceProxy "<<std::endl;
		headerFile_<<indent()<<"{ "<<std::endl;
		headerFile_<<indent()<<"public: "<<std::endl;
		indent_up();

		//fingerprint
		headerFile_<<indent()<<"static const char* strFingerprint;"<<std::endl;
		headerFile_<<indent()<<"static const char* getObjName;"<<std::endl;

		headerFile_<<indent()<<className<<"(const rpc::Connection* conn):ServiceProxy(conn){}"<<std::endl;
		headerFile_<<indent()<<"virtual ~"<<className<<"(){}"<<std::endl;
		genFunProxyDeclare(it);
		//dispatch
		headerFile_<<indent()<<"virtual bool dispatch(std::shared_ptr<rpc::RpcMsg> msg);"<<std::endl;
		headerFile_<<std::endl;
		indent_down();
		headerFile_<<indent()<<"};//class"<<std::endl;
		headerFile_<<std::endl;
	}

}

void CppGenerator::genServiceProxySrc()
{
	Context*  generateContext = program_->getGenerateContext();
	if (generateContext->ns_.services_.defs_.empty())
		return;

	for (auto& it : generateContext->ns_.services_.defs_)
	{
		//fingerprint
		srcFile_<<indent()<<"const char* "<<generateContext->ns_.name_<<"::"<<(it)->name_<<"ProxyIF::"<<"strFingerprint=\""<<md5((it)->getFingerPrint())<<"\";"<<std::endl;
		srcFile_<<indent()<<"const char* "<<generateContext->ns_.name_<<"::"<<(it)->name_<<"ProxyIF::"<<"getObjName=\""<<generateContext->ns_.name_ <<"."<< (it)->name_<<"\";"<<std::endl;
		//dispatch
		std::string className=(it)->name_+"ProxyIF";
		srcFile_<<indent()<<"bool "<<generateContext->ns_.name_<<"::"<<className<<"::dispatch(std::shared_ptr<rpc::RpcMsg> msg)"<<std::endl;
		srcFile_<<indent()<<"{"<<std::endl;
		indent_up();
		srcFile_<<indent()<<"auto id=msg->requestMsg_.msgId;"<<std::endl;
		srcFile_<<indent()<< "msg->serviceName_=getObjName;" << std::endl;
		srcFile_<<indent()<<"switch (id)"<<std::endl;
		srcFile_<<indent()<<"{"<<std::endl;
		indent_up();
		int i=0;
		std::vector<FuctionDefType*>::iterator it_inner=(it)->funs_.begin();
		while(it_inner!=(it)->funs_.end())
		{
			FuctionDefType*& t=*it_inner;
			srcFile_<<indent()<<"case  "<<i++<<" :"<<std::endl;
			srcFile_<<indent()<<"{"<<std::endl;
			indent_up();

			srcFile_ << indent() << "std::unique_ptr<rpc::IProtocol> __P__(service_->getProtocol()->createProtoBuffer());" << std::endl;
			srcFile_ << indent() << "__P__->setBuffer(msg->requestMsg_.buff);"<<std::endl;

			deSerializeFields(t->argrs_,"__P__.get()");
			srcFile_ << indent() << "auto result = " << t->name_ << "(";
			genFunAgrList(srcFile_, t->argrs_, PT_Value);
			srcFile_ <<  ");"<<std::endl;

			if (!t->result_->is_void())
			{
				srcFile_ << indent() << "if (std::get<0>(result)==0)"<<std::endl;
				srcFile_ << indent() <<"{"<<std::endl;
				indent_up();
				srcFile_ << indent() << "std::unique_ptr<rpc::IProtocol> __P__(service_->getProtocol()->createProtoBuffer());" << std::endl;
				srcFile_ << indent() <<typeName(t->result_)<<" ret="<<"std::get<1>(result);"<<std::endl;
				serializeField(t->result_, "ret","__P__.get()");
				srcFile_ << indent() << "msg->responseMsg_.buff=__P__->getBuffer();"<<std::endl;
				srcFile_ << indent() << "invoke(msg);"<<std::endl;
				indent_down();
				srcFile_ << indent() <<"}"<<std::endl;
				srcFile_ << indent() << "return true;" << std::endl;
			}

			indent_down();
			srcFile_<<indent()<<"}"<<std::endl;
			++it_inner;
		}
		srcFile_<<indent()<<"default:"<<std::endl;
		srcFile_<<indent()<<"{"<<std::endl;
		indent_up();
		srcFile_<<indent()<<"return false;"<<std::endl;
		indent_down();
		srcFile_<<indent()<<"}"<<std::endl;
		indent_down();
		srcFile_<<indent()<<"}//switch"<<std::endl;
		indent_down();
		srcFile_<<indent()<<"}//dispatch"<<std::endl;
		//
		++it;
	}
}

void CppGenerator::genFunAgrList( std::ofstream& stream,StructDefType* agrList,ParamType paramType)
{
	bool first=true;
	for (auto& it_inner:agrList->members_)
	{
		FieldDefType*& t=it_inner;
		if (!first)
		{
			stream<<",";
		}
		if (paramType==PT_Value)
		{
			stream<<t->name_;
		}
		else if(paramType==PT_TypeValue)
		{
			stream<<typeName(t->type_,true)<<"  "<<t->name_;
		}
		else if (paramType==PT_Type) 
		{
			stream<<typeName(t->type_,true);
		}
		first = false;
	}
}

void CppGenerator::genFunStubDeclare( ServiceDefType* service )
{
	for (auto& it_inner:service->funs_)
	{
		FuctionDefType*& t=it_inner;
		headerFile_<<indent()<<"int "<<t->name_<<"(";
		genFunAgrList(headerFile_,t->argrs_);
		if (!t->result_->is_void())
		{
			if (!t->argrs_->members_.empty()) {
				headerFile_ << ",";
			}
			headerFile_ <<"std::function<int("<<typeName(t->result_)<<")> cb";
		}
		headerFile_<<");"<<std::endl;
	}
	headerFile_ << std::endl;
	headerFile_<<"protected:"<<std::endl;

	for (auto& it_inner:service->funs_)
	{
		FuctionDefType*& t=it_inner;
		if (!t->result_->is_void())
		{
			headerFile_ << indent() << "std::function<int(" << typeName(t->result_) << ")> " << t->name_ << "CallBack;" << std::endl;
		}
	}
}

void CppGenerator::genFunProxyDeclare( ServiceDefType* service )
{
	std::vector<FuctionDefType*>::iterator it_inner=service->funs_.begin();
	while(it_inner!=service->funs_.end())
	{
		FuctionDefType*& t=*it_inner;
		headerFile_ << indent() << "virtual std::tuple<int";
		if (t->result_->is_void())
		{
			headerFile_ << ">";
		}
		else 
		{
			headerFile_ <<","<<typeName(t->result_)<< ">";
		}
		headerFile_<<t->name_<<"(";
		genFunAgrList(headerFile_,t->argrs_);
		headerFile_<<")=0;"<<std::endl;
		++it_inner;
	}
}

void CppGenerator::serializeFields( StructDefType* t ,const std::string& prefix)
{
	std::vector<FieldDefType*>::iterator it_inner=t->members_.begin();
	while(it_inner!=t->members_.end())
	{
		serializeField((*it_inner)->type_,(*it_inner)->name_,prefix);
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


void CppGenerator::deSerializeFields( StructDefType* t ,const std::string& prefix)
{
	std::vector<FieldDefType*>::iterator it_inner=t->members_.begin();
	while(it_inner!=t->members_.end())
	{
		srcFile_<<indent()<<typeName((*it_inner)->type_)<<" "<<(*it_inner)->name_<<";"<<std::endl;
		deSerializeField((*it_inner)->type_,(*it_inner)->name_,prefix);
		srcFile_<<std::endl;
		++it_inner;
	}

}

void CppGenerator::genIncludeHeader( std::ofstream& stream )
{
	std::vector<std::string>::iterator it=	program_->include_.begin();
	while(it!=program_->include_.end())
	{
		stream<<"#include \""<<*it<<".h\""<<std::endl;
		++it;
	}
}


