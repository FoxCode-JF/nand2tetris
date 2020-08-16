//============================================================================
// Name        : VM_part_I.cpp
// Author      : FoxCode
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Parser.h"
#include <iostream>
#include <sstream>
#include <string>
#include <iterator>


using namespace std;

int main() {

	Parser p("../MemoryAccess/BasicTest/BasicTest.vm");

	while(p.hasMoreCommands())
	{
		p.advance();
		p.commandType();
		p.arg1();
		p.arg2();
	}
	return 0;
}
