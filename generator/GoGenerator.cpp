//==============================================
/**
	@date:		2015:8:14  
	@file: 		GoGenerator.cpp
	@author: 	zhangshuai
*/
//==============================================
#include "GoGenerator.h"
#include "../misc/md5.h"
#include "../misc/misc.h"
#include <sstream>
#include <locale>
#include <cctype>
static std::string setInitialUpper(std::string str)
{
	if (str.empty())
		return "";
	str[0]=::toupper(str[0]);
	return str;
}
static std::string setInitialLower(std::string str)
{
	if (str.empty())
		return "";
	str[0]=::tolower(str[0]);
	return str;
}

GoGenerator::GoGenerator( Program* pro,const std::string& name ) :Generator(pro,name)
{

}

void GoGenerator::generateProgram()
{
	generateEnum();
	generateStruct();
	generateService();
}

void GoGenerator::generateEnum()
{
	Context*  generateContext = program_->getGenerateContext();
	if (generateContext->ns_.enums_.defs_.empty())
		return;

	for(auto& it :program_->enums_.defs_)
	{
		std::string name=program_->getOutputDir()+ setInitialLower(it->name_)+".go";
		goFile_.open(name.c_str());
		goFile_<<indent()<<"package "<<generateContext->ns_.name_<<std::endl;

		goFile_<<indent()<<"const ("<<std::endl;
		indent_up();
		bool flag=true;
		for (auto& inner:it->defs_)
		{
			if (flag)
			{
				goFile_<<indent()<<setInitialUpper(inner)<<" =iota "<<std::endl;
				flag=false;
			}
			else
			{
				goFile_<<indent()<<setInitialUpper(inner)<<std::endl;
			}
		}
		indent_down();
		goFile_<<indent()<<")"<<std::endl;
		//map
		goFile_<<indent()<<"var ("<<std::endl;
		indent_up();

		goFile_<<indent()<<setInitialUpper(it->name_)<<"S2I"<<"= map[string] int16 {"<<std::endl;
		for (auto& inner:it->defs_)
		{
			goFile_<<indent()<<"\""<<setInitialUpper(inner)<<"\" : "<<setInitialUpper(inner)<<","<<std::endl;
		}
		goFile_<<indent()<<"}"<<std::endl;

		goFile_<<indent()<<setInitialUpper(it->name_)<<"I2S"<<"= map[int16] string {"<<std::endl;
		for (auto& inner:it->defs_)
		{
			goFile_<<indent()<<""<<setInitialUpper(inner)<<" : "<<"\""<<setInitialUpper(inner)<<"\""<<","<<std::endl;
		}
		goFile_<<indent()<<"}"<<std::endl;

		indent_down();
		goFile_<<indent()<<")"<<std::endl;
		goFile_.close();
	}
}

