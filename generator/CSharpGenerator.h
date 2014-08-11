//==============================================
/**
	@date:		2014:7:23  
	@file: 		CSharpGenerator.h
	@author		tanlei
*/
//==============================================
#ifndef	__CSHARPGENERATOR_H__
#define __CSHARPGENERATOR_H__

#include "Generator.h"

/** 生成C# 后端*/
class CSharpGenerator :public Generator
{
public:
	CSharpGenerator(Program* pro,const std::string& name):Generator(pro,name){}

	virtual void generateProgram();
	void	generateEnum();
	void generateStruct();
	void	generateService();

	void genServiceStub();
	void genServiceProxy();
	void genServiceProxyIf();

	std::string typeName(DefType* t);
	std::string DefaultValue( DefType* t );
	void serializeField( DefType* t ,const std::string& fieldName );
	void deSerializeField( DefType* t ,const std::string& fieldName );

	void genFunAgrList( std::ofstream& stream,StructDefType* agrList,bool onlyValue=false);
	void serializeFields( StructDefType* t );
	void deSerializeFields( StructDefType* t );
	
private:
	std::ofstream	csharpFile_;
	std::ofstream	ifFile_;		//<接口文件
};

#endif
