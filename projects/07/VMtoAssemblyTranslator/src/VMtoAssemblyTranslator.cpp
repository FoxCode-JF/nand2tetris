//============================================================================
// Name        : VM_part_I.cpp
// Author      : FoxCode
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++
//============================================================================


/**
 * TODO:
 * 		Allow translation of all files in a directory
 */

#include "Parser.h"
#include "CodeWriter.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iterator>
#include <filesystem>


using namespace std;

int main(int argc, char **argv) {

//		../StackArithmetic/StackTest/StackTest.vm
	if (argc != 2)
	{
		 std::cerr << "Usage: " << argv[0] << " <file or directory>" << std::endl;
		 return 1;
	}

	string fullPath = argv[1];

	Parser p(fullPath);
	cout << "Input file: " << fullPath << endl;


	size_t found = fullPath.find(".vm");
	if (found!=std::string::npos)
	{
		fullPath.erase(fullPath.begin()+found, fullPath.end());
	}
/*	else //fullPath contains directory
	{
		for(const auto &entry : std::filesystem::directory_iterator(fullPath))
		{

		}
	}*/

	CodeWriter a(fullPath);
	cout << "Output file: " << fullPath << ".asm"<< endl;

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
