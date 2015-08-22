//==============================================
/**
	@date:		2015:8:14  
	@file: 		GoGenerator.h
	@author		zhangshuai
*/
//==============================================

#ifndef	__GOGENERATOR_H__
#define	__GOGENERATOR_H__
#include "Generator.h"

/** Éú³Égo ºó¶Ë*/
class GoGenerator: public Generator
{
public:
	GoGenerator(Program* pro,const std::string& name);

	virtual void generateProgram();

private:
	void	generateEnum();
	void    generateStruct();
	void	generateService();

private:
	std::string typeName(DefType* t);
	void serializeField( DefType* t ,const std::string& fieldName );
	void deSerializeField( DefType* t ,const std::string& fieldName );

private:

	std::ofstream	goFile_;
};

#endif