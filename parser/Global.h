//==============================================
/**
	@date:		2012:3:28  
	@file: 		Global.h
	@author		zhangshuai
*/
//==============================================

#ifndef	__GLOBAL_H__
#define	__GLOBAL_H__


void yyerror(const char* fmt, ...);
extern int yylex();
extern int   yylineno;
extern char  yytext[];
extern FILE* yyin;
#endif