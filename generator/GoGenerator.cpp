//==============================================
/**
	@date:		2015:8:14  
	@file: 		GoGenerator.cpp
	@author: 	zhangshuai
*/
//==============================================
#include "GoGenerator.h"
#include "../md5.h"
#include "../misc.h"

GoGenerator::GoGenerator( Program* pro,const std::string& name ) :Generator(pro,name)
{

}

void GoGenerator::generateProgram()
{
	generateEnum();
	generateStruct();
	generateService();
}

void GoGenerator::generateEnum()
{
	if (program_->enums_.defs_.empty())
		return;
	for(auto& it :program_->enums_.defs_)
	{
		if(it->fileName_!=program_->fileName_)
		{
			 continue; 
		}
		//´´½¨Ä¿Â¼
		std::string dirName=program_->outputDir_+it->name_+"/";
		misc::mkdir(dirName.c_str());

		std::string name=dirName+it->name_+".go";
		goFile_.open(name.c_str());
		goFile_<<indent()<<"package "<<it->name_<<std::endl;
		goFile_<<indent()<<"const ("<<std::endl;
		indent_up();

		bool flag=true;
		for (auto& inner:it->defs_)
		{
			if (flag)
			{
				goFile_<<indent()<<inner<<" =iota "<<std::endl;
				flag=false;
			}
			goFile_<<indent()<<inner<<std::endl;
		}

		indent_down();
		goFile_<<indent()<<")"<<std::endl;
		goFile_.close();
	}
}

void GoGenerator::generateStruct()
{

}

void GoGenerator::generateService()
{

}



