
#ifndef	__MISC_H__
#define	__MISC_H__

#include <algorithm>
#ifdef WIN32
#include <direct.h>

#else
#include <sys/stat.h>
#endif

namespace misc
{
inline int	mkdir(const char* fileName)
	{
#ifdef WIN32
		return ::_mkdir(fileName);
#else
	return  ::mkdir(fileName, S_IRWXU | S_IRWXG | S_IRWXO);
#endif
	}

inline	std::string stringToUpper(std::string& str)
	{
		std::string upperStr;
		upperStr.resize(str.size());
		std::transform(str.begin(),str.end(),upperStr.begin(),::toupper);
		return std::move(upperStr);
	}
}

#endif

