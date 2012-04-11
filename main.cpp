//==============================================
/**
	@date:		2012:3:27  
	@file: 		main.cpp
	@author		zhangshuai
*/
//==============================================

#include <stdio.h>
#include <string>
#include <assert.h>
#include <stdarg.h>
#include <direct.h>

#include "parser/Global.h"
extern int yyparse();
extern FILE* yyin;
#include "generator/Generator.h"
#include "generator/As3Generator.h"
#include "generator/CppGenerator.h"
#include "parser/Program.h"

/**		照抄thrift 
		前端和后端有引用 以后需要重构!!!!
*/
void usage() 
{
  fprintf(stderr, "Usage: rpc [options] file\n");
  fprintf(stderr, "Options:\n");
  fprintf(stderr, "  -o dir     output file directory\n");
  fprintf(stderr, "  -i dir     input  file directory\n");
  fprintf(stderr, "  -gen STR   cpp;as3 Generate code .\n");
  exit(0);
}
void split(std::string& s, const std::string& delim,std::vector< std::string >& ret)
{
 size_t last = 0;
 size_t index=s.find_first_of(delim,last);
 while (index!=std::string::npos)
 {
  ret.push_back(s.substr(last,index-last));
  last=index+1;
  index=s.find_first_of(delim,last);
 }
 if (index-last>0)
 {
  ret.push_back(s.substr(last,index-last));
 }
}
int main(int argc,char** argv)
{
	if (argc < 2)
		usage();
	std::string genStr;
	int i;
	for (i = 1; i < argc-1; i++) 
	{
      if (strcmp(argv[i], "-o") == 0) 
	  {
		Program::inst()->outputDir_=argv[++i];
		Program::inst()->outputDir_+="/";
		mkdir(Program::inst()->outputDir_.c_str());
      } else if (strcmp(argv[i], "-i") == 0)
	  {
		Program::inst()->inputDir_=argv[++i];
		Program::inst()->inputDir_+="/";
	  }
	  else if (strcmp(argv[i], "-gen") == 0)
	  {
		  genStr=argv[++i];
	  }
	}
	if (argv[i] == NULL) {
		fprintf(stderr, "Missing file name\n");
		usage();
	}
	if(genStr.empty())
	{
		fprintf(stderr, "Missing Generate code\n");
		usage();
	}
	std::vector<std::string> gen;
	split(genStr,";",gen);

	std::string fileName=argv[i];
	size_t found=fileName.find(".");
	if (found==std::string::npos)
		found=0;
	std::string baseName=fileName.substr(0,found);
	Program::inst()->baseName_=baseName;
	Program::inst()->fileName_=fileName;
	std::string f=Program::inst()->inputDir_+Program::inst()->fileName_;
	yyin = fopen(f.c_str() , "r" );
	if(!yyin)
	{
		printf("open fail %d.\n",errno);
		return 0;
	}
	try
	{
		if (yyparse() != 0) 
		{
			printf("Parser error .");
			return 0;
		}
	} catch (std::string x) {
		printf(x.c_str());
	}
	/**后端生成代码*/

	std::vector<std::string>::iterator it= gen.begin();
	while(it!=gen.end())
	{
		if (*it=="cpp")
		{
			Generator* cpp=new CppGenerator(Program::inst(),"cpp");
			cpp->generateProgram();
		}
		else if(*it=="as3")
		{
			Generator* as3=new As3Generator(Program::inst(),"as3");
			as3->generateProgram();
		}
		++it;
	}
	return 0;
}