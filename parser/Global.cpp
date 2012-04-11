//==============================================
/**
	@date:		2012:3:29  
	@file: 		Global.cpp
	@author: 	zhangshuai
*/
//==============================================
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "global.h"
std::string curFileName;
void yyerror(const char* fmt, ...) {
	va_list args;
	fprintf(stderr,
		"[ERROR:%s:%d] (last token was '%s')\n",
		"shuai",
		yylineno,
		yytext);

	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);

	fprintf(stderr, "\n");
	exit(1);
}
