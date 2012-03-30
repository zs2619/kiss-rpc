//==============================================
/**
	@date:		2012:3:29  
	@file: 		Global.cpp
	@author: 	zhangshuai
*/
//==============================================
#include <stdio.h>
#include <stdarg.h>
#include "global.h"
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
}
