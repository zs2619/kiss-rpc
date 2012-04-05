//==============================================
/**
	@date:		2012:3:29  
	@file: 		DefType.cpp
	@author: 	zhangshuai
*/
//==============================================

#include "DefType.h"
bool EnumDefType::fineValueByName(const std::string& value)
{
	std::vector<std::string>::iterator it=defs_.begin();
	while(it!=defs_.end())
	{
		if((*it)==value)
			return true;
		++it;
	}
	return false;

}
bool EnumDefType::addEnumValue( const std::string& value )
{
	if(!fineValueByName(value))
	{
		defs_.push_back(value);
		return true;
	}
	return false;
}


bool StructDefType::addStructValue( FieldDefType* member )
{
	if(!fineValueByName(member->name_))
	{
		members_.push_back(member);
		return true;
	}
	return false;
}

bool StructDefType::fineValueByName( const std::string& value )
{
	std::vector<FieldDefType*>::iterator it=members_.begin();
	while(it!=members_.end())
	{
		if((*it)->name_==value)
			return true;
		++it;
	}
	return false;

}

bool ServiceDefType::addFunciton( FuctionDefType* fun )
{
	if(!findFunByName(fun->name_))
	{
		funs_.push_back(fun);
		return true;
	}
	return false;
}

bool ServiceDefType::findFunByName( const std::string& name )
{
	std::vector<FuctionDefType*>::iterator it=funs_.begin();
	while (it!=funs_.end())
	{
		if((*it)->name_==name)
			return true;
		++it;
	}
	return false;
}
