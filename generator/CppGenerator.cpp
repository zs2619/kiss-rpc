//==============================================
/**
	@date:		2012:3:29  
	@file: 		CppGenerator.cpp
	@author: 	zhangshuai
*/
//==============================================
#include "CppGenerator.h"

CppGenerator::CppGenerator( Program* pro,const std::string& name ) :Generator(pro,name)
{

}

void CppGenerator::generateProgram()
{
	std::string headerName=program_->outputDir_+program_->fileName_+".h";
	std::string srcName=program_->outputDir_+program_->fileName_+".cpp";
	headerFile_.open(headerName.c_str());
	srcFile_.open(srcName.c_str());
	generateEnumHeader();
	generateEnumSrc();

	generateStructHeader();
	generateStructSrc();

	headerFile_.close();
	srcFile_.close();
}

void CppGenerator::generateEnumHeader()
{
	std::vector<EnumDefType*>::iterator it=program_->enums_.defs_.begin();
	std::vector<EnumDefType*>::iterator it_end=program_->enums_.defs_.end();
	while(it!=it_end)
	{
		headerFile_<<"enum "<<(*it)->name_<<std::endl;
		headerFile_<<"{ "<<std::endl;
		std::vector<std::string>::iterator it_inner=(*it)->defs_.begin();
		indent_up();
		while(it_inner!=(*it)->defs_.end())
		{
			headerFile_<<indent()<<*it_inner<<","<<std::endl;
			++it_inner;
		}
		indent_down();
		headerFile_<<"} "<<std::endl;
		headerFile_<<"extern EnumMap enum("<<(*it)->name_<<");"<<std::endl;
		headerFile_<<std::endl;
		++it;
	}
}

void CppGenerator::generateEnumSrc()
{
	std::vector<EnumDefType*>::iterator it=program_->enums_.defs_.begin();
	std::vector<EnumDefType*>::iterator it_end=program_->enums_.defs_.end();
	while(it!=it_end)
	{
		srcFile_<<"EnumMap"<<" enum("<<(*it)->name_<<")(enum("<<(*it)->name_<<"Init));"<<std::endl;
		srcFile_<<"void "<<(*it)->name_<<"Init(EnumMap* e)"<<std::endl;
		srcFile_<<"{ "<<std::endl;
		std::vector<std::string>::iterator it_inner=(*it)->defs_.begin();
		indent_up();
		while(it_inner!=(*it)->defs_.end())
		{
			srcFile_<<indent()<<"e.push_back("<<*it_inner<<")"<<";"<<std::endl;
			++it_inner;
		}
		indent_down();
		srcFile_<<"} "<<std::endl;
		srcFile_<<std::endl;
		++it;
	}
}

void CppGenerator::generateStructHeader()
{
	std::vector<StructDefType*>::iterator it=program_->structs_.defs_.begin();
	std::vector<StructDefType*>::iterator it_end=program_->structs_.defs_.end();
	while(it!=it_end)
	{
		headerFile_<<"struct "<<(*it)->name_<<std::endl;
		headerFile_<<"{ "<<std::endl;
		indent_up();

		std::vector<FieldDefType*>::iterator it_inner=(*it)->members_.begin();
		indent_up();
		while(it_inner!=(*it)->members_.end())
		{
			FieldDefType*& t=*it_inner;
			if(t->type_->is_array())
			{

			}else if(t->type_->is_map())
			{

			}else if (t->type_->is_simple_type())
			{
			}

			headerFile_<<indent()<<"e.push_back("<<*it_inner<<")"<<";"<<std::endl;
			++it_inner;
		}
		indent_down();
		headerFile_<<"} "<<std::endl;
		++it;
	}

}

void CppGenerator::generateStructSrc()
{

}
