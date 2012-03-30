//==============================================
/**
	@date:		2012:3:29  
	@file: 		CppGenerator.h
	@author		zhangshuai
*/
//==============================================

#ifndef	__CPPGENERATOR_H__
#define	__CPPGENERATOR_H__
#include "Generator.h"
class CppGenerator: public Generator
{
public:
	CppGenerator(Program* pro,const std::string& name);

	virtual void generateProgram();

private:
	void generateEnumHeader();
	void generateEnumSrc();

	void generateStructHeader();
	void generateStructSrc();
private:
	std::ofstream	headerFile_;
	std::ofstream	srcFile_;
};

#endif