void GoGenerator::generateStruct()
{
	Context*  generateContext = program_->getGenerateContext();
	if (generateContext->ns_.structs_.defs_.empty())
		return;

	for(auto& it :program_->structs_.defs_)
	{
		std::string name=program_->getOutputDir()+setInitialLower(it->name_)+".go";
		goFile_.open(name.c_str());
		goFile_<<indent()<<"package "<<generateContext->ns_.name_<<std::endl;

		//imports 
		goFile_<<indent()<<"import ("<<std::endl;
		indent_up();
		goFile_<<indent()<<"\"kiss/rpc\""<<std::endl;
		indent_down();
		goFile_<<indent()<<")"<<std::endl;

		///struct 
		//fingerprint
		goFile_<<std::endl;
		goFile_<<indent()<<"const "<<it->name_<<"_"<<"strFingerprint=\""<<md5(it->getFingerPrint())<<"\""<<std::endl;
		goFile_<<indent()<<"type "<<setInitialUpper(it->name_)<<" struct{"<<std::endl;
		indent_up();

		for(auto& inner:it->members_)
		{
			goFile_<<indent()<<setInitialUpper(inner->name_)<<" "<<typeName(inner->type_)<<" `json:\""<<inner->name_<<"\"`"<<std::endl;
		}

		indent_down();
		goFile_<<indent()<<"}" <<std::endl;

		//struct id
		goFile_<<indent()<<"func (this *"<<setInitialUpper(it->name_)<<") GetFingerprint() string{"<<std::endl;
		indent_up();
		goFile_<<indent()<<"return "<<it->name_<<"_"<<"strFingerprint"<<std::endl;
		indent_down();
		goFile_<<"}"<<std::endl;

		goFile_<<indent()<<"func (this *"<<setInitialUpper(it->name_)<<") Serialize( P__ rpc.IProtocol){"<<std::endl;
		indent_up();
		for(auto& inner:it->members_)
		{
			serializeField(inner->type_,"this."+setInitialUpper(inner->name_),"P__");
			goFile_<<std::endl;
		}
		indent_down();
		goFile_<<"}"<<std::endl;

		goFile_<<indent()<<"func (this *"<<setInitialUpper(it->name_)<<") DeSerialize( P__ rpc.IProtocol) bool{"<<std::endl;
		indent_up();
		for(auto& inner:it->members_)
		{
			deSerializeField(inner->type_,"this."+setInitialUpper(inner->name_),"P__");
			goFile_<<std::endl;
		}
		goFile_<<indent()<<"return true"<<std::endl;
		indent_down();

		goFile_<<"}"<<std::endl;

		goFile_<<std::endl;
		goFile_.close();
	}
}

