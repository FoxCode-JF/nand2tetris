//============================================================================
// Name        : hackAssembler.cpp
// Author      : FoxCode
// Version     :
// Copyright   :
// Description : Assembler created for Hack Computer Architecture
//============================================================================

#include <iostream>
#include "Parser.h"

using namespace std;

int main() {
	//std::string str="../max/MaxL.asm";
	//std::ifstream c(str);

	Parser a("../max/Max.asm");
	//cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	int i = 0;
	while(a.hasMoreCommands())
	{
		std::cout << ++i;
		a.advance();
		a.commandType();
		cout << "symbol: " << a.symbol() << endl;
		cout << "dest: " << a.dest() << endl;
		cout << "comp: " << a.comp() << endl;
		cout << "dest: " << a.jump() << endl;

	}
	return 0;
}
