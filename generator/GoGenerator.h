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

/** go backend*/
class GoGenerator: public Generator
{
public:
	GoGenerator(Program* pro,const std::string& name);

	virtual void generateProgram();

private:
	void generateEnum();
	void generateStruct();
	void generateService();

private:
	std::string typeName(DefType* t);
	void serializeField( DefType* t ,const std::string& fieldName ,const std::string& inner);
	void deSerializeField( DefType* t ,const std::string& fieldName ,const std::string& inner);
	void serializeFields( StructDefType* t ,std::string s);
	void deSerializeFields( StructDefType* t ,std::string s);
	void genServiceStub();
	void genServiceProxy();
	void genFunAgrList(std::ofstream& stream,StructDefType* agrList,ParamType paramType=PT_TypeValue);

private:

	std::ofstream	goFile_;
};

#endif