void GoGenerator::generateService()
{
	genServiceStub();
	genServiceProxy();
}
void GoGenerator::genServiceStub()
{
	Context*  generateContext = program_->getGenerateContext();
	if (generateContext->ns_.services_.defs_.empty())
		return;

	std::string fileName=program_->getOutputDir()+setInitialLower(program_->getBaseName())+"Stub.go";
	goFile_.open(fileName.c_str());
	goFile_<<indent()<<"package "<<generateContext->ns_.name_<<std::endl;
	goFile_<<std::endl;

	//imports 
	goFile_<<indent()<<"import ("<<std::endl;
	indent_up();
	goFile_<<indent()<<"\"kiss/rpc\""<<std::endl;
	indent_down();
	goFile_<<indent()<<")"<<std::endl;

	for (auto& it:program_->services_.defs_)
	{
		goFile_<<indent()<<"const "<<it->name_<<"_"<<"strFingerprintStub=\""<<md5(it->getFingerPrint())<<"\""<<std::endl;
		std::string ifName=setInitialUpper(it->name_)+"Stub";
		goFile_<<indent()<<"type "<<ifName<<" struct {"<<std::endl;
		indent_up();
		goFile_<<indent()<<"*rpc.ServiceStub"<<std::endl;
		goFile_<<indent()<<"strFingerprint string"<<std::endl;
		goFile_<<indent()<<"getObjName string"<<std::endl;
		goFile_<<std::endl;

		for (auto& it_inner:it->funs_)
		{
			goFile_<<indent()<<it_inner->name_<<"CallBack"<<" "<<setInitialUpper(it_inner->name_)<<"CallBackType" <<std::endl;
		}
		indent_down();
		goFile_<<indent()<<"}"<<std::endl;
		goFile_<<std::endl;

		goFile_<<indent()<<"func New"<<setInitialUpper(it->name_)<<"Stub"<<"(rpcChan *rpc.RpcChannel) *"<<setInitialUpper(it->name_)<<"Stub"<< "{"<<std::endl;
		indent_up();
		goFile_<<indent()<<"stub := &"<<setInitialUpper(it->name_)<<"Stub"<<"{strFingerprint: \""<<md5(it->getFingerPrint())<<"\", getObjName: "
		<<"\""<<it->name_<<"\", ServiceStub: rpc.NewServiceStub(rpcChan)}"<<std::endl;
		goFile_<<indent()<<"stub.ServiceStub.RpcMsgDispatchCB = stub.Dispatch"<<std::endl;
		goFile_<<indent()<<"return stub"<<std::endl;
		indent_down();
		goFile_<<indent()<<"}"<<std::endl;

		goFile_<<indent()<<"func (this *OpServiceStub) invokeAsync(msgId uint16, p rpc.IProtocol, funcName string) {"<<std::endl;
		indent_up();
		goFile_<<indent()<<"msg := &rpc.RpcMsg{}"<<std::endl;
		goFile_<<indent()<<"msg.ServiceName = funcName"<<std::endl;
		goFile_<<indent()<<"msg.RequestMsg.MsgId = msgId"<<std::endl;
		goFile_<<indent()<<"msg.RequestMsg.Buff = p.GetBuffer()"<<std::endl;
		goFile_<<indent()<<"this.Invoke(msg)"<<std::endl;
		indent_down();
		goFile_<<indent()<<"}"<<std::endl;

		
		for (auto& it_inner:it->funs_)
		{
			goFile_<<indent()<<"type "<<setInitialUpper(it_inner->name_)<<"CallBackType "
			<<"func (";
			goFile_<< typeName(it_inner->result_);
			goFile_<<") error"<< std::endl;
		}
		goFile_<<std::endl;

		//get id
		goFile_<<indent()<<"func (this *"<<setInitialUpper(it->name_)<<"Stub) GetFingerprint() string{"<<std::endl;
		indent_up();
		goFile_<<indent()<<"return this.strFingerprint"<<std::endl;
		indent_down();
		goFile_<<indent()<<"}"<<std::endl;
		//get id
		goFile_<<indent()<<"func (this *"<< setInitialUpper(it->name_)<<"Stub) GetObjName() string{"<<std::endl;
		indent_up();
		goFile_<<indent()<<"return this.getObjName"<<std::endl;
		indent_down();
		goFile_<<indent()<<"}"<<std::endl;
		
		//stub func
		int i=0;
		for(auto& inner:it->funs_)
		{
			goFile_<<indent()<<"func (this *"<<setInitialUpper(it->name_)<<"Stub) "<<setInitialUpper(inner->name_)<<" (";
			genFunAgrList(goFile_,inner->argrs_);
			if (!inner->result_->is_void())
			{
				if (!inner->argrs_->members_.empty()) {
					goFile_ << ",";
				}
				goFile_ <<"cb "<<setInitialUpper(inner->name_)<<"CallBackType";
			}

			goFile_<<") error {"<<std::endl;
			indent_up();
			goFile_<<indent()<<"proto := this.GetProtocol().CreateProtoBuffer()"<<std::endl;
			serializeFields(inner->argrs_,"proto");
			goFile_<<indent()<<"this.invokeAsync("<<i<<", proto, \""<<inner->name_<<"\")"<<std::endl;
			if (!inner->result_->is_void())
			{
				goFile_ << indent() << "this." << inner->name_ << "CallBack = cb" << std::endl;
			}
			goFile_<<indent()<<"return nil"<<std::endl;
			indent_down();
			goFile_<<indent()<<"}"<<std::endl;
			i++;
		}

		goFile_ <<indent()<< "func (this *"<<setInitialUpper(it->name_)<<"Stub) "<<"Dispatch(msg *rpc.RpcMsg) error {" << std::endl;
		indent_up();
		goFile_ << indent() << " id:=msg.ResponseMsg.MsgId" << std::endl;
		goFile_ << indent() << "switch id {" << std::endl;
		i = 0;
		indent_up();
		for(auto& it_inner:it->funs_)
		{
			FuctionDefType*& t = it_inner;
			if (!t->result_->is_void())
			{
				goFile_ << indent() << "case "<<i<<":"<< std::endl;
				goFile_ << indent() << "{" << std::endl;
				indent_up();
				goFile_<<indent()<<"proto := this.GetProtocol().CreateProtoBuffer()"<<std::endl;
				goFile_ << indent() << "proto.SetBuffer(msg.ResponseMsg.Buff)" << std::endl;

				goFile_<<indent()<<"var ret "<<typeName(t->result_)<<std::endl;
				deSerializeField(t->result_, "ret","proto");

				goFile_ << indent() << "cbRet := this."<<t->name_<<"CallBack(ret)" << std::endl;
				goFile_ << indent() << "return cbRet" << std::endl;
				indent_down();
				goFile_ << indent() << "}" << std::endl;
			}
			i++;
		}

		goFile_ << indent() << "default:" << std::endl;
		goFile_ << indent() << "{" << std::endl;
		indent_up();
		goFile_ << indent() << "return nil" << std::endl;
		indent_down();
		goFile_ << indent() << "}" << std::endl;
		indent_down();
		goFile_ << indent() << "}" << std::endl;

		indent_down();
		goFile_ <<indent()<< "}" << std::endl;
		goFile_<<indent()<<std::endl;

	}
	goFile_.close();
}
void GoGenerator::genServiceProxy()
{
	Context*  generateContext = program_->getGenerateContext();
	if (generateContext->ns_.services_.defs_.empty())
		return;
	//
	std::string fileName=program_->getOutputDir()+program_->getBaseName()+"Proxy.go";
	goFile_.open(fileName.c_str());
	goFile_<<indent()<<"package "<<generateContext->ns_.name_<<std::endl;
	goFile_<<std::endl;

	//imports 
	goFile_<<indent()<<"import ("<<std::endl;
	indent_up();
	goFile_<<indent()<<"\"kiss/rpc\""<<std::endl;
	indent_down();
	goFile_<<indent()<<")"<<std::endl;

	for (auto& it:program_->services_.defs_)
	{
		std::string ifName="I"+setInitialUpper(it->name_)+"Proxy";
		goFile_<<indent()<<"type "<<ifName<<" interface {"<<std::endl;
		indent_up();
		for(auto& inner:it->funs_)
		{
			goFile_<<indent()<<setInitialUpper(inner->name_)<<" (";
			genFunAgrList(goFile_,inner->argrs_);
			goFile_<<") ("<<typeName(inner->result_);
			if (!inner->result_->is_void())
			{
				goFile_<<",";
			}
			goFile_<<"error)";
			goFile_<<std::endl;
		}
		indent_down();
		goFile_<<indent()<<"}"<<std::endl;
		goFile_<<std::endl;

		std::string cName=setInitialUpper(it->name_)+"Proxy";
		goFile_<<indent()<<"const "<<it->name_<<"_"<<"strFingerprintProxy=\""<<md5(it->getFingerPrint())<<"\""<<std::endl;

		goFile_<<indent()<<"type "<<cName<<" struct {"<<std::endl;
		indent_up();
		
		goFile_<<indent()<<"*rpc.RpcService" << std::endl;
		goFile_<<indent()<<"strFingerprint string"<<std::endl;
		goFile_<<indent()<<"getObjName string"<<std::endl;
		goFile_<<indent()<<"cbObject "<<ifName<<std::endl;
		indent_down();
		goFile_<<indent()<<"}"<<std::endl;
		goFile_<<std::endl;

		goFile_<<indent()<< "func New"<<cName<<"(rpcService *rpc.RpcService,"<<"obj "<<ifName<<") *"<<cName<<" {"<<std::endl;
		indent_up();
		goFile_<<indent()<< "return &"<<cName<<"{strFingerprint: \""<<md5(it->getFingerPrint())<<"\", getObjName:\""<<cName
		<<"\", RpcService: rpcService , cbObject:obj }"<<std::endl;
		indent_down();
		goFile_<<indent()<<"}"<<std::endl;

		//get id
		goFile_<<indent()<<"func (this *"<<setInitialUpper(it->name_)<<"Proxy) GetFingerprint() string{"<<std::endl;
		indent_up();
		goFile_<<indent()<<"return "<<it->name_<<"_"<<"strFingerprintProxy"<<std::endl;
		indent_down();
		goFile_<<indent()<<"}"<<std::endl;
		goFile_<<indent()<<"func (this *"<< setInitialUpper(it->name_)<<"Proxy) GetObjName() string{"<<std::endl;
		indent_up();
		goFile_<<indent()<<"return this.getObjName"<<std::endl;
		indent_down();
		goFile_<<indent()<<"}"<<std::endl;
		

		//dispatch
		goFile_<<indent()<<"func (this *"<< setInitialUpper(it->name_)<<"Proxy) "<<"Dispatch"<<"(";
		goFile_<<" msg *rpc.RpcMsg ";
		goFile_<<") bool {"<<std::endl;
		indent_up();
		goFile_<<indent()<<"id := msg.ResponseMsg.MsgId"<<std::endl;
		goFile_<<indent()<<"switch id {"<<std::endl;
		int i=0;
		for(auto& t:it->funs_)
		{
			goFile_<<indent()<<"case "<<i++<<":"<<std::endl;
			indent_up();

			goFile_<<indent()<<"proto := this.GetProtocol().CreateProtoBuffer()"<<std::endl;
			goFile_<<indent()<<"proto.SetBuffer(msg.ResponseMsg.Buff)"<<std::endl;
			deSerializeFields(t->argrs_,"proto");
			if (!t->result_->is_void())
			{
				goFile_ << indent() << "result,err := this.cbObject." <<setInitialUpper( t->name_ )<< "(";
			}
			else
			{
				goFile_ << indent() << "err := this.cbObject." << setInitialUpper(t->name_) << "(";
			}
			genFunAgrList(goFile_, t->argrs_, PT_Value);
			goFile_ <<  ")"<<std::endl;

			if (!t->result_->is_void())
			{
				goFile_<< indent()<< "if err!=nil{"<<std::endl;
				indent_up();
				goFile_<< indent()<<"proto := this.GetProtocol().CreateProtoBuffer()"<<std::endl;
				serializeField(t->result_, "result","proto");
				indent_down();
				goFile_ << indent() <<"}"<<std::endl;
			}
			else
			{
				goFile_<< indent()<< "if err!=nil{"<<std::endl;

				goFile_ << indent() <<"}"<<std::endl;
			}
		
			goFile_<<indent()<<"return "<<"true"<<std::endl;
			indent_down();
		}
		goFile_<<indent()<<"default:"<<std::endl;
		goFile_<<indent()<<"return false"<<std::endl;


		goFile_<<indent()<<"}//switch "<<std::endl;

		indent_down();
		goFile_<<indent()<<"}//dispatch func"<<std::endl;

	}
	goFile_.close();


}
void GoGenerator::deSerializeFields( StructDefType* t ,std::string s)
{
	for (auto& it :t->members_)
	{
		goFile_<<indent()<<"var "<<it->name_<<" "<<typeName(it->type_)<<std::endl;
		deSerializeField(it->type_,it->name_,s);
		goFile_<<std::endl;
	}
}
void GoGenerator::serializeFields( StructDefType* t ,std::string s)
{
	for (auto it:t->members_)
	{
		serializeField(it->type_,it->name_,s);
		goFile_<<std::endl;
	}
} 
void GoGenerator::serializeField( DefType* t ,const std::string& fieldName ,const std::string& inner)
{
	if (t->is_struct())
	{
		goFile_<<indent()<<fieldName<<".Serialize("<<inner<<")"<<std::endl;
	}
	else if (t->is_simple_type())
	{
		SimpleDefType* s=(SimpleDefType*)t;
		switch (s->t_)
		{
		case	SimpleDefType::boolType : 
			{
				goFile_<<indent()<<inner<<".WriteBool("<<fieldName<<")"<<std::endl;
				break;
			} 
		case	SimpleDefType::uint8Type : 
			{
				goFile_<<indent()<<inner<<".WriteUInt8("<<fieldName<<")"<<std::endl;
				break;
			} 
		case	SimpleDefType::int8Type : 
			{
				goFile_<<indent()<<inner<<".WriteInt8("<<fieldName<<")"<<std::endl;
				break;
			} 
		case	SimpleDefType::uint16Type :
			{
				goFile_<<indent()<<inner<<".WriteUInt16("<<fieldName<<")"<<std::endl;
				break;
			} 
		case	SimpleDefType::int16Type :
			{
				goFile_<<indent()<<inner<<".WriteInt16("<<fieldName<<")"<<std::endl;
				break;
			} 
		case	SimpleDefType::uint32Type :
			{
				goFile_<<indent()<<inner<<".WriteUInt32("<<fieldName<<")"<<std::endl;
				break;
			} 
		case	SimpleDefType::int32Type :
			{
				goFile_<<indent()<<inner<<".WriteInt32("<<fieldName<<")"<<std::endl;
				break;
			} 
		case	SimpleDefType::int64Type :
			{
				goFile_<<indent()<<inner<<".WriteInt64("<<fieldName<<")"<<std::endl;
				break;
			} 
		case	SimpleDefType::floatType :
			{
				goFile_<<indent()<<inner<<".WriteFloat("<<fieldName<<")"<<std::endl;
				break;
			} 
		case	SimpleDefType::stringType :
			{
				goFile_<<indent()<<inner<<".WriteString("<<fieldName<<")"<<std::endl;
				break;
			}

		}
	}
	else if(t->is_array())
	{
		goFile_<<indent()<<inner<<".WriteUInt16(uint16(len("<<fieldName<<")))"<<std::endl;
		goFile_<<indent()<<"for _ ,v := range "<<fieldName<<" {"<<std::endl;
		indent_up();
		serializeField(((ArrayDefType*)t)->valueDef_,"v",inner);
		indent_down();
		goFile_<<indent()<<"}"<<std::endl;

	}
	else if (t->is_enum())
	{
		goFile_<<indent()<<inner<<".WriteInt16("<<fieldName<<")"<<std::endl;

	}
	else if(t->is_map())
	{
		goFile_<<indent()<<inner<<".WriteUInt16(uint16(len("<<fieldName<<")))"<<std::endl;
		goFile_<<indent()<<"for k ,v := range "<<fieldName<<" {"<<std::endl;
		indent_up();
		serializeField(((MapDefType*)t)->keyDef_,"k",inner);
		serializeField(((MapDefType*)t)->valueDef_,"v",inner);
		indent_down();
		goFile_<<indent()<<"}"<<std::endl;
	}
}


