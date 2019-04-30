//==============================================
/**
	@date:		2019:4:30 
	@file: 		TSGenerator.h
	@author		zhangshuai
*/
//==============================================
#ifndef	__TSGENERATOR_H__
#define	__TSGENERATOR_H__
#include "Generator.h"

class TSGenerator: public Generator
{
public:
	TSGenerator(Program* pro,const std::string& name);

	virtual void generateProgram();

private:
	void generateEnum();
	void generateStruct();
	void generateService();

	std::string typeName(DefType* t,bool isAgr=false);
	std::string defaultValue( DefType* t );

private:
	std::ofstream	tsFile_;
};
#endif