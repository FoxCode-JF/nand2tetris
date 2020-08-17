//============================================================================
// Name        : VM_part_I.cpp
// Author      : FoxCode
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++
//============================================================================

#include "Parser.h"
#include "CodeWriter.h"
#include <iostream>
#include <sstream>
#include <string>
#include <iterator>


using namespace std;

int main(int argc, char **argv) {

//		../StackArithmetic/StackTest/StackTest.vm
	  if (argc != 2)
	  {
		  std::cerr << "Usage: " << argv[0] << " <file or directory>" << std::endl;
	      return 1;
	  }
	string fileName = argv[1];
	Parser p(fileName);
	cout << "Input file: " << fileName << endl;

	std::size_t found = fileName.find(".vm");
	if (found!=std::string::npos)
	{
		fileName.erase(fileName.begin()+found, fileName.end());
	}

	CodeWriter a(fileName);
	cout << "Output file: " << fileName << endl;

	while(p.hasMoreCommands())
	{
		p.advance();
		if(p.commandType() == cmdType::NO_COMMAND) continue;
		else if(p.commandType() == cmdType::C_ARITHMETIC)
			a.writeArithmetic(p.getArg0());
		else
			a.writePushPop(p.commandType(), p.arg1(), p.arg2());
	}
	return 0;
}
