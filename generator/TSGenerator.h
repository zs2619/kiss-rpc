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

	void serializeFields( StructDefType* t ,const std::string& prefix);
	void deSerializeFields( StructDefType* t ,const std::string& prefix);

	void serializeField( DefType* t ,const std::string& fieldName,const std::string& prefix );
	void deSerializeField( DefType* t ,const std::string& fieldName,const std::string& prefix );

private:
	std::ofstream	tsFile_;
};
#endif