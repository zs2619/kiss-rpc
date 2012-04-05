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

/** Éú³Écpp ºó¶Ë*/
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

	void defineField(FieldDefType* t);
	std::string typeName(DefType* t);
	std::string DefaultValue( DefType* t );

	void serializeField( DefType* t ,const std::string& fieldName );
	void deSerializeField( DefType* t ,const std::string& fieldName );
private:
	std::ofstream	headerFile_;
	std::ofstream	srcFile_;
};

#endif