//==============================================
/**
	@date:		2012:3:27  
	@file: 		Program.h
	@author		zhangshuai
*/
//==============================================
#ifndef		__Program_H__
#define		__Program_H__


#include <vector>
#include <assert.h>
#include <string>
#include <map>
#include <algorithm>    
#include "DefType.h"
#include "Global.h"
  
class Context{
public:
    NameSpaceType     ns_;
};
class Program 
{
public:
	static Program* inst()
	{
		static Program p;
		return &p;
	}
	Program(){}

	bool parseArg(int argc,char** argv);
	bool findDefByName(const std::string& name);
	bool addIncludeFile(const std::string& includeName);
	Context* getFileContext(const std::string& name);

    bool addEnumDefType(EnumDefType* d){
        if (!enums_.addDef(d)){
    		yyerror("enum name repeat: \"%s\"\n", d->name_.c_str());
        }
        return true;
    }

    bool addStructDefType(StructDefType* d){
        if (!structs_.addDef(d)){
    		yyerror("struct name repeat: \"%s\"\n", d->name_.c_str());
        }
        return true;
    }
    bool addServiceDefType(ServiceDefType* d){
        if (!services_.addDef(d)){
    		yyerror("serviece name repeat: \"%s\"\n", d->name_.c_str());
        }
        return true;
    }
	
	std::string& getFileName() { return option_.fileName_;}
	std::string& getBaseName() { return option_.fileName_;}
	std::string& getOutputDir(){ return option_.outputDir_;}
	std::string& getInputDir() { return option_.inputDir_; }

    class Option{
    public:
        Option():json_(false){}
        std::string			fileName_;	
        std::string			baseName_;	
        std::string			inputDir_;	
        std::string			outputDir_;	
        bool				json_;		
    };
    Option  option_;
    

public:
	EnumVector			            enums_;
	StructVector		            structs_;
	ServiceVector		            services_;
	std::vector<std::string>	    include_;
    std::map<std::string ,Context*> contexts_;
};


#endif