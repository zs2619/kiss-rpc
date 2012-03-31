//==============================================
/**
	@date:		2012:3:31  
	@file: 		EnumMap.h
	@author		zhangshuai
*/
//==============================================
#ifndef		__ENUMMAP_H__
#define		__ENUMMAP_H__

#define enum(T)	(enum##T)

class EnumMap
{
public:
	typedef void (*Fun)(EnumMap*);
	EnumMap(Fun f)
	{
		f(this);
	}
	int	getIdByName(const std::stirng& name)
	{
		for (int i=0;i<v.size();i++)
		{
			if (v[i]=name)
			{
				return i;
			}
		}
		return -1;
	}
	bool addEnumValue(const std::string& v)
	{
		v.push_back(v);
	}
private:
	std::vector<std::string>	v;
};

#endif