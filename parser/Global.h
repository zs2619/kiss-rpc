//==============================================
/**
	@date:		2012:3:28  
	@file: 		Global.h
	@author		zhangshuai
*/
//==============================================

#ifndef	__GLOBAL_H__
#define	__GLOBAL_H__

#include <string>

extern int yyparse();
extern FILE* yyin;

void yyerror(const char* fmt, ...);
extern int yylex();
extern int   yylineno;
extern char  yytext[];
extern FILE* yyin;
extern void switchBuf(std::string& includeName);
extern std::string curFileName;
extern std::string getBaseName(const std::string& includeName);
#endif