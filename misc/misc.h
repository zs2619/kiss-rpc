
#ifndef	__MISC_H__
#define	__MISC_H__

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
		return ::mkdir(fileName);
#else
	return  ::mkdir(fileName, S_IRWXU | S_IRWXG | S_IRWXO);
#endif
	}


}

#endif