void GoGenerator::genFunAgrList( std::ofstream& stream,StructDefType* agrList,ParamType paramType)
{
	bool first=true;
	for (auto& it:agrList->members_)
	{
		if (!first)
		{
			stream<<",";
		}
		if (paramType==PT_Value)
		{
			stream<<it->name_;
		}
		else if(paramType==PT_Type)
		{
			stream<<typeName(it->type_);
		}
		else if(paramType==PT_TypeValue)
		{
			stream<<it->name_<<"  "<<typeName(it->type_);
		}
		first = false;
	}
}



void GoGenerator::deSerializeField( DefType* t ,const std::string& fieldName  ,const std::string& inner)
{
	if (t->is_struct())
	{
		goFile_<<indent()<<fieldName<<".DeSerialize("<<inner<<")"<<std::endl;
	}
	else if (t->is_simple_type())
	{
		SimpleDefType* s=(SimpleDefType*)t;
		switch (s->t_)
		{
		case	SimpleDefType::boolType : 
			{
				goFile_<<indent()<<fieldName<<"="<<inner<<".ReadBool()"<<std::endl;
				break;
			} 
		case	SimpleDefType::uint8Type : 
			{
				goFile_<<indent()<<fieldName<<"="<<inner<<".ReadUInt8() "<<std::endl;
				break;
			} 
		case	SimpleDefType::int8Type : 
			{
				goFile_<<indent()<<fieldName<<"="<<inner<<".ReadInt8()"<<std::endl;
				break;
			} 
		case	SimpleDefType::uint16Type :
			{
				goFile_<<indent()<<fieldName<<"="<<inner<<".ReadUInt16()"<<std::endl;
				break;
			} 
		case	SimpleDefType::int16Type :
			{
				goFile_<<indent()<<fieldName<<"="<<inner<<".ReadInt16()"<<std::endl;
				break;
			} 
		case	SimpleDefType::uint32Type :
			{
				goFile_<<indent()<<fieldName<<"="<<inner<<".ReadUInt32()"<<std::endl;
				break;
			} 
		case	SimpleDefType::int32Type :
			{
				goFile_<<indent()<<fieldName<<"="<<inner<<".ReadInt32()"<<std::endl;
				break;
			} 
		case	SimpleDefType::int64Type :
			{
				goFile_<<indent()<<fieldName<<"="<<inner<<".ReadInt64()"<<std::endl;
				break;
			} 
		case	SimpleDefType::floatType :
			{
				goFile_<<indent()<<fieldName<<"="<<inner<<".ReadFloat()"<<std::endl;
				break;
			} 
		case	SimpleDefType::stringType :
			{
				goFile_<<indent()<<fieldName<<"="<<inner<<".ReadString()"<<std::endl;
				break;
			}
		}
	}
	else if(t->is_array())
	{
		static int i=0;
		std::stringstream str;
		str<<"_n_"<<i<<"_array";
		goFile_<<indent()<<str.str()<<":="<<inner<<".ReadUInt16()"<<std::endl;
		std::stringstream count;count<<"_i_"<<i<<"_";
		i++;

		goFile_<<indent()<<"for "<<count.str()<<":=0; "<<"uint16("<<count.str()<<")<"<<str.str()<<"; "<<count.str()<<"++ {"<<std::endl;
		indent_up();
		goFile_<<indent()<<"var tmp "<<typeName(((ArrayDefType*)t)->valueDef_)<<std::endl;
		deSerializeField(((ArrayDefType*)t)->valueDef_,"tmp",inner);
		goFile_<<indent()<<fieldName<<"= append("<<fieldName<<",tmp)"<<std::endl;
		indent_down();
		goFile_<<indent()<<"}"<<std::endl;

	}else if (t->is_enum())
	{
		goFile_<<indent()<<fieldName<<"="<<inner<<".ReadInt16()"<<std::endl;

	}else if(t->is_map())
	{
		static int i=0;
		std::stringstream str;
		str<<"_n_"<<i<<"_map";
		goFile_<<indent()<<str.str()<<":="<<inner<<".ReadUInt16()"<<std::endl;
		std::stringstream count;count<<"_i_"<<i<<"_";
		i++;

		goFile_<<indent()<<"for "<<count.str()<<":=0; "<<"uint16("<<count.str()<<")<"<<str.str()<<"; "<<count.str()<<"++ {"<<std::endl;
		indent_up();
		//key
		goFile_<<indent()<<"var tmpKey "<<typeName(((MapDefType*)t)->keyDef_)<<std::endl;
		deSerializeField(((MapDefType*)t)->keyDef_,"tmpKey",inner);

		//value
		goFile_<<indent()<<"var tmpValue "<<typeName(((MapDefType*)t)->valueDef_)<<std::endl;
		deSerializeField(((MapDefType*)t)->valueDef_,"tmpValue",inner);

		goFile_<<indent()<<fieldName<<"[tmpKey]=tmpValue"<<std::endl;
		indent_down();
		goFile_<<indent()<<"}"<<std::endl;

	}
}


