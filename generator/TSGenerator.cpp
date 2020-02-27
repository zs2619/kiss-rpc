
#include"TSGenerator.h"
#include "../misc/md5.h"
#include <sstream>
#include <set>
#include "../misc/misc.h"
TSGenerator::TSGenerator( Program* pro,const std::string& name ) :Generator(pro,name)
{
}

static void generateImport(std::ofstream& tsFile ,const std::string& compileParam, const std::vector<FieldDefType*>& defs)
{
		std::set<std::string> importSet;

		for (auto& it:defs)
		{
			DefType* dt = nullptr;
			if (it->type_->is_struct() ||it->type_->is_enum()){
				dt=it->type_;
			}
			else if (it->type_->is_array())
			{
				ArrayDefType* array=(ArrayDefType*)it->type_;
				if (array->valueDef_->is_struct()||it->type_->is_enum()){
					dt=array->valueDef_;
				}
			}
			else if (it->type_->is_map())
			{
				MapDefType* mt=(MapDefType*)it->type_;
				if (mt->valueDef_->is_struct()||mt->valueDef_->is_enum()){
					dt=mt->valueDef_;
				}
			}
			if (dt!=nullptr)
			{
				importSet.insert(dt->name_);
			}
	
		}
		for (auto& it : importSet)
		{
			if (compileParam=="deno")
			{
				tsFile << "import  {" << it << "}  from \"./" << it << ".ts\"" << std::endl;
			}
			else
			{
				tsFile << "import  {" << it << "}  from \"./" << it << "\"" << std::endl;
			}
		}
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
		misc::mkdir((program_->getOutputDir() + generateContext->ns_.name_).c_str());
		std::string name = program_->getOutputDir() + generateContext->ns_.name_ + "/" + it->name_ + ".ts";
		tsFile_.open(name.c_str());
		tsFile_<<"export enum "<<it->name_<<std::endl;
		tsFile_<<"{ "<<std::endl;
		indent_up();
		for (auto& it_inner :it->defs_)
		{
			tsFile_<<indent()<<it_inner<<","<<std::endl;
		}
		indent_down();
		tsFile_<<"}//enum"<<std::endl;
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
		std::set<std::string> importSet;
		misc::mkdir((program_->getOutputDir() + generateContext->ns_.name_).c_str());
		std::string name=program_->getOutputDir()+generateContext->ns_.name_+"/"+ it->name_+".ts";
		tsFile_.open(name.c_str());
		if (program_->option_.compileParam_ =="deno")
		{
			tsFile_ << "import  * as rpc from \"../rpc/index.ts\"" << std::endl;
		}
		else
		{
			tsFile_ << "import  * as rpc from \"../rpc/index\"" << std::endl;
		}

		generateImport(tsFile_,program_->option_.compileParam_,it->members_);

		tsFile_<<"export class "<<it->name_<<std::endl;
		tsFile_<<"{ "<<std::endl;
		indent_up();
		tsFile_<<indent()<<"public static readonly strFingerprint:string=\""<<md5(it->getFingerPrint())<<"\""<<std::endl;
		for(auto& it_inner :it->members_)
		{
			FieldDefType*& t=it_inner;
			if (t->type_->is_simple_type()&& ((SimpleDefType*)t->type_)->t_==SimpleDefType::binaryType)
			{
				tsFile_<<indent()<<"public "<<t->name_<<"!:"<<typeName(t->type_) <<std::endl;
			}
			else
			{
				tsFile_<<indent()<<"public "<<t->name_<<":"<<typeName(t->type_) <<"="<<defaultValue(t->type_)<<std::endl;
			}
		}
		tsFile_<<indent()<<"//serialize"<<std::endl;
		tsFile_<<indent()<<"public serialize( __P__:rpc.Protocol):void "<<std::endl;
		tsFile_<<indent()<<"{ "<<std::endl;
		indent_up();
		serializeFields(it,"__P__");
		indent_down();
		tsFile_<<indent()<<"}// serialize"<<std::endl;

		tsFile_<<indent()<<"//deSerialize"<<std::endl;
		tsFile_<<indent()<<"public deSerialize( __P__:rpc.Protocol):void "<<std::endl;
		tsFile_<<indent()<<"{ "<<std::endl;
		indent_up();
		deSerializeFields(it,"__P__");
		indent_down();
		tsFile_<<indent()<<"}// deSerialize"<<std::endl;

		indent_down();
		tsFile_<<"}//class"<<std::endl;

		tsFile_.close();
	}
}

void TSGenerator::generateService()
{
	Context*  generateContext = program_->getGenerateContext();
	if (generateContext->ns_.services_.defs_.empty())
		return;

	for(auto& it :generateContext->ns_.services_.defs_)
	{
		misc::mkdir((program_->getOutputDir() + generateContext->ns_.name_).c_str());
		std::string name=program_->getOutputDir()+generateContext->ns_.name_+"/"+ it->name_+".ts";
		tsFile_.open(name.c_str());

		if (program_->option_.compileParam_ =="deno")
		{
			tsFile_ << "import  * as rpc from \"../rpc/index.ts\"" << std::endl;
		}
		else
		{
			tsFile_ << "import  * as rpc from \"../rpc/index\"" << std::endl;
		}
		for (auto& innerit:it->funs_)
		{
			generateImport(tsFile_,program_->option_.compileParam_,innerit->argrs_->members_);
		}

		std::string className=it->name_+"Stub";
		tsFile_<<indent()<<"export class "<<className<<" extends rpc.ServiceStub"<<std::endl;
		tsFile_<<indent()<<"{ "<<std::endl;
		indent_up();

		//fingerprint
		tsFile_<<indent()<<"static readonly strFingerprint=\""<<md5(it->getFingerPrint())<<"\""<<std::endl;
		tsFile_<<indent()<<"static readonly getObjName=\""<<generateContext->ns_.name_<<"\""<<std::endl;

		tsFile_<<indent()<<"public getObjName():string{ return "<<className<<".getObjName }"<<std::endl;

		for (auto& it_inner:it->funs_)
		{
			FuctionDefType*& t=it_inner;
			if (!t->result_->is_void())
			{
				tsFile_ << indent() << "private "<< t->name_ << "CallBack!:"<< "(ret"<<":"<< typeName(t->result_) << ")=>number"  << std::endl;
			}
		}

		tsFile_<<indent()<<"constructor(chan:rpc.Connection)"<<std::endl;
		tsFile_<<indent()<<"{ "<<std::endl;
		indent_up();
		tsFile_<<indent()<<"super(chan)"<<std::endl;
		indent_down();
		tsFile_<<indent()<<"} "<<std::endl;

		tsFile_ <<indent()<<"protected "<<"invokeAsync(msgId:number,p:rpc.Protocol, functionName:string) {" << std::endl;
		indent_up();
		tsFile_ <<indent()<<"let msg =new rpc.RpcMsg();" << std::endl;
		tsFile_ <<indent()<<"msg.serviceName= this.getObjName();" << std::endl;
		tsFile_ <<indent()<<"msg.functionName= functionName ;" << std::endl;
		tsFile_ <<indent()<<"msg.requestMsg.msgId = msgId;"<<std::endl;
		tsFile_ <<indent()<<"msg.requestMsg.buff = p.getBuffer();"<<std::endl;
		tsFile_ <<indent()<<"super.invoke(msg);" << std::endl;
		indent_down();
		tsFile_ <<indent()<< "}" << std::endl;

		int i=0;
		for (auto& innerit:it->funs_)
		{
			FuctionDefType*& t=innerit;
			tsFile_<<indent()<<"public "<<t->name_<<"(";
			genFunAgrList(tsFile_,t->argrs_);
			if (!t->result_->is_void())
			{
				if (!t->argrs_->members_.empty()) {
					tsFile_ << ",";
				}
				tsFile_ <<"cb:"<<"(ret"<<":"<< typeName(t->result_) << ")=>number";
			}
			tsFile_<<")"<<std::endl;
			tsFile_ <<indent()<< "{" << std::endl;

			indent_up();
			tsFile_<<indent()<<"let __P__= this.chan.getProtocol().createProtoBuffer();"<<std::endl;
			serializeFields(t->argrs_,"__P__",false);
			tsFile_<<indent()<<"this.invokeAsync("<<i++<<",__P__"<<",\"" << t->name_<<"\");"<<std::endl;
			tsFile_<<indent()<< "this."<<t->name_ << "CallBack" <<"=cb" << std::endl;

			indent_down();
			tsFile_ <<indent()<< "}" << std::endl;
		}

		tsFile_<<indent()<<"protected dispatch(msg:rpc.RpcMsg):boolean" <<std::endl;
		tsFile_<<indent()<<"{"<<std::endl;
		indent_up();
		tsFile_ << indent() << "let id=msg.responseMsg.msgId;" << std::endl;
		tsFile_ << indent() << "switch (id)" << std::endl;
		tsFile_ << indent() << "{" << std::endl;
		indent_up();
		i = 0;
		indent_up();
		for(auto& it_inner:it->funs_)
		{
			FuctionDefType*& t = it_inner;
			if (!t->result_->is_void())
			{
				tsFile_ << indent() << "case "<<i<<":"<< std::endl;
				tsFile_ << indent() << "{" << std::endl;
				indent_up();
				tsFile_ << indent() << "let __P__= this.chan.getProtocol().createProtoBuffer()" << std::endl;
				tsFile_ << indent() << "__P__.setBuffer(msg.responseMsg.buff);" << std::endl;
				tsFile_ << indent() <<  "let ret:"<< typeName(t->result_)<<"="<<defaultValue(t->result_)<< std::endl;

				deSerializeField(t->result_, "ret","__P__");

				tsFile_ << indent() << "let cbRet = this."<<t->name_<<"CallBack(ret);" << std::endl;
				tsFile_ << indent() << "return true;" << std::endl;
				indent_down();
				tsFile_ << indent() << "}" << std::endl;
			}
			i++;
		}


		tsFile_ << indent() << "default:" << std::endl;
		tsFile_ << indent() << "{" << std::endl;
		indent_up();
		tsFile_ << indent() << "return false;" << std::endl;
		indent_down();
		tsFile_ << indent() << "}" << std::endl;
		indent_down();
		tsFile_ << indent() << "}" << std::endl;
		indent_down();
		tsFile_<<indent()<<"}" << std::endl;

		indent_down();
		tsFile_<<indent()<<"};//class"<<std::endl;
	}

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
			case	SimpleDefType::int64Type : return "rpc.Int64";
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
			case SimpleDefType::int64Type : return "new rpc.Int64";
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

void TSGenerator::serializeFields( StructDefType* t ,const std::string& prefix,bool needThis)
{
	for(auto& it :t->members_)
	{
		if(needThis)
		{
			serializeField(it->type_,"this."+it->name_,prefix);
		}
		else
		{
			serializeField(it->type_,it->name_,prefix);
		}
		
		tsFile_<<std::endl;
	}

}

void TSGenerator::deSerializeFields( StructDefType* t ,const std::string& prefix,bool needThis)
{
	for(auto& it :t->members_)
	{
		if (needThis)
		{
			deSerializeField(it->type_,"this."+it->name_,prefix);
		}
		else
		{
			deSerializeField(it->type_,it->name_,prefix);
		}
		tsFile_<<std::endl;
	}

}
void TSGenerator::serializeField( DefType* t ,const std::string& fieldName,const std::string& prefix )
{
	if (t->is_struct())
	{
		tsFile_<<indent()<<fieldName<<".serialize("<<prefix<<");"<<std::endl;
	}
	else if (t->is_simple_type())
	{
		SimpleDefType* s=(SimpleDefType*)t;
		switch (s->t_)
		{
		case	SimpleDefType::boolType : 
			{
				tsFile_<<indent()<<prefix<<".writeBool("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::binaryType : 
			{
				tsFile_<<indent()<<prefix<<".writeBinary("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::byteType : 
			{
				tsFile_<<indent()<<prefix<<".writeByte("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::int8Type : 
			{
				tsFile_<<indent()<<prefix<<".writeInt8("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::int16Type :
			{
				tsFile_<<indent()<<prefix<<".writeInt16("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::int32Type :
			{
				tsFile_<<indent()<<prefix<<".writeInt32("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::int64Type :
			{
				tsFile_<<indent()<<prefix<<".writeInt64("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::floatType :
			{
				tsFile_<<indent()<<prefix<<".writeFloat("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::stringType :
			{
				tsFile_<<indent()<<prefix<<".writeString("<<fieldName<<");"<<std::endl;
				break;
			}
		}
	}
	else if(t->is_array())
	{
		tsFile_<<indent()<<prefix<<".writeInt32("<<fieldName<<".length);"<<std::endl;
		tsFile_<<indent()<<"for (let  temp of "<<fieldName<<")"<<std::endl;
		tsFile_<<indent()<<"{"<<std::endl;
		indent_up();
		serializeField(((ArrayDefType*)t)->valueDef_,"temp",prefix);
		indent_down();
		tsFile_<<indent()<<"}"<<std::endl;

	}
	else if (t->is_enum()) 
	{
		tsFile_<<indent()<<prefix<<".writeInt16("<<fieldName<<");"<<std::endl;
	}
	else if(t->is_map()) 
	{
		tsFile_<<indent()<<prefix<<".writeInt32("<<fieldName<<".size);"<<std::endl;
		tsFile_<<indent()<<fieldName<<".forEach((value , key) =>{ "<<std::endl;
		indent_up();
		serializeField(((MapDefType*)t)->keyDef_,"key",prefix);
		serializeField(((MapDefType*)t)->valueDef_,"value",prefix);
		indent_down();
		tsFile_<<indent()<<"})"<<std::endl;
	}
}
void TSGenerator::deSerializeField( DefType* t ,const std::string& fieldName,const std::string& prefix )
{
	if (t->is_struct())
	{
		tsFile_<<indent()<<fieldName<<".deSerialize("<<prefix<<")"<<std::endl;
	}
	else if (t->is_simple_type())
	{
		SimpleDefType* s=(SimpleDefType*)t;
		switch (s->t_)
		{
		case	SimpleDefType::boolType : 
			{
				tsFile_<<indent()<<fieldName<<"="<<prefix<<".readBool()"<<std::endl;
				break;
			} 
		case	SimpleDefType::int8Type : 
			{
				tsFile_<<indent()<<fieldName<<"="<<prefix<<".readInt8()"<<std::endl;
				break;
			} 
		case	SimpleDefType::int16Type :
			{
				tsFile_<<indent()<<fieldName<<"="<<prefix<<".readInt16()"<<std::endl;
				break;
			} 
		case	SimpleDefType::int32Type :
			{
				tsFile_<<indent()<<fieldName<<"="<<prefix<<".readInt32()"<<std::endl;
				break;
			} 
		case	SimpleDefType::int64Type :
			{
				tsFile_<<indent()<<fieldName<<"="<<prefix<<".readInt64()"<<std::endl;
				break;
			} 
		case	SimpleDefType::floatType :
			{
				tsFile_<<indent()<<fieldName<<"="<<prefix<<".readFloat()"<<std::endl;
				break;
			} 
		case	SimpleDefType::stringType :
			{
				tsFile_<<indent()<<fieldName<<"="<<prefix<<".readString()"<<std::endl;
				break;
			}
		case	SimpleDefType::binaryType :
			{
				tsFile_<<indent()<<fieldName<<"="<<prefix<<".readBinary()"<<std::endl;
				break;
			}
		}
	}
	else if(t->is_array())
	{
		static int i=0;
		std::stringstream str;
		str<<"_n_"<<i<<"_array";
		tsFile_<<indent()<<"let "<<str.str()<<":number="<<prefix<<".readInt32()"<<std::endl;
		std::stringstream count;count<<"_i_"<<i<<"_";
		i++;

		tsFile_<<indent()<<"for( let "<<count.str()<<":number=0; "<<count.str()<<"<"<<str.str()<<"; "<<count.str()<<"++) {"<<std::endl;
		indent_up();
		tsFile_<<indent()<<"let tmp:"<<typeName(((ArrayDefType*)t)->valueDef_)<<"="<<defaultValue(((ArrayDefType*)t)->valueDef_)<<std::endl;
		deSerializeField(((ArrayDefType*)t)->valueDef_,"tmp",prefix);
		tsFile_<<indent()<<fieldName<<".push(tmp)"<<std::endl;
		indent_down();
		tsFile_<<indent()<<"}"<<std::endl;

	}
	else if (t->is_enum())
	{
		tsFile_<<indent()<<fieldName<<"="<<prefix<<".readInt16()"<<std::endl;
	}
	else if(t->is_map())
	{
		static int i=0;
		std::stringstream str;
		str<<"_n_"<<i<<"_map";
		tsFile_<<indent()<<"let "<<str.str()<<":number="<<prefix<<".readInt32()"<<std::endl;
		std::stringstream count;count<<"_i_"<<i<<"_";
		i++;

		tsFile_<<indent()<<"for(let "<<count.str()<<":number=0; "<<count.str()<<"<"<<str.str()<<"; "<<count.str()<<"++ ){"<<std::endl;
		indent_up();
		//key
		tsFile_<<indent()<<"let tmpKey:"<<typeName(((MapDefType*)t)->keyDef_)<<"="<<defaultValue(((MapDefType*)t)->keyDef_)<<std::endl;
		deSerializeField(((MapDefType*)t)->keyDef_,"tmpKey",prefix);

		//value
		tsFile_<<indent()<<"let tmpValue:"<<typeName(((MapDefType*)t)->valueDef_)<<"="<<defaultValue(((MapDefType*)t)->valueDef_)<<std::endl;
		deSerializeField(((MapDefType*)t)->valueDef_,"tmpValue",prefix);

		tsFile_<<indent()<<fieldName<<".set(tmpKey,tmpValue)"<<std::endl;
		indent_down();
		tsFile_<<indent()<<"}"<<std::endl;
	}
}

void TSGenerator::genFunAgrList( std::ofstream& stream,StructDefType* agrList,ParamType paramType)
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
			stream<<t->name_ <<":"<<typeName(t->type_,true);
		}
		else if (paramType==PT_Type) 
		{
			stream<<typeName(t->type_,true);
		}
		first = false;
	}

}
