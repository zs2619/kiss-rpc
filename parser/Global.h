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

void yyerror(const char* fmt, ...);
extern int yylex();
extern int   yylineno;
extern char  yytext[];
extern FILE* yyin;
extern void switchBuf(const char* s);
extern std::string curFileName;
#endif