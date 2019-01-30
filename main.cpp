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
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <type_traits>


#include "parser/Global.h"
#include "generator/Generator.h"
#include "generator/As3Generator.h"
#include "generator/CppGenerator.h"
#include "generator/CSharpGenerator.h"
#include "generator/GoGenerator.h"
#include "parser/Program.h"

#include "misc/misc.h"


void usage() 
{
	std::cout<< "Usage: rpc [options] file" << std::endl;
	std::cout<< "Options:" << std::endl;
	std::cout<< "  -o   dir    output file directory" << std::endl;
	std::cout<< "  -i   dir    input  file directory" << std::endl;
	std::cout<< "  -gen lang   cpp as3 cs go Generate code" << std::endl;
	std::cout<< "  -json       serialize json." << std::endl;
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
	Program::Option option;

	for (i = 1; i < argc-1; i++) 
	{
      if (strcmp(argv[i], "-o") == 0) 
	  {
		option.outputDir_=argv[++i];
		option.outputDir_+="/";

      } else if (strcmp(argv[i], "-i") == 0)
	  {
		option.inputDir_=argv[++i];
		option.inputDir_+="/";
	  }
	  else if (strcmp(argv[i], "-gen") == 0)
	  {
		  genStr=argv[++i];
	  }
	  else if (strcmp(argv[i], "-json") ==0 )
	  {
		  option.json_=true;
	  }
	}
	if (argv[i] == NULL) {
		std::cerr<<"missing file name"<<std::endl;
		usage();
	}
	if(genStr.empty())
	{
		std::cerr<<"missing Generate code "<<std::endl;
		usage();
	}

	std::vector<std::string> gen;
	split(genStr,";",gen);

	std::string fileName=argv[i];
	option.baseName_=getBaseName(fileName);

	std::string f=option.inputDir_+fileName;
	option.fileName_=f;

	Program::inst()->option_ = option;
	misc::mkdir(Program::inst()->getOutputDir().c_str());

	curFileName=option.baseName_;
	Program::inst()->contexts_[curFileName]=new Context(curFileName);
	yyin = std::fopen(f.c_str() , "r" );
	if(!yyin)
	{
		std::cerr<<"open fail "<< errno<<std::endl;
		return 0;
	}
	try
	{
		if (yyparse() != 0) 
		{
			std::cerr<<"parser error ."<<std::endl;
			return 0;
		}
	} catch (std::string x) {
		std::cerr<<x<<std::endl;
	}

	/**后端生成代码*/
	for (auto& it:gen)
	{
		if (it=="cpp")
		{
			std::unique_ptr<Generator> cpp(new CppGenerator(Program::inst(),"cpp"));
			cpp->generateProgram();
		}
		else if(it=="as3")
		{
			std::unique_ptr<Generator> as3(new As3Generator(Program::inst(),"as3"));
			as3->generateProgram();
		}else if(it=="cs")
		{
			std::unique_ptr<Generator> cs(new CSharpGenerator(Program::inst(),"cs"));
			cs->generateProgram();
		}else if(it=="go")
		{
			std::unique_ptr<Generator> go(new GoGenerator(Program::inst(),"go"));
			go->generateProgram();
		}
	}
	return 0;
}