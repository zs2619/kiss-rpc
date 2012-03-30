//==============================================
/**
	@date:		2012:3:29  
	@file: 		As3Generator.cpp
	@author: 	zhangshuai
*/
//==============================================
#include "As3Generator.h"

void As3Generator::generateProgram()
{
	generateEnum();
}

void As3Generator::generateEnum()
{
	std::vector<EnumDefType*>::iterator it=program_->enums_.defs_.begin();
	std::vector<EnumDefType*>::iterator it_end=program_->enums_.defs_.end();
	while(it!=it_end)
	{
		std::string name=program_->outputDir_+(*it)->name_+".as";
		as3File_.open(name.c_str());
		as3File_<<"public class "<<(*it)->name_<<std::endl;
		as3File_<<"{"<<std::endl;
		indent_up();
		int i=0;
		std::vector<std::string>::iterator it_inner=(*it)->defs_.begin();
		while(it_inner!=(*it)->defs_.end())
		{
			as3File_<<indent()<<"public const static var "<<*it_inner<<":uint ="<<i<<";"<<std::endl;
			i++;
			++it_inner;
		}
		indent_down();
		as3File_<<"}"<<std::endl;
		as3File_.close();
		++it;
	}
}