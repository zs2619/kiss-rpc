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
	generateService();
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
		std::string temp="new Dictionary";
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
		std::string temp="Dictionary";
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
		as3File_<<indent()<<"public bool serialize(IProtocol __P__) "<<std::endl;
		as3File_<<indent()<<"{ "<<std::endl;
		indent_up();
		//序列化属性
		it_inner=(*it)->members_.begin();
		while(it_inner!=(*it)->members_.end())
		{
			serializeField((*it_inner)->type_,(*it_inner)->name_);
			as3File_<<std::endl;
			++it_inner;
		}
		as3File_<<indent()<<"return true;"<<std::endl;
		indent_down();
		as3File_<<indent()<<"}//serialize "<<std::endl;
		
		//反序列化函数
		as3File_<<std::endl;
		as3File_<<indent()<<"//deSerialize"<<std::endl;
		as3File_<<indent()<<"public bool deSerialize(IProtocol __P__)"<<std::endl;
		as3File_<<indent()<<"{ "<<std::endl;
		indent_up();
		//反序列化属性
		it_inner=(*it)->members_.begin();
		while(it_inner!=(*it)->members_.end())
		{
			deSerializeField((*it_inner)->type_,(*it_inner)->name_);
			as3File_<<std::endl;
			++it_inner;
		}
		as3File_<<indent()<<"return true;"<<std::endl;
		indent_down();
		as3File_<<indent()<<"}// deSerialize"<<std::endl;

		indent_down();

		as3File_<<"}//class"<<std::endl;
		as3File_.close();
		++it;
	}
}


void As3Generator::serializeField( DefType* t ,const std::string& fieldName )
{
	if (t->is_struct())
	{
		as3File_<<indent()<<fieldName<<".serialize(__P__);"<<std::endl;
	}
	else if (t->is_simple_type())
	{
		SimpleDefType* s=(SimpleDefType*)t;
		switch (s->t_)
		{
		case	SimpleDefType::boolType : 
			{
				as3File_<<indent()<<"__P__.writeBool("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::uint8Type : 
			{
				as3File_<<indent()<<"__P__.writeUint8("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::int8Type : 
			{
				as3File_<<indent()<<"__P__.writeInt8("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::uint16Type :
			{
				as3File_<<indent()<<"__P__.writeUInt16("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::int16Type :
			{
				as3File_<<indent()<<"__P__.writeInt16("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::uint32Type :
			{
				as3File_<<indent()<<"__P__.writeUInt32("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::int32Type :
			{
				as3File_<<indent()<<"__P__.writeInt32("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::int64Type :
			{
				as3File_<<indent()<<"__P__.writeInt64("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::floatType :
			{
				as3File_<<indent()<<"__P__.writeFloat("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::stringType :
			{
				as3File_<<indent()<<"__P__.writeString("<<fieldName<<");"<<std::endl;
				break;
			}

		}
	}
	else if(t->is_array())
	{
		as3File_<<indent()<<"__P__.writeUInt16("<<fieldName<<".length );"<<std::endl;
		std::string temp="_i_"+fieldName+"_";
		as3File_<<indent()<<"for (var  "<<temp<<":int=0;"<<temp<<"<"<<fieldName<<".length;"<<temp<<"++)"<<std::endl;
		as3File_<<indent()<<"{"<<std::endl;
		indent_up();
		std::string tempAgr=fieldName+"["+temp+"]";
		serializeField(((ArrayDefType*)t)->valueDef_,tempAgr);
		indent_down();
		as3File_<<indent()<<"}"<<std::endl;

	}else if (t->is_enum())
	{
		as3File_<<indent()<<"__P__.writeUInt16("<<fieldName<<");"<<std::endl;

	}else if(t->is_map())
	{
		//todo map
	}
}

void As3Generator::deSerializeField( DefType* t ,const std::string& fieldName )
{
	if (t->is_struct())
	{
		as3File_<<indent()<<fieldName<<".deSerialize(__P__);"<<std::endl;
	}
	else if (t->is_simple_type())
	{
		SimpleDefType* s=(SimpleDefType*)t;
		switch (s->t_)
		{
		case	SimpleDefType::boolType : 
			{
				as3File_<<indent()<<fieldName<<"=__P__.readBool() ;"<<std::endl;
				break;
			} 
		case	SimpleDefType::uint8Type : 
			{
				as3File_<<indent()<<fieldName<<"=__P__.readUint8();"<<std::endl;
				break;
			} 
		case	SimpleDefType::int8Type : 
			{
				as3File_<<indent()<<fieldName<<"=__P__.>readInt8();"<<std::endl;
				break;
			} 
		case	SimpleDefType::uint16Type :
			{
				as3File_<<indent()<<fieldName<<"=__P__.readUInt16();"<<std::endl;
				break;
			} 
		case	SimpleDefType::int16Type :
			{
				as3File_<<indent()<<fieldName<<"=__P__.readInt16();"<<std::endl;
				break;
			} 
		case	SimpleDefType::uint32Type :
			{
				as3File_<<indent()<<fieldName<<"=__P__.readUInt32();"<<std::endl;
				break;
			} 
		case	SimpleDefType::int32Type :
			{
				as3File_<<indent()<<fieldName<<"=__P__.readInt32();"<<std::endl;
				break;
			} 
		case	SimpleDefType::int64Type :
			{
				as3File_<<indent()<<fieldName<<"=__P__.readInt64();"<<std::endl;
				break;
			} 
		case	SimpleDefType::floatType :
			{
				as3File_<<indent()<<fieldName<<"=__P__.readFloat();"<<std::endl;
				break;
			} 
		case	SimpleDefType::stringType :
			{
				as3File_<<indent()<<fieldName<<"=__P__.readString();"<<std::endl;
				break;
			}
		}
	}
	else if(t->is_array())
	{
		std::string size="_n_"+fieldName+"_array";
		as3File_<<indent()<<"var "<<size<<":int=0;"<<std::endl;
		as3File_<<indent()<<size<<"=__P__.readUInt16();"<<std::endl;
		std::string count="_i_"+fieldName+"_";
		as3File_<<indent()<<"for (var "<<count<<":int=0;"<<count<<"<"<<size<<";"<<count<<"++)"<<std::endl;
		as3File_<<indent()<<"{"<<std::endl;
		indent_up();
		std::string valueStr="temp_array_"+fieldName;
		std::string tempAgr="var "+valueStr+":"+typeName(((ArrayDefType*)t)->valueDef_)
						+"="+DefaultValue(((ArrayDefType*)t)->valueDef_)+";";
		as3File_<<indent()<<tempAgr<<std::endl;
		deSerializeField(((ArrayDefType*)t)->valueDef_,valueStr);
		as3File_<<indent()<<fieldName<<".push("<<valueStr<<");"<<std::endl;
		indent_down();
		as3File_<<indent()<<"}"<<std::endl;

	}else if (t->is_enum())
	{
		as3File_<<indent()<<fieldName<<"=__P__.readUInt16();"<<std::endl;

	}else if(t->is_map())
	{
		//todo map
	}
}

void As3Generator::generateService()
{
	genServiceStub();
	genServiceProxy();
}
void As3Generator::genFunAgrList( std::ofstream& stream,StructDefType* agrList,bool onlyValue)
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
				stream<<typeName(t->type_)<<"  "<<t->name_;
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
				stream<<","<<typeName(t->type_)<<"  "<<t->name_;
			}
		}
		++it_inner;
	}
}


void As3Generator::serializeFields( StructDefType* t )
{
	std::vector<FieldDefType*>::iterator it_inner=t->members_.begin();
	while(it_inner!=t->members_.end())
	{
		serializeField((*it_inner)->type_,(*it_inner)->name_);
		as3File_<<std::endl;
		++it_inner;
	}
}

void As3Generator::genServiceStub()
{
	std::vector<ServiceDefType*>::iterator it=program_->services_.defs_.begin();
	std::vector<ServiceDefType*>::iterator it_end=program_->services_.defs_.end();
	while(it!=it_end)
	{
		std::string className=(*it)->name_+"Stub";
		std::string name=program_->outputDir_+className+".as";
		as3File_.open(name.c_str());
		as3File_<<"public class"<<className<<std::endl;
		as3File_<<"{ "<<std::endl;
		indent_up();
		//属性
		as3File_<<indent()<<"public IProtocol __P__;"<<std::endl;
		//构造函数
		as3File_<<indent()<<"//construction"<<std::endl;
		as3File_<<indent()<<"public function "<<className<<"(IProtocol p)"<<std::endl;
		as3File_<<indent()<<"{ "<<std::endl;
		indent_up();
		as3File_<<indent()<<"__P__=p; "<<std::endl;
		indent_down();
		as3File_<<indent()<<"} "<<std::endl;
		//stub 
		as3File_<<indent()<<"//stub "<<std::endl;
		int i=0;
		std::vector<FuctionDefType*>::iterator it_inner=(*it)->funs_.begin();
		while(it_inner!=(*it)->funs_.end())
		{
			FuctionDefType*& t=*it_inner;
			as3File_<<indent()<<"public void "<<t->name_<<"(";
			genFunAgrList(as3File_,t->argrs_);
			as3File_<<")"<<std::endl;
			as3File_<<indent()<<"{"<<std::endl;
			indent_up();
			//序列化
			as3File_<<indent()<<"__P__->writeMsgBegin();"<<std::endl;
			as3File_<<indent()<<"__P__->writeUInt16("<<i++<<");"<<std::endl;
			serializeFields(t->argrs_);
			as3File_<<indent()<<"__P__->writeMsgEnd();"<<std::endl;
			indent_down();
			as3File_<<indent()<<"}"<<std::endl;
			++it_inner;
		}

		indent_down();
		as3File_<<"}//class"<<std::endl;
		as3File_.close();
		++it;
	}
}

void As3Generator::genServiceProxy()
{
	std::vector<ServiceDefType*>::iterator it=program_->services_.defs_.begin();
	std::vector<ServiceDefType*>::iterator it_end=program_->services_.defs_.end();
	while(it!=it_end)
	{
		std::string className=(*it)->name_+"Proxy";
		std::string name=program_->outputDir_+className+".as";
		as3File_.open(name.c_str());
		as3File_<<"public class"<<className<<std::endl;
		as3File_<<"{ "<<std::endl;
		indent_up();
		//属性
		as3File_<<indent()<<"public IProtocol __P__;"<<std::endl;
		//构造函数
		as3File_<<indent()<<"//construction"<<std::endl;
		as3File_<<indent()<<"public function "<<className<<"(IProtocol p)"<<std::endl;
		as3File_<<indent()<<"{ "<<std::endl;
		indent_up();
		as3File_<<indent()<<"__P__=p; "<<std::endl;
		indent_down();
		as3File_<<indent()<<"} "<<std::endl;
		//stub 
		as3File_<<indent()<<"//proxy "<<std::endl;
		int i=0;
		std::vector<FuctionDefType*>::iterator it_inner=(*it)->funs_.begin();
		while(it_inner!=(*it)->funs_.end())
		{
			FuctionDefType*& t=*it_inner;
			++it_inner;
		}

		indent_down();
		as3File_<<"} //class"<<std::endl;
		as3File_.close();
		++it;
	}

}