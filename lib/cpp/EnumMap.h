//==============================================
/**
	@date:		2012:3:31  
	@file: 		EnumMap.h
	@author		zhangshuai
*/
//==============================================
#ifndef		__ENUMMAP_H__
#define		__ENUMMAP_H__

#include <vector>
#include <string>
#define Enum(T)	(Enum##T)

class EnumMap
{
public:
	typedef void (*Fun)(EnumMap*);
	EnumMap(Fun f)
	{
		f(this);
	}
	int	getIdByName(const std::string& name)
	{
		for (size_t i=0;i<v.size();i++)
		{
			if (v[i]==name)
			{
				return i;
			}
		}
		return -1;
	}
	bool addEnumValue(const std::string& value)
	{
		v.push_back(value);
		return true;
	}
private:
	std::vector<std::string>	v;
};

#endif