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

template <typename T>
class DefVector
{
public:
	T findDefByName(const std::string& name)
	{
		auto it = std::find_if(defs_.begin(), defs_.end(), [&] (auto it){	
			if (it->name_ == name)
				return true; 
			return false;

		}
		);
		if (it != defs_.end())
		{
			return *it;
		}
		return NULL;
	}
	bool addDef(T	t) {
		assert(t);
		defs_.push_back(t);
		return true;
	}

	std::vector<T>   defs_;
};

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
	virtual bool is_group()      const { return false; }
	virtual bool is_void()      const { return false; }

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

class VoidDefType: public DefType
{
public:
	virtual bool is_void() const { return true; }
	std::string getFingerPrint(){return " void ";}
};

class StructDefType:public DefType
{
public:
	virtual bool is_struct()    const { return true; }
	bool	addStructValue(FieldDefType* value);
	bool	fineValueByName(const std::string& value);

	std::string getFingerPrint();
	std::vector<FieldDefType*>		members_;
};

class MapDefType:public DefType
{
public:
	virtual bool is_map()       const { return true; }

	SimpleDefType* keyDef_;
	DefType*	   valueDef_;
	std::string getFingerPrint() {  return " map "+keyDef_->getFingerPrint()+valueDef_->getFingerPrint();}
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
	DefType*		result_;
};

class GroupDefType :public Definition
{
public:
	virtual bool is_group()      const { return true; }

	std::vector<FuctionDefType*>	funs_;
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
	std::vector<GroupDefType*>		groups_;
	std::vector<FuctionDefType*>	funs_;
};

typedef DefVector<EnumDefType*>		EnumVector;
typedef DefVector<StructDefType*>   StructVector;
typedef DefVector<ServiceDefType*>  ServiceVector;

class NameSpaceType : public DefType{
public:

	std::string getFingerPrint() { return ""; };

	EnumVector		enums_;
	StructVector	structs_;
	ServiceVector	services_;
};

typedef DefVector<NameSpaceType*>   NameSapceVector;

#endif