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



namespace fs = std::filesystem;

using namespace std;

std::string makeOutFileName(std::string name);
void translator(Parser& myParser, CodeWriter& myWriter);

int main(int argc, char **argv) {

	if (argc != 2)
	{
		 std::cerr << "Usage: " << argv[0] << " <file or directory>" << std::endl;
		 return 1;
	}

	fs::path path(argv[1]);

	Parser *myParser;
	CodeWriter *myWriter;
	std::string strPath = path.string();
	std::string outFileName;
	cout << "Input: " << strPath << endl;

	if(fs::is_regular_file(path))
	{
		myParser = new Parser(strPath);
		outFileName = makeOutFileName(strPath);
		myWriter = new CodeWriter(outFileName);

		translator(*myParser, *myWriter);
	}
	else if(fs::is_directory(path))
	{
//		add "/" at the end of directory if it is not there
		if(strPath[strPath.length()-1] != '/')
		{
			strPath.push_back('/');
		}
		outFileName = makeOutFileName((strPath));
		myWriter = new CodeWriter();
		for(const auto& it : fs::directory_iterator(path))
		{
			std::string s(fs::absolute( it.path()).string());
			cout << it << endl;
			size_t found = s.find(".vm");
			if( it.is_regular_file() && found!=std::string::npos)
			{
				myParser = new Parser(s);
				myWriter->setFileName(outFileName);
				translator(*myParser, *myWriter);
			}
			//if(myParser != nullptr) delete myParser;
		}
	}
	if(myParser != nullptr) delete myParser;
	delete myWriter;

	return 0;
}


std::string  makeOutFileName(std::string name)
{
	size_t found = name.find(".vm");
	if (found!=std::string::npos)
	{
		return name.substr(found, name.length()-1);
		cout << "Out Name: " << name << endl;
	}
	else
	{
		std::string s = name.substr(0, name.length()-1);
		found = s.find_last_of("/");
		if(found!=std::string::npos)
		{
			return name + s.substr(found, s.length()-1);
		}
	}
//	TODO Add exception
	return "";
}

void translator(Parser& myParser, CodeWriter& myWriter)
{
	while(myParser.hasMoreCommands())
		{
			myParser.advance();
			if(myParser.commandType() == cmdType::NO_COMMAND) continue;
			else if(myParser.commandType() == cmdType::C_ARITHMETIC)
				myWriter.writeArithmetic(myParser.getArg0());
			else if(myParser.commandType() == cmdType::C_LABEL)
				myWriter.writeLabel(myParser.arg1());
			else if(myParser.commandType() == cmdType::C_GOTO)
			{
				myWriter.writeGoto(myParser.arg1());
			}
			else if(myParser.commandType() == cmdType::C_IF)
			{
				myWriter.writeIfGoto(myParser.arg1());
			}
			else if(myParser.commandType() == cmdType::C_FUNCTION)
			{
				myWriter.writeFunction(myParser.arg1(), myParser.arg2());
			}
			else if(myParser.commandType() == cmdType::C_RETURN)
			{
				myWriter.writeReturn();
			}
			else if(myParser.commandType() == cmdType::C_CALL)
			{
				myWriter.writeCall(myParser.arg1(), myParser.arg2());
			}
			else
				myWriter.writePushPop(myParser.commandType(), myParser.arg1(), myParser.arg2());
		}
}
