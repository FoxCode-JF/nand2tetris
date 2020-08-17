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

	Parser p("../StackArithmetic/StackTest/StackTest.vm");
	CodeWriter a("StackTest");


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
