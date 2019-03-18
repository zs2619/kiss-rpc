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

	void generateServiceHeader();
	void generateServiceSrc();

	void defineField(FieldDefType* t);
	std::string typeName(DefType* t,bool isAgr=false);
	std::string DefaultValue( DefType* t );

	void serializeField( DefType* t ,const std::string& fieldName ,const std::string& prefix);
	void serializeJsonField( DefType* t ,const std::string& key, const std::string& value);
	void serializeJsonFields( StructDefType* t );
	void deSerializeField( DefType* t ,const std::string& fieldName ,const std::string& prefix);

	void serializeFields( StructDefType* t ,const std::string& prefix);
	void deSerializeFields( StructDefType* t ,const std::string& prefix);

	void genServiceStubHeader();
	void genServiceStubSrc();
	void genServiceProxyHeader();
	void genServiceProxySrc();

	enum ParamType 
	{
		PT_Type,
		PT_Value,
		PT_TypeValue,
	};
	void genFunAgrList(std::ofstream& stream,StructDefType* agrList,ParamType paramType=PT_TypeValue);
	void genFunStubDeclare(ServiceDefType* service);
	void genFunProxyDeclare(ServiceDefType* service);

	void genIncludeHeader(std::ofstream& stream);

private:
	std::ofstream	headerFile_;
	std::ofstream	srcFile_;	
	std::ofstream	ifFile_;
};

#endif