std::string GoGenerator::typeName(DefType* t)
{
	if(t->is_array())
	{
		ArrayDefType* array=(ArrayDefType*)t;
		std::string temp="[]"+typeName(array->valueDef_); 
		return temp;
	}
	else if(t->is_map())
	{
		MapDefType* mapType=(MapDefType*)t;
		std::string temp="map["+typeName( mapType->keyDef_)+"]"+typeName(mapType->valueDef_); 
		return temp;
	}
	else if (t->is_simple_type())
	{
		SimpleDefType* s=(SimpleDefType*)t;
		switch (s->t_)
		{
		case SimpleDefType::boolType : return "bool";
		case SimpleDefType::uint8Type : return "uint8";
		case SimpleDefType::int8Type : return "int8";
		case SimpleDefType::uint16Type : return "uint16";
		case SimpleDefType::int16Type : return "int16";

		case SimpleDefType::uint32Type : return "uint32";
		case SimpleDefType::int32Type : return "int32";

		case SimpleDefType::int64Type : return "int64";
		case SimpleDefType::floatType : return "float32";
		case SimpleDefType::stringType : return "string";
		default : assert(0&&"type error"); return "";
		}
	}
	else if(t->is_struct())
	{
		return setInitialUpper(t->name_);
	}
	else if (t->is_enum())
	{
		 return "int16";
	}
	else if (t->is_void())
	{
		return "";
	}
	assert(0&&"type error");
	return "";
}




