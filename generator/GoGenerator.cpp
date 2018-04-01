//==============================================
/**
	@date:		2015:8:14  
	@file: 		GoGenerator.cpp
	@author: 	zhangshuai
*/
//==============================================
#include "GoGenerator.h"
#include "../md5.h"
#include "../misc.h"
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
	if (program_->enums_.defs_.empty())
		return;

	for(auto& it :program_->enums_.defs_)
	{
		if(it->fileName_!=program_->fileName_)
		{
			 //包含头文件 不生成代码
			 continue; 
		}
		//创建目录
		std::string dirName=program_->outputDir_+it->name_+"/";
		misc::mkdir(dirName.c_str());

		std::string name=dirName+it->name_+".go";
		goFile_.open(name.c_str());
		goFile_<<indent()<<"package "<<it->name_<<std::endl;
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
				goFile_<<indent()<<inner<<std::endl;
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
	if (program_->structs_.defs_.empty())
		return;

	for(auto& it :program_->structs_.defs_)
	{
		if(it->fileName_!=program_->fileName_)
		{
			//包含头文件 不生成代码
			continue; 
		}

		std::string name=program_->outputDir_+it->name_+".go";
		goFile_.open(name.c_str());
		goFile_<<indent()<<"package "<<"rpc"<<std::endl;

		///struct 
		//fingerprint
		goFile_<<std::endl;
		goFile_<<indent()<<"const "<<it->name_<<"_"<<"strFingerprint=\""<<md5(it->getFingerPrint())<<"\""<<std::endl;
		goFile_<<indent()<<"type "<<it->name_<<" struct{"<<std::endl;
		indent_up();

		//属性
		for(auto& inner:it->members_)
		{
			goFile_<<indent()<<setInitialUpper(inner->name_)<<" "<<typeName(inner->type_)<<" `name:\""<<inner->name_<<"\"`"<<std::endl;
		}

		indent_down();
		goFile_<<indent()<<"}" <<std::endl;

		//struct id
		goFile_<<indent()<<"func (this *"<<it->name_<<") GetFingerprint() string{"<<std::endl;
		indent_up();
		goFile_<<indent()<<"return "<<it->name_<<"_"<<"strFingerprint"<<std::endl;
		indent_down();
		goFile_<<"}"<<std::endl;

		//struct 序列化
		goFile_<<indent()<<"func (this *"<<it->name_<<") Serialize( P__ IProtocol){"<<std::endl;
		indent_up();
		for(auto& inner:it->members_)
		{
			serializeField(inner->type_,"this."+setInitialUpper(inner->name_),"P__");
			goFile_<<std::endl;
		}
		indent_down();
		goFile_<<"}"<<std::endl;

		//struct 反序列化
		goFile_<<indent()<<"func (this *"<<it->name_<<") DeSerialize( P__ IProtocol) bool{"<<std::endl;
		indent_up();
		for(auto& inner:it->members_)
		{
			deSerializeField(inner->type_,"this."+setInitialUpper(inner->name_));
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
	if (program_->services_.defs_.empty())
		return;
	std::string fileName=program_->outputDir_+program_->baseName_+"Stub.go";
	goFile_.open(fileName.c_str());
	goFile_<<indent()<<"package "<<"rpc"<<std::endl;
	goFile_<<std::endl;
	for (auto& it:program_->services_.defs_)
	{
		if(it->fileName_!=program_->fileName_)
		{
			 //包含头文件 不生成代码
			continue; 
		}
		//
		goFile_<<indent()<<"const "<<it->name_<<"_"<<"strFingerprintStub=\""<<md5(it->getFingerPrint())<<"\""<<std::endl;
		std::string ifName=it->name_+"Stub";
		goFile_<<indent()<<"type "<<ifName<<" struct {"<<std::endl;
		indent_up();
		goFile_<<indent()<<"P__ IProtocol"<<std::endl;
		indent_down();
		goFile_<<indent()<<"}"<<std::endl;
		goFile_<<std::endl;
		
		//get id
		goFile_<<indent()<<"func (this *"<<it->name_<<"Stub) GetFingerprint() string{"<<std::endl;
		indent_up();
		goFile_<<indent()<<"return "<<it->name_<<"_"<<"strFingerprintStub"<<std::endl;
		indent_down();
		goFile_<<indent()<<"}"<<std::endl;
		
		//stub func
		int i=0;
		for(auto& inner:it->funs_)
		{
			goFile_<<indent()<<"func (this *"<<it->name_<<"Stub) "<<setInitialUpper(inner->name_)<<" (";
			genFunAgrList(goFile_,inner->argrs_);
			goFile_<<") {"<<std::endl;
			indent_up();
			goFile_<<std::endl;
			goFile_<<indent()<<"this.P__.WriteMsgBegin()"<<std::endl;
			goFile_<<std::endl;
			goFile_<<indent()<<"this.P__.WriteUInt16("<<i++<<")"<<std::endl;
			serializeFields(inner->argrs_);
			goFile_<<std::endl;
			goFile_<<indent()<<"this.P__.WriteMsgEnd()"<<std::endl;
			indent_down();
			goFile_<<indent()<<"}"<<std::endl;
		}

		goFile_<<indent()<<std::endl;

	}
	goFile_.close();
}
void GoGenerator::genServiceProxy()
{
	if (program_->services_.defs_.empty())
		return;

	//接口文件
	std::string fileName=program_->outputDir_+"I"+program_->baseName_+"Proxy.go";
	goFile_.open(fileName.c_str());
	goFile_<<indent()<<"package "<<"rpc"<<std::endl;
	goFile_<<std::endl;
	for (auto& it:program_->services_.defs_)
	{
		if(it->fileName_!=program_->fileName_)
		{
			//包含头文件 不生成代码
			continue; 
		}
		std::string ifName="I"+it->name_+"Proxy";
		goFile_<<indent()<<"type "<<ifName<<" interface {"<<std::endl;
		indent_up();
		for(auto& inner:it->funs_)
		{
			goFile_<<indent()<<setInitialUpper(inner->name_)<<" (";
			genFunAgrList(goFile_,inner->argrs_);
			goFile_<<") bool";
			goFile_<<std::endl;
		}
		indent_down();
		goFile_<<indent()<<"}"<<std::endl;
		goFile_<<std::endl;
	}
	goFile_.close();

	//
	fileName=program_->outputDir_+program_->baseName_+"Proxy.go";
	goFile_.open(fileName.c_str());
	goFile_<<indent()<<"package "<<"rpc"<<std::endl;
	goFile_<<std::endl;
	for (auto& it:program_->services_.defs_)
	{
		if(it->fileName_!=program_->fileName_)
		{
			//包含头文件 不生成代码
			continue; 
		}
		std::string ifName=it->name_+"Proxy";
		goFile_<<indent()<<"const "<<it->name_<<"_"<<"strFingerprintProxy=\""<<md5(it->getFingerPrint())<<"\""<<std::endl;
		goFile_<<indent()<<"type "<<ifName<<" struct {"<<std::endl;
		indent_up();
		goFile_<<indent()<<"I__ I"<<ifName<<std::endl;
		indent_down();
		goFile_<<indent()<<"}"<<std::endl;
		goFile_<<std::endl;

		//get id
		goFile_<<indent()<<"func (this *"<<it->name_<<"Proxy) GetFingerprint() string{"<<std::endl;
		indent_up();
		goFile_<<indent()<<"return "<<it->name_<<"_"<<"strFingerprintProxy"<<std::endl;
		indent_down();
		goFile_<<indent()<<"}"<<std::endl;

		//dispatch
		goFile_<<indent()<<"func (this *"<<it->name_<<"Proxy) "<<"Dispatch"<<"(";
		goFile_<<" P__ IProtocol";
		goFile_<<") bool {"<<std::endl;
		indent_up();
		goFile_<<indent()<<"id :=P__.ReadUInt16()"<<std::endl;
		goFile_<<indent()<<"switch id {"<<std::endl;
		int i=0;
		for(auto& it1:it->funs_)
		{
			goFile_<<indent()<<"case "<<i++<<":"<<std::endl;
			indent_up();
			goFile_<<indent()<<"return "<<"this.recv_"<<it1->name_<<"(P__)"<<std::endl;
			indent_down();
		}
		goFile_<<indent()<<"default:"<<std::endl;
		goFile_<<indent()<<"return false"<<std::endl;


		goFile_<<indent()<<"}//switch "<<std::endl;

		indent_down();
		goFile_<<indent()<<"}//dispatch func"<<std::endl;

		//func 
		for(auto& inner:it->funs_)
		{
			goFile_<<indent()<<"func (this *"<<it->name_<<"Proxy) "<<"recv_"<<inner->name_<<" (";
			goFile_<<" P__ IProtocol ";
			goFile_<<") bool {"<<std::endl;
			indent_up();
			goFile_<<std::endl;
			deSerializeFields(inner->argrs_);

			goFile_<<indent()<<"return this.I__."<<setInitialUpper(inner->name_)<<"(";
			genFunAgrList(goFile_,inner->argrs_,true);
			goFile_<<")"<<std::endl;

			indent_down();
			goFile_<<indent()<<"}"<<std::endl;
		}

	}
	goFile_.close();


}
void GoGenerator::deSerializeFields( StructDefType* t )
{
	for (auto& it :t->members_)
	{
		goFile_<<indent()<<"var "<<it->name_<<" "<<typeName(it->type_)<<std::endl;
		deSerializeField(it->type_,it->name_);
		goFile_<<std::endl;
	}
}
void GoGenerator::serializeFields( StructDefType* t )
{
	for (auto it:t->members_)
	{
		serializeField(it->type_,it->name_,"this.P__");
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

	}else if (t->is_enum())
	{
		goFile_<<indent()<<inner<<".WriteInt16("<<fieldName<<")"<<std::endl;

	}else if(t->is_map())
	{
	}
}


void GoGenerator::genFunAgrList( std::ofstream& stream,StructDefType* agrList,bool onlyValue)
{
	bool frist=true;
	for (auto& it:agrList->members_)
	{
		if (frist)
		{
			if (onlyValue)
			{
				stream<<it->name_;
			} 
			else
			{
				stream<<" "<<it->name_<<"  "<<typeName(it->type_);
			}
			frist=false;
		}
		else
		{
			if (onlyValue)
			{
				stream<<", "<<it->name_;
			} 
			else
			{
				stream<<", "<<it->name_<<"  "<<typeName(it->type_);
			}
		}
	}
}



void GoGenerator::deSerializeField( DefType* t ,const std::string& fieldName )
{
	if (t->is_struct())
	{
		goFile_<<indent()<<fieldName<<".DeSerialize(P__)"<<std::endl;
	}
	else if (t->is_simple_type())
	{
		SimpleDefType* s=(SimpleDefType*)t;
		switch (s->t_)
		{
		case	SimpleDefType::boolType : 
			{
				goFile_<<indent()<<fieldName<<"=P__.ReadBool()"<<std::endl;
				break;
			} 
		case	SimpleDefType::uint8Type : 
			{
				goFile_<<indent()<<fieldName<<"=P__.ReadUInt8() "<<std::endl;
				break;
			} 
		case	SimpleDefType::int8Type : 
			{
				goFile_<<indent()<<fieldName<<"=P__.ReadInt8()"<<std::endl;
				break;
			} 
		case	SimpleDefType::uint16Type :
			{
				goFile_<<indent()<<fieldName<<"=P__.ReadUInt16()"<<std::endl;
				break;
			} 
		case	SimpleDefType::int16Type :
			{
				goFile_<<indent()<<fieldName<<"=P__.ReadInt16()"<<std::endl;
				break;
			} 
		case	SimpleDefType::uint32Type :
			{
				goFile_<<indent()<<fieldName<<"=P__.ReadUInt32()"<<std::endl;
				break;
			} 
		case	SimpleDefType::int32Type :
			{
				goFile_<<indent()<<fieldName<<"=P__.ReadInt32()"<<std::endl;
				break;
			} 
		case	SimpleDefType::int64Type :
			{
				goFile_<<indent()<<fieldName<<"=P__.ReadInt64()"<<std::endl;
				break;
			} 
		case	SimpleDefType::floatType :
			{
				goFile_<<indent()<<fieldName<<"=P__.ReadFloat()"<<std::endl;
				break;
			} 
		case	SimpleDefType::stringType :
			{
				goFile_<<indent()<<fieldName<<"=P__.ReadString()"<<std::endl;
				break;
			}
		}
	}
	else if(t->is_array())
	{
		static int i=0;
		std::stringstream str;
		str<<"_n_"<<i<<"_array";
		goFile_<<indent()<<str.str()<<":=P__.ReadUInt16()"<<std::endl;
		std::stringstream count;count<<"_i_"<<i<<"_";
		i++;

		goFile_<<indent()<<"for "<<count.str()<<":=0; "<<"uint16("<<count.str()<<")<"<<str.str()<<"; "<<count.str()<<"++ {"<<std::endl;
		indent_up();
		goFile_<<indent()<<"var tmp "<<typeName(((ArrayDefType*)t)->valueDef_)<<std::endl;
		deSerializeField(((ArrayDefType*)t)->valueDef_,"tmp");
		goFile_<<indent()<<fieldName<<"= append("<<fieldName<<",tmp)"<<std::endl;
		indent_down();
		goFile_<<indent()<<"}"<<std::endl;

	}else if (t->is_enum())
	{
		goFile_<<indent()<<fieldName<<"=P__.ReadInt16()"<<std::endl;

	}else if(t->is_map())
	{
	}
}


std::string GoGenerator::typeName(DefType* t)
{
	if(t->is_array())
	{
		ArrayDefType* array=(ArrayDefType*)t;
		std::string temp="[]"+typeName(array->valueDef_); 
		return temp;
	}else if(t->is_map())
	{
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
		case	SimpleDefType::floatType : return "float32";
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
		 return "int16";
	}
	assert(0&&"type error"); 
	return "";
}




