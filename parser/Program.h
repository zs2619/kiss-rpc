//==============================================
/**
	@date:		2012:3:27  
	@file: 		Program.h
	@author		zhangshuai
*/
//==============================================
#ifndef		__Program_H__
#define		__Program_H__


#include <vector>
#include <assert.h>
class	EnumDefType;
class	StructDefType;
class	ServiceDefType;

template <typename T>
class DefVector
{
public:
	bool findDefByName(const std::string& name)
	{
		std::vector<T>::iterator it=defs_.begin();
		while(it!=defs_.end())
		{
			if((*it)->name_==name)
				return true;
			++it;
		}
		return false;
	}
	bool addDef(T	t)
	{
		assert(t);
		if(findDefByName(t->name_))
			return false;
		defs_.push_back(t);
		return true;
	}

	std::vector<T>   defs_;
};

typedef DefVector<EnumDefType*>		EnumVector;
typedef DefVector<StructDefType*>   StructVector;
typedef DefVector<ServiceDefType*>  ServiceVector;

class Program 
{
public:
	static Program* inst()
	{
		static Program p;
		return &p;
	}

	EnumVector			enums_;
	StructVector		structs_;
	ServiceVector		services_;

	std::string			fileName_; //<输入文件名
	std::string			outputDir_; //<输出文件目录
};
#endif