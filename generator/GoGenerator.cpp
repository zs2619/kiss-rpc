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
	for(auto& it :program_->enums_.defs_)
	{
		if(it->fileName_!=program_->fileName_)
		{
			//这个代码是啥意思来 忘了?
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
				goFile_<<indent()<<inner<<" =iota "<<std::endl;
				flag=false;
			}
			goFile_<<indent()<<inner<<std::endl;
		}

		indent_down();
		goFile_<<indent()<<")"<<std::endl;
		goFile_.close();
	}
}

void GoGenerator::generateStruct()
{
	std::string name=program_->outputDir_+program_->baseName_+".go";
	goFile_.open(name.c_str());
	goFile_<<indent()<<"package "<<program_->baseName_<<std::endl;
	//import package
	goFile_<<"import("<<std::endl;
	goFile_<<" \"IProtocol\""<<std::endl;
	goFile_<<")"<<std::endl;


	for(auto& it :program_->structs_.defs_)
	{
		if(it->fileName_!=program_->fileName_)
		{
			//这个代码是啥意思来 忘了?
			continue; 
		}
		///struct 
		//fingerprint
		goFile_<<std::endl;
		goFile_<<indent()<<"const "<<it->name_<<"_"<<"strFingerprint=\""<<md5(it->getFingerPrint())<<"\""<<std::endl;
		goFile_<<indent()<<"type "<<it->name_<<" struct{"<<std::endl;
		indent_up();

		//属性
		for(auto& inner:it->members_)
		{
			goFile_<<indent()<<inner->name_<<" "<<typeName(inner->type_)<<std::endl;
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
		goFile_<<indent()<<"func (this *"<<it->name_<<") Serialize(IProtocol __P__){"<<std::endl;
		indent_up();
		for(auto& inner:it->members_)
		{
			serializeField(inner->type_,inner->name_);
			goFile_<<std::endl;
		}
		indent_down();
		goFile_<<"}"<<std::endl;

		//struct 反序列化
		goFile_<<indent()<<"func (this *"<<it->name_<<") DeSerialize(IProtocol __P__) bool{"<<std::endl;
		indent_up();
		for(auto& inner:it->members_)
		{
			deSerializeField(inner->type_,inner->name_);
			goFile_<<std::endl;
		}
		goFile_<<indent()<<"return true"<<std::endl;
		indent_down();

		goFile_<<"}"<<std::endl;

		goFile_<<std::endl;
	}
	goFile_.close();
}

void GoGenerator::generateService()
{

}


void GoGenerator::deSerializeField( DefType* t ,const std::string& fieldName )
{
	if (t->is_struct())
	{
		goFile_<<indent()<<"if !"<<fieldName<<".deSerialize(__P__) {return false};"<<std::endl;
	}
	else if (t->is_simple_type())
	{
		SimpleDefType* s=(SimpleDefType*)t;
		switch (s->t_)
		{
		case	SimpleDefType::boolType : 
			{
				goFile_<<indent()<<"if !"<<"__P__->readBool("<<fieldName<<") {return false};"<<std::endl;
				break;
			} 
		case	SimpleDefType::uint8Type : 
			{
				goFile_<<indent()<<"if !"<<"__P__->readUInt8("<<fieldName<<") {return false};"<<std::endl;
				break;
			} 
		case	SimpleDefType::int8Type : 
			{
				goFile_<<indent()<<"if !"<<"__P__->readInt8("<<fieldName<<") {return false};"<<std::endl;
				break;
			} 
		case	SimpleDefType::uint16Type :
			{
				goFile_<<indent()<<"if !"<<"__P__->readUInt16("<<fieldName<<") {return false};"<<std::endl;
				break;
			} 
		case	SimpleDefType::int16Type :
			{
				goFile_<<indent()<<"if !"<<"__P__->readInt16("<<fieldName<<") {return false}"<<std::endl;
				break;
			} 
		case	SimpleDefType::uint32Type :
			{
				goFile_<<indent()<<"if !"<<"__P__->readUInt32("<<fieldName<<") {return false};"<<std::endl;
				break;
			} 
		case	SimpleDefType::int32Type :
			{
				goFile_<<indent()<<"if !"<<"__P__->readInt32("<<fieldName<<") {return false};"<<std::endl;
				break;
			} 
		case	SimpleDefType::int64Type :
			{
				goFile_<<indent()<<"if !"<<"__P__->readInt64("<<fieldName<<") {return false};"<<std::endl;
				break;
			} 
		case	SimpleDefType::floatType :
			{
				goFile_<<indent()<<"if !"<<"__P__->readFloat("<<fieldName<<") {return false};"<<std::endl;
				break;
			} 
		case	SimpleDefType::stringType :
			{
				goFile_<<indent()<<"if !"<<"__P__->readString("<<fieldName<<") {return false};"<<std::endl;
				break;
			}
		}
	}
	else if(t->is_array())
	{
		std::string size="_n_"+fieldName+"_array";
		goFile_<<indent()<<size<<" :=0;"<<std::endl;
		goFile_<<indent()<<"if !"<<"__P__->readUInt16("<<size<<") {return false};"<<std::endl;

		goFile_<<indent()<<fieldName<<".resize( "<<size<<");"<<std::endl;
		std::string count="_i_"+fieldName+"_";
		goFile_<<indent()<<"for (size_t "<<count<<"=0;"<<count<<"<"<<size<<";"<<count<<"++){"<<std::endl;
		indent_up();
		goFile_<<indent()<<"var tmp "<<typeName(((ArrayDefType*)t)->valueDef_)<<std::endl;
		deSerializeField(((ArrayDefType*)t)->valueDef_,"tmp");
		goFile_<<indent()<<fieldName<<"= append("<<fieldName<<",tmp)"<<std::endl;
		indent_down();
		goFile_<<indent()<<"}"<<std::endl;

	}else if (t->is_enum())
	{
		std::string tempName=" __temp_enum__"+fieldName;
		goFile_<<indent()<<"int16 "<<tempName<<" =0;"<<std::endl;
		goFile_<<indent()<<"if(!"<<"__P__->readInt16((int16&)"<<tempName<<")) {return false};"<<std::endl;
		goFile_<<indent()<<fieldName<<"=("<<typeName(t)<<")"<<tempName<<";"<<std::endl;

	}else if(t->is_map())
	{
	}
}


void GoGenerator::serializeField( DefType* t ,const std::string& fieldName )
{
	if (t->is_struct())
	{
		goFile_<<indent()<<fieldName<<".serialize(__P__);"<<std::endl;
	}
	else if (t->is_simple_type())
	{
		SimpleDefType* s=(SimpleDefType*)t;
		switch (s->t_)
		{
		case	SimpleDefType::boolType : 
			{
				goFile_<<indent()<<"__P__.writeBool("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::uint8Type : 
			{
				goFile_<<indent()<<"__P__.writeUInt8("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::int8Type : 
			{
				goFile_<<indent()<<"__P__.writeInt8("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::uint16Type :
			{
				goFile_<<indent()<<"__P__.writeUInt16("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::int16Type :
			{
				goFile_<<indent()<<"__P__.writeInt16("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::uint32Type :
			{
				goFile_<<indent()<<"__P__.writeUInt32("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::int32Type :
			{
				goFile_<<indent()<<"__P__.writeInt32("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::int64Type :
			{
				goFile_<<indent()<<"__P__.writeInt64("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::floatType :
			{
				goFile_<<indent()<<"__P__.writeFloat("<<fieldName<<");"<<std::endl;
				break;
			} 
		case	SimpleDefType::stringType :
			{
				goFile_<<indent()<<"__P__.writeString("<<fieldName<<");"<<std::endl;
				break;
			}

		}
	}
	else if(t->is_array())
	{
		goFile_<<indent()<<"__P__.writeUInt16("<<fieldName<<".size());"<<std::endl;

		goFile_<<indent()<<"for i,v :range "<<fieldName<<" {"<<std::endl;
		indent_up();
		serializeField(((ArrayDefType*)t)->valueDef_,"v");
		indent_down();
		goFile_<<indent()<<"}"<<std::endl;

	}else if (t->is_enum())
	{
		goFile_<<indent()<<"__P__.writeInt16("<<fieldName<<");"<<std::endl;

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




