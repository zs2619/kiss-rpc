//==============================================
/**
	@date:		2012:3:27  
	@file: 		Program.cpp
	@author: 	zhangshuai
*/
//==============================================

#include "Program.h"
#include "DefType.h"

bool Program::addIncludeFile( const std::string& baseName )
{
	auto it = std::find(include_.begin(), include_.end(), baseName);
	if (it != include_.end())
		return false;

	include_.push_back(baseName);
    contexts_[baseName]=new Context(baseName);
	return true;
}

Context * Program::getFileContext(const std::string & name) {
	auto it = contexts_.find(name);
	if (it == contexts_.end())
		return nullptr;
	return it->second;
}

Context * Program::getGenerateContext() {
	auto it = contexts_.find(getBaseName());
	if (it == contexts_.end()) {
		return nullptr;
	} else {
		return contexts_[getBaseName()];
	}
}

bool Program::parseArg(int argc,char** argv) 
{
	return false;
}

bool Program::findDefByName( const std::string& name )
{
	if(enums_.findDefByName(name))
		return true;
	if(structs_.findDefByName(name))
		return true;
	if(services_.findDefByName(name))
		return true;
	return false;
}
