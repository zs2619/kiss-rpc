//==============================================
/**
	@date:		2012:3:29  
	@file: 		As3Generator.h
	@author		zhangshuai
*/
//==============================================
#ifndef	__AS3GENERATOR_H__
#define	__AS3GENERATOR_H__

#include "Generator.h"

/** Éú³Éas3 ºó¶Ë*/
class As3Generator :public Generator
{
public:
	As3Generator(Program* pro,const std::string& name):Generator(pro,name){}

	virtual void generateProgram();
	void	generateEnum();
	void    generateStruct();
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
	std::ofstream	as3File_;
};

#endif