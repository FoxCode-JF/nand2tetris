//============================================================================
// Name        : hackAssembler.cpp
// Author      : FoxCode
// Version     :
// Copyright   :
// Description : Assembler created for Hack Computer Architecture
//============================================================================

#include "Parser.h"
#include "Code.h"
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char **argv) {

	uint16_t StackPointer = 16;
	uint16_t ROMaddr = 0;

	if (argc > 2)
	{
		 std::cerr << "Usage: " << argv[0] << " " << argv[1] << ", <file or directory>" << std::endl;
		 return 1;
	}

	string 	inputPath = argv[1];
	string 	outPath = "";

	cout << "Input file: " << inputPath << endl;


	size_t found = inputPath.find(".asm");
	if (found!=std::string::npos)
	{
		outPath = inputPath.substr(0, found) + ".hack";
	}

	cout << "Output file: " << outPath << ".hack"<< endl;

	Parser 	a(inputPath);
	Code 	b;

	ofstream outFile(outPath);

/**		FIRST PASS
* 			Fill Symbol table
*/
	std::string mySymbol = "";
	cmdType myCmdType;

	while(a.hasMoreCommands())
	{
		a.advance();
		a.commandType();
		myCmdType = a.commandType();
		mySymbol = a.symbol();

		if(myCmdType == L_COMMAND)	b.hackTab.addEntry(mySymbol, ROMaddr);
		else if(myCmdType != NO_COMMAND) ROMaddr++;
	}

/**		SECOND PASS
 * 			Translation
 */
	a.rewindFile();

	while(a.hasMoreCommands())
	{
		a.advance();
		myCmdType = a.commandType();


		if(myCmdType == NO_COMMAND) continue;
		else if(myCmdType == A_COMMAND)
		{
			mySymbol = a.symbol();
			if(!isdigit(mySymbol[0]) && !b.hackTab.Contains(mySymbol))
			{
				b.hackTab.addEntry(mySymbol, StackPointer);
				StackPointer++;
			}
			outFile << b.symbolBits(mySymbol) << endl;
		}
		else if(a.commandType() == C_COMMAND)
		{
			string c=a.comp();
			string d=a.dest();
			string j=a.jump();

			outFile << "111" << b.comp(c) << b.dest(d) << b.jump(j) << endl;
		}
	}
	return 0;
}
