//==============================================
/**
@date:		2012:3:27  
@file: 		DefType.h
@author		zhangshuai
*/
//==============================================

#ifndef		__DEFTYPE_H__
#define		__DEFTYPE_H__

#include <string>
#include <vector>
#include <assert.h>
#include "Program.h"
class Definition
{
};

class DefType : public Definition
{
public:
	virtual bool is_simple_type() const { return false; }
	virtual bool is_enum()      const { return false; }
	virtual bool is_struct()    const { return false; }
	virtual bool is_array()     const { return false; }
	virtual bool is_map()       const { return false; }
	virtual bool is_service()   const { return false; }
	virtual bool is_function()   const { return false; }

	virtual std::string getFingerPrint()=0;

	std::string name_;
	std::string fileName_;
};

class EnumDefType: public DefType
{
public:
	virtual bool is_enum() const { return true; }
	bool	addEnumValue(const std::string& value);
	bool	fineValueByName(const std::string& value);

	std::string getFingerPrint(){return " enum ";}

	std::vector<std::string> defs_;
};

class FieldDefType :public Definition
{
public:
	DefType*	type_;
	std::string name_;
};

class SimpleDefType: public DefType
{
public:
	enum dataType
	{
		boolType ,
		uint8Type  ,
		int8Type  ,
		uint16Type  ,
		int16Type  ,
		uint32Type  ,
		int32Type  ,
		int64Type  ,
		floatType  ,
		stringType  ,
	};
	virtual bool is_simple_type() const { return true; }

	std::string getFingerPrint()
	{
		switch (t_)
		{
		case	SimpleDefType::boolType : return " boolType ";

		case	SimpleDefType::uint8Type : return " uint8Type ";
		case	SimpleDefType::int8Type : return " int8Type ";
		case	SimpleDefType::uint16Type : return " uint16Type ";
		case	SimpleDefType::int16Type : return " int16Type ";

		case	SimpleDefType::uint32Type : return " uint32Type ";
		case	SimpleDefType::int32Type : return " int32Type ";

		case	SimpleDefType::int64Type : return " int64Type ";
		case	SimpleDefType::floatType : return " floatType ";
		case	SimpleDefType::stringType : return " stringType ";
		default          : assert(0&&"type error"); return "";
		}
	}
	dataType t_;
};

class StructDefType:public DefType
{
public:
	virtual bool is_struct()    const { return true; }
	bool	addStructValue(FieldDefType* value);
	bool	fineValueByName(const std::string& value);

	std::string getFingerPrint()
	{ 
		std::string tmp="struct "+name_;
		for (size_t i=0;i<members_.size();i++)
		{
			if(members_[i]->type_->is_struct())
			{
				StructDefType* tmpStruct=Program::inst()->structs_.findDefByName(members_[i]->type_->name_);
				tmp+=tmpStruct->getFingerPrint();
			}
			else
			{
				tmp+=members_[i]->type_->getFingerPrint();
			}
		}
		return tmp;
	}
	std::vector<FieldDefType*>		members_;
};

class MapDefType:public DefType
{
public:
	virtual bool is_map()       const { return true; }

	SimpleDefType* keyDef_;
	DefType*	   valueDef_;
	std::string getFingerPrint() { assert(0); return "";}
};

class ArrayDefType:public DefType
{
public:
	virtual bool is_array()      const { return true; }
	std::string getFingerPrint() 
	{
		return "array"+valueDef_->getFingerPrint();
	}
	DefType*	valueDef_;
};

class FuctionDefType: public DefType
{
public:
	virtual bool is_function()   const { return true; }
	std::string getFingerPrint() 
	{
		return " fuction "+name_+argrs_->getFingerPrint();
	}
	StructDefType*	argrs_;
};

class ServiceDefType:public DefType 
{
public:
	virtual bool is_service()   const { return true; }
	bool findFunByName(const std::string& name);
	bool addFunciton(FuctionDefType* fun);
	std::string getFingerPrint() 
	{
		std::string tmp=" service "+name_;
		for (size_t i=0;i<funs_.size();i++)
		{
			tmp+=funs_[i]->getFingerPrint();
		}
		return tmp;
	}

	std::vector<FuctionDefType*>	funs_;
};


#endif