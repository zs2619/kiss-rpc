//==============================================
/**
	@date:		2012:3:27  
	@file: 		Generator.h
	@author		zhangshuai
*/
//==============================================
#ifndef		__GENERATOR_H__
#define		__GENERATOR_H__

class Program;
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "../parser/Program.h"
#include "../parser/DefType.h"

class Generator
{
public:
	Generator(Program* pro,const std::string& name):program_(pro),name_(name),indent_(0){}
	const Program* getProgram() const { return program_; }

	virtual void generateProgram()=0;
	void indent_up(){
		++indent_;
	}

	void indent_down() {
		--indent_;
	}

	/**
	* Indentation print function
	*/
	std::string indent() {
		std::string ind = "";
		int i;
		for (i = 0; i < indent_; ++i) {
			ind += "\t";
		}
		return ind;
	}

	std::ostream& indent(std::ostream &os) {
		return os << indent();
	}

	void scope_up(std::ostream& out) {
		indent(out) << "{" << std::endl;
		indent_up();
	}

	void scope_down(std::ostream& out) {
		indent_down();
		indent(out) << "}" << std::endl;
	}


protected:
	int			indent_;		//<缩进
	Program*	program_;
	std::string name_;			//<生成后端的名字
};

#endif