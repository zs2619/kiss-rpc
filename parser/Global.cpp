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
#include "Global.h"
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
std::string  getBaseName(const std::string & includeName) {
	size_t found = includeName.find(".");
	if (found == std::string::npos)
		found = 0;
	std::string baseName = includeName.substr(0, found);
	return baseName;
}