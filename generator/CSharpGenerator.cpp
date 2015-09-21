//==============================================
/**
	@date:		2014:7:23  
	@file: 		CSharpGenerator.h
	@author		tanlei
*/
//==============================================
#include "CSharpGenerator.h"
#include "../md5.h"

void CSharpGenerator::generateProgram()
{
	std::string name=program_->outputDir_+program_->baseName_+".cs";
	csharpFile_.open(name.c_str());
	csharpFile_<<indent()<<"using UnityEngine;"<<std::endl;
	csharpFile_<<indent()<<"using System;"<<std::endl;
	csharpFile_<<indent()<<"using System.Collections.Generic;"<<std::endl;
	csharpFile_<<indent()<<"namespace ComRpc"<<std::endl;
	csharpFile_<<indent()<<"{"<<std::endl;

	generateEnum();
	generateStruct();
	generateService();

	indent_down();
	csharpFile_<<indent()<<"}//namespace"<<std::endl;
	csharpFile_.close();
}

void CSharpGenerator::generateEnum()
{
	std::vector<EnumDefType*>::iterator it=program_->enums_.defs_.begin();
	std::vector<EnumDefType*>::iterator it_end=program_->enums_.defs_.end();
	while(it!=it_end)
	{
		if((*it)->fileName_!=program_->fileName_)
		{
			++it; continue; 
		}
		indent_up();
		csharpFile_<<indent()<<"public enum "<<(*it)->name_<<std::endl;
		csharpFile_<<indent()<<"{"<<std::endl;
		indent_up();
		std::vector<std::string>::iterator it_inner=(*it)->defs_.begin();
		while(it_inner!=(*it)->defs_.end())
		{
			csharpFile_<<indent()<<*it_inner<<","<<std::endl;
			++it_inner;
		}
		indent_down();
		csharpFile_<<indent()<<"}"<<std::endl;
		++it;
		indent_down();
		csharpFile_<<std::endl;
	}
}

void CSharpGenerator::generateStruct()
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
		indent_up();
		
		csharpFile_<<indent()<<"public class "<<(*it)->name_<<std::endl;
		csharpFile_<<indent()<<"{ "<<std::endl;
		indent_up();
		//fingerprint
		csharpFile_<<indent()<<"public const string strFingerprint=\""<<md5((*it)->getFingerPrint())<<"\";"<<std::endl;

		//属性
		std::vector<FieldDefType*>::iterator it_inner=(*it)->members_.begin();
		while(it_inner!=(*it)->members_.end())
		{
			FieldDefType*& t=*it_inner;
			csharpFile_<<indent()<<"public "<<typeName(t->type_)<<" "<<t->name_
				<<" = "<<DefaultValue(t->type_)<<";"<<std::endl;
			++it_inner;
		}
		//序列化函数
		csharpFile_<<std::endl;
		csharpFile_<<indent()<<"//serialize"<<std::endl;
		csharpFile_<<indent()<<"public bool  serialize(IProtocol __P__) "<<std::endl;
		csharpFile_<<indent()<<"{ "<<std::endl;
		indent_up();
		//序列化属性
		it_inner=(*it)->members_.begin();
		while(it_inner!=(*it)->members_.end())
		{
			serializeField((*it_inner)->type_,(*it_inner)->name_);
			csharpFile_<<std::endl;
			++it_inner;
		}
		csharpFile_<<indent()<<"return true;"<<std::endl;
		indent_down();
		csharpFile_<<indent()<<"}//serialize "<<std::endl;

		//反序列化函数
		csharpFile_<<std::endl;
		csharpFile_<<indent()<<"//deSerialize"<<std::endl;
		csharpFile_<<indent()<<"public bool deSerialize(IProtocol __P__)"<<std::endl;
		csharpFile_<<indent()<<"{ "<<std::endl;
		indent_up();
		//反序列化属性
		it_inner=(*it)->members_.begin();
		while(it_inner!=(*it)->members_.end())
		{
			deSerializeField((*it_inner)->type_,(*it_inner)->name_);
			csharpFile_<<std::endl;
			++it_inner;
		}
		csharpFile_<<indent()<<"return true;"<<std::endl;
		indent_down();
		csharpFile_<<indent()<<"}// deSerialize"<<std::endl;

		indent_down();
		csharpFile_<<indent()<<"}//class"<<std::endl;
		csharpFile_<<std::endl;
		indent_down();
		++it;
	}
}
std::string CSharpGenerator::DefaultValue( DefType* t )
{
	if(t->is_array())
	{
		ArrayDefType* array=(ArrayDefType*)t;
		std::string temp=" new List<";
		temp = temp+typeName(array->valueDef_) +">()";
		return temp;
	}else if(t->is_map())
	{
		MapDefType* map=(MapDefType*)t;
		std::string temp="new Dictionary<";
		temp = temp +typeName(map->keyDef_)+", "+typeName(map->valueDef_)+">()";
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

		case	SimpleDefType::int64Type : return "0";
		case	SimpleDefType::floatType : return "0.0f";
		case	SimpleDefType::stringType : return "\"\"";
		default          : assert(0&&"type error"); return "";
		}
	}
	else if(t->is_struct())
	{
		std::string temp =" new ";
		temp+=t->name_+"()";
		return temp;
	}
	else if (t->is_enum())
	{
		return "0";
	}
	assert(0&&"type error"); 
	return "";
}

std::string CSharpGenerator::typeName( DefType* t )
{
	if(t->is_array())
	{
		ArrayDefType* array=(ArrayDefType*)t;
		std::string temp = "List<";
		temp = temp+typeName(array->valueDef_) +">";
		return temp;
	}else if(t->is_map())
	{
		MapDefType* map=(MapDefType*)t;
		std::string temp="Dictionary<";
		temp=temp+typeName(map->keyDef_)+","+typeName(map->valueDef_)+">";
		return temp;
	}else if (t->is_simple_type())
	{
		SimpleDefType* s=(SimpleDefType*)t;
		switch (s->t_)
		{
		case	SimpleDefType::boolType : return "bool";

		case	SimpleDefType::uint8Type : return "byte";
		case	SimpleDefType::int8Type : return "sbyte";
		case	SimpleDefType::uint16Type : return "ushort";
		case	SimpleDefType::int16Type : return "short";

		case	SimpleDefType::uint32Type : return "uint";
		case	SimpleDefType::int32Type : return "int";

		case	SimpleDefType::int64Type : return "long";
		case	SimpleDefType::floatType : return "float";
		case	SimpleDefType::stringType : return "string";
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

void CSharpGenerator::serializeField( DefType* t ,const std::string& fieldName )
{
	if (t->is_struct())
	{
		csharpFile_<<indent()<<fieldName<<".serialize(__P__);"<<std::endl;
	}
	else if (t->is_simple_type())
	{
		SimpleDefType* s=(SimpleDefType*)t;
		switch (s->t_)
		{
		case	SimpleDefType::boolType : 
			{
				csharpFile_<<indent()<<"__P__.writeBool("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::uint8Type : 
			{
				csharpFile_<<indent()<<"__P__.writeUInt8("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::int8Type : 
			{
				csharpFile_<<indent()<<"__P__.writeInt8("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::uint16Type :
			{
				csharpFile_<<indent()<<"__P__.writeUInt16("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::int16Type :
			{
				csharpFile_<<indent()<<"__P__.writeInt16("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::uint32Type :
			{
				csharpFile_<<indent()<<"__P__.writeUInt32("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::int32Type :
			{
				csharpFile_<<indent()<<"__P__.writeInt32("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::int64Type :
			{
				csharpFile_<<indent()<<"__P__.writeInt64("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::floatType :
			{
				csharpFile_<<indent()<<"__P__.writeFloat("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::stringType :
			{
				csharpFile_<<indent()<<"__P__.writeString("<<fieldName<<");"<<std::endl;
				break;
			}

		}
	}
	else if(t->is_array())
	{
		csharpFile_<<indent()<<"__P__.writeUInt16((ushort)"<<fieldName<<".Count );"<<std::endl;
		std::string temp="_i_"+fieldName+"_";
		csharpFile_<<indent()<<"for (int  "<<temp<<"=0;"<<temp<<"<"<<fieldName<<".Count;"<<temp<<"++)"<<std::endl;
		csharpFile_<<indent()<<"{"<<std::endl;
		indent_up();
		std::string tempAgr=fieldName+"["+temp+"]";
		serializeField(((ArrayDefType*)t)->valueDef_,tempAgr);
		indent_down();
		csharpFile_<<indent()<<"}"<<std::endl;

	}else if (t->is_enum())
	{
		csharpFile_<<indent()<<"__P__.writeInt16((short)"<<fieldName<<");"<<std::endl;

	}else if(t->is_map())
	{
		//todo map
	}
}

void CSharpGenerator::deSerializeField( DefType* t ,const std::string& fieldName )
{
	if (t->is_struct())
	{
		csharpFile_<<indent()<<fieldName<<".deSerialize(__P__);"<<std::endl;
	}
	else if (t->is_simple_type())
	{

		SimpleDefType* s=(SimpleDefType*)t;
		switch (s->t_)
		{
		case	SimpleDefType::boolType : 
			{
				csharpFile_<<indent()<<fieldName<<"=__P__.readBool() ;"<<std::endl;
				break;
			} 
		case	SimpleDefType::uint8Type : 
			{
				csharpFile_<<indent()<<fieldName<<"=__P__.readUInt8();"<<std::endl;
				break;
			} 
		case	SimpleDefType::int8Type : 
			{
				csharpFile_<<indent()<<fieldName<<"=__P__.readInt8();"<<std::endl;
				break;
			} 
		case	SimpleDefType::uint16Type :
			{
				csharpFile_<<indent()<<fieldName<<"=__P__.readUInt16();"<<std::endl;
				break;
			} 
		case	SimpleDefType::int16Type :
			{
				csharpFile_<<indent()<<fieldName<<"=__P__.readInt16();"<<std::endl;
				break;
			} 
		case	SimpleDefType::uint32Type :
			{
				csharpFile_<<indent()<<fieldName<<"=__P__.readUInt32();"<<std::endl;
				break;
			} 
		case	SimpleDefType::int32Type :
			{
				csharpFile_<<indent()<<fieldName<<"=__P__.readInt32();"<<std::endl;
				break;
			} 
		case	SimpleDefType::int64Type :
			{
				csharpFile_<<indent()<<fieldName<<"=__P__.readInt64();"<<std::endl;
				break;
			} 
		case	SimpleDefType::floatType :
			{
				csharpFile_<<indent()<<fieldName<<"=__P__.readFloat();"<<std::endl;
				break;
			} 
		case	SimpleDefType::stringType :
			{
				csharpFile_<<indent()<<fieldName<<"=__P__.readString();"<<std::endl;
				break;
			}
		}
	}
	else if(t->is_array())
	{
		std::string size="_n_"+fieldName+"_array";
		csharpFile_<<indent()<<"int "<<size<<"=0;"<<std::endl;
		csharpFile_<<indent()<<size<<"=__P__.readUInt16();"<<std::endl;
		std::string count="_i_"+fieldName+"_";
		csharpFile_<<indent()<<"for (int "<<count<<"=0;"<<count<<"<"<<size<<";"<<count<<"++)"<<std::endl;
		csharpFile_<<indent()<<"{"<<std::endl;
		indent_up();
		std::string valueStr="temp_array_"+fieldName;
		std::string tempAgr=typeName(((ArrayDefType*)t)->valueDef_)+" "+valueStr
			+"="+DefaultValue(((ArrayDefType*)t)->valueDef_)+";";
		csharpFile_<<indent()<<tempAgr<<std::endl;
		deSerializeField(((ArrayDefType*)t)->valueDef_,valueStr);
		csharpFile_<<indent()<<fieldName<<".Add("<<valueStr<<");"<<std::endl;
		indent_down();
		csharpFile_<<indent()<<"}"<<std::endl;

	}else if (t->is_enum())
	{
		csharpFile_<<indent()<<fieldName<<"=("<<t->name_<<")__P__.readInt16();"<<std::endl;

	}else if(t->is_map())
	{
		//todo map
	}
}

void CSharpGenerator::generateService()
{
	genServiceStub();
	genServiceProxy();
	genServiceProxyIf();
}

void CSharpGenerator::genServiceStub()
{
	std::vector<ServiceDefType*>::iterator it=program_->services_.defs_.begin();
	std::vector<ServiceDefType*>::iterator it_end=program_->services_.defs_.end();
	while(it!=it_end)
	{
		if((*it)->fileName_!=program_->fileName_)
		{
			++it; continue; 
		}
		std::string className=(*it)->name_+"Stub";
		/*std::string name=program_->outputDir_+className+".cs";
		csharpFile_.open(name.c_str());
		csharpFile_<<indent()<<"namespace com.rpc"<<std::endl;
		csharpFile_<<indent()<<"{"<<std::endl;*/
		indent_up();
		csharpFile_<<indent()<<"public class "<<className<<std::endl;
		csharpFile_<<indent()<<"{ "<<std::endl;
		indent_up();

		csharpFile_<<indent()<<"public const string  strFingerprint=\""<<md5((*it)->getFingerPrint())<<"\";"<<std::endl;
		//属性
		csharpFile_<<indent()<<"public IProtocol __P__ ;"<<std::endl;
		//构造函数
		csharpFile_<<indent()<<"//construction"<<std::endl;
		csharpFile_<<indent()<<"public "<<className<<"(IProtocol p)"<<std::endl;
		csharpFile_<<indent()<<"{ "<<std::endl;
		indent_up();
		csharpFile_<<indent()<<"__P__=p; "<<std::endl;
		indent_down();
		csharpFile_<<indent()<<"} "<<std::endl;
		//stub 
		csharpFile_<<indent()<<"//stub "<<std::endl;
		int i=0;
		std::vector<FuctionDefType*>::iterator it_inner=(*it)->funs_.begin();
		while(it_inner!=(*it)->funs_.end())
		{
			FuctionDefType*& t=*it_inner;
			csharpFile_<<indent()<<"public void "<<t->name_<<"(";
			genFunAgrList(csharpFile_,t->argrs_);
			csharpFile_<<")"<<std::endl;
			csharpFile_<<indent()<<"{"<<std::endl;
			indent_up();
			//序列化
			csharpFile_<<indent()<<"Debug.Log(\""<<t->name_<<"\");"<<std::endl;
			csharpFile_<<indent()<<"__P__.writeMsgBegin();"<<std::endl;
			csharpFile_<<indent()<<"__P__.writeUInt16((ushort)"<<i++<<");"<<std::endl;
			serializeFields(t->argrs_);
			csharpFile_<<indent()<<"__P__.writeMsgEnd();"<<std::endl;
			indent_down();
			csharpFile_<<indent()<<"}"<<std::endl;
			++it_inner;
		}

		indent_down();
		csharpFile_<<indent()<<"}//class"<<std::endl;
		indent_down();
		/*csharpFile_<<indent()<<"}//namespace"<<std::endl;
		csharpFile_.close();*/
		++it;
	}
}

void CSharpGenerator::genServiceProxy()
{
	std::vector<ServiceDefType*>::iterator it=program_->services_.defs_.begin();
	std::vector<ServiceDefType*>::iterator it_end=program_->services_.defs_.end();
	while(it!=it_end)
	{
		if((*it)->fileName_!=program_->fileName_)
		{
			++it; continue; 
		}
		std::string className=(*it)->name_+"Proxy";
		/*std::string name=program_->outputDir_+className+".cs";
		csharpFile_.open(name.c_str());
		csharpFile_<<indent()<<"namespace com.rpc"<<std::endl;
		csharpFile_<<indent()<<"{"<<std::endl;*/
		indent_up();
		csharpFile_<<indent()<<"public class "<<className<<std::endl;
		csharpFile_<<indent()<<"{ "<<std::endl;
		indent_up();
		csharpFile_<<indent()<<"public const string strFingerprint=\""<<md5((*it)->getFingerPrint())<<"\";"<<std::endl;
		//属性
		std::string IFName="I"+(*it)->name_+"Proxy";
		csharpFile_<<indent()<<"public "<<IFName<<"  __I__;"<<std::endl;
		//构造函数
		csharpFile_<<indent()<<"//construction"<<std::endl;
		csharpFile_<<indent()<<"public "<<className<<"("<<IFName<<" I"<<")"<<std::endl;
		csharpFile_<<indent()<<"{ "<<std::endl;
		indent_up();
		csharpFile_<<indent()<<"__I__=I; "<<std::endl;
		indent_down();
		csharpFile_<<indent()<<"} "<<std::endl;
		csharpFile_<<std::endl;

		//dispatch
		csharpFile_<<indent()<<"public bool "<<"dispatch(IProtocol __P__)"<<std::endl;
		csharpFile_<<indent()<<"{"<<std::endl;
		indent_up();
		csharpFile_<<indent()<<"int id=__P__.readUInt16();"<<std::endl;
		csharpFile_<<indent()<<"switch (id)"<<std::endl;
		csharpFile_<<indent()<<"{"<<std::endl;
		indent_up();
		int i=0;
		std::vector<FuctionDefType*>::iterator it_inner=(*it)->funs_.begin();
		while(it_inner!=(*it)->funs_.end())
		{
			FuctionDefType*& t=*it_inner;
			csharpFile_<<indent()<<"case "<<i++<<" :"<<std::endl;
			csharpFile_<<indent()<<"{"<<std::endl;
			indent_up();
			csharpFile_<<indent()<<"return recv_"<<t->name_<<"(__P__);"<<std::endl;
			indent_down();
			csharpFile_<<indent()<<"}"<<std::endl;
			++it_inner;
		}
		csharpFile_<<indent()<<"default:"<<std::endl;
		csharpFile_<<indent()<<"{"<<std::endl;
		csharpFile_<<indent()<<"	return false;"<<std::endl;
		csharpFile_<<indent()<<"}"<<std::endl;
		indent_down();
		csharpFile_<<indent()<<"}//switch"<<std::endl;
		indent_down();
		csharpFile_<<indent()<<"}//dispatch"<<std::endl;
		csharpFile_<<std::endl;

		//proxy 
		csharpFile_<<indent()<<"//proxy "<<std::endl;
		it_inner=(*it)->funs_.begin();
		while(it_inner!=(*it)->funs_.end())
		{
			FuctionDefType*& t=*it_inner;
			csharpFile_<<indent()<<"private bool "<<"recv_"<<t->name_<<"(IProtocol __P__)"<<std::endl;
			csharpFile_<<indent()<<"{"<<std::endl;
			indent_up();
			csharpFile_<<indent()<<"Debug.Log(\""<<t->name_<<"\");"<<std::endl;
			//反序列化
			deSerializeFields(t->argrs_);
			csharpFile_<<indent()<<"return __I__."<<t->name_<<"(";
			genFunAgrList(csharpFile_,t->argrs_,true);
			csharpFile_<<");"<<std::endl;
			indent_down();
			csharpFile_<<indent()<<"}"<<std::endl;
			++it_inner;
		}

		indent_down();
		csharpFile_<<indent()<<"} //class"<<std::endl;
		indent_down();
		/*csharpFile_<<indent()<<"} //namespace"<<std::endl;
		csharpFile_.close();*/
		++it;
	}

}

void CSharpGenerator::genServiceProxyIf()
{
	std::vector<ServiceDefType*>::iterator it=program_->services_.defs_.begin();
	std::vector<ServiceDefType*>::iterator it_end=program_->services_.defs_.end();
	while(it!=it_end)
	{
		if((*it)->fileName_!=program_->fileName_)
		{
			++it; continue; 
		}
		std::string className="I"+(*it)->name_+"Proxy";
		std::string name=program_->outputDir_+className+".cs";
		ifFile_.open(name.c_str());
		ifFile_<<indent()<<"using System;"<<std::endl;
		ifFile_<<indent()<<"using System.Collections.Generic;"<<std::endl;
		ifFile_<<indent()<<"namespace ComRpc"<<std::endl;
		ifFile_<<indent()<<"{"<<std::endl;
		indent_up();
		ifFile_<<indent()<<"public interface "<<className<<std::endl;
		ifFile_<<indent()<<"{ "<<std::endl;
		indent_up();
		//interface 
		ifFile_<<indent()<<"//interface "<<std::endl;
		int i=0;
		std::vector<FuctionDefType*>::iterator it_inner=(*it)->funs_.begin();
		while(it_inner!=(*it)->funs_.end())
		{
			FuctionDefType*& t=*it_inner;
			ifFile_<<indent()<<"bool "<<t->name_<<"(";
			genFunAgrList(ifFile_,t->argrs_);
			ifFile_<<");"<<std::endl;
			++it_inner;
		}
		indent_down();
		ifFile_<<indent()<<"} //class"<<std::endl;
		indent_down();
		ifFile_<<indent()<<"} //namespace"<<std::endl;
		ifFile_.close();
		++it;
	}
	indent_down();

}

void CSharpGenerator::genFunAgrList( std::ofstream& stream,StructDefType* agrList,bool onlyValue)
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
				stream<<typeName(t->type_)<<" "<<t->name_;
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
				stream<<","<<typeName(t->type_)<<" "<<t->name_;
			}
		}
		++it_inner;
	}
}


void CSharpGenerator::serializeFields( StructDefType* t )
{
	std::vector<FieldDefType*>::iterator it_inner=t->members_.begin();
	while(it_inner!=t->members_.end())
	{
		serializeField((*it_inner)->type_,(*it_inner)->name_);
		csharpFile_<<std::endl;
		++it_inner;
	}
}

void CSharpGenerator::deSerializeFields( StructDefType* t )
{
	std::vector<FieldDefType*>::iterator it_inner=t->members_.begin();
	while(it_inner!=t->members_.end())
	{
			csharpFile_<<indent()<<typeName((*it_inner)->type_)<<" "<<(*it_inner)->name_
				<<"="<<DefaultValue((*it_inner)->type_)<<";"<<std::endl;
		
		deSerializeField((*it_inner)->type_,(*it_inner)->name_);
		csharpFile_<<std::endl;
		++it_inner;
	}
}
