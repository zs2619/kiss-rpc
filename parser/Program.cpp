//==============================================
/**
	@date:		2012:3:27  
	@file: 		Program .cpp
	@author: 	zhangshuai
*/
//==============================================

#include "Program.h"
#include "DefType.h"

bool Program::addIncludeFile( const std::string& includeName )
{
	std::vector<std::string>::iterator it =include_.begin();
	while (it!=include_.end())
	{
		if (*it==includeName)
		{
			return false;
		}
		++it;
	}
	include_.push_back(includeName);
	return true;
}