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

	std::string name_;
};

class EnumDefType: public DefType
{
public:
	virtual bool is_enum() const { return true; }
	bool	addEnumValue(const std::string& value);
	bool	fineValueByName(const std::string& value);
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
	dataType t_;
};

class StructDefType:public DefType
{
public:
	virtual bool is_struct()    const { return true; }
	bool	addStructValue(FieldDefType* value);
	bool	fineValueByName(const std::string& value);

	std::vector<FieldDefType*>		members_;
};

class MapDefType:public DefType
{
public:
	virtual bool is_map()       const { return true; }
	SimpleDefType* keyDef_;
	DefType*	   valueDef_;
};

class ArrayDefType:public DefType
{
public:
	virtual bool is_array()      const { return true; }
	DefType*	valueDef_;
};

class FuctionDefType: public DefType
{
public:
	virtual bool is_function()   const { return true; }
	StructDefType*	argrs_;
};

class ServiceDefType:public DefType 
{
public:
	virtual bool is_service()   const { return true; }
	bool findFunByName(const std::string& name);
	bool addFunciton(FuctionDefType* fun);

	std::vector<FuctionDefType*>	funs_;
};


#endif