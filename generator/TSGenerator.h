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

	void serializeFields( StructDefType* t ,const std::string& prefix,bool needThis=true);
	void deSerializeFields( StructDefType* t ,const std::string& prefix,bool needThis=true);

	void serializeField( DefType* t ,const std::string& fieldName,const std::string& prefix );
	void deSerializeField( DefType* t ,const std::string& fieldName,const std::string& prefix );
	void genFunAgrList(std::ofstream& stream,StructDefType* agrList,ParamType paramType=PT_TypeValue);

private:
	std::ofstream	tsFile_;
};
#endif