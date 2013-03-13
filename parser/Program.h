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
#include <string>
class	EnumDefType;
class	StructDefType;
class	ServiceDefType;

template <typename T>
class DefVector
{
public:
	T findDefByName(const std::string& name)
	{
		typename std::vector<T>::iterator it=defs_.begin();
		while(it!=defs_.end())
		{
			if((*it)->name_==name)
				return *it;
			++it;
		}
		return NULL;
	}
	bool addDef(T	t);

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
	Program():json_(false){}

	EnumVector			enums_;
	StructVector		structs_;
	ServiceVector		services_;
	bool findDefByName(const std::string& name);

	bool addIncludeFile(const std::string& includeName);
	std::vector<std::string>	include_;
	std::string			fileName_; //<输入文件名
	std::string			baseName_; //文件名
	std::string			inputDir_;	//<输入目录
	std::string			outputDir_; //<输出文件目录
	bool				json_;		//<序列化json
};

template <typename T>
bool DefVector<T>::addDef(T	t)
{
	assert(t);
	if(Program::inst()->findDefByName(t->name_))
		return false;
	defs_.push_back(t);
	return true;
}
#endif