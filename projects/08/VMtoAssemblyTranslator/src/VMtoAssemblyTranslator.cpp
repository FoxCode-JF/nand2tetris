//============================================================================
// Name        : VM_part_I.cpp
// Author      : FoxCode
// Version     :
// Copyright   : Your copyright notice
// Description : VM to assembly translator
//============================================================================




#include "Parser.h"
#include "CodeWriter.h"
#include <dirent.h>		// used instead of <filesystem> because course grader is not compatible with c++17
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iterator>
//#include <filesystem>

//namespace fs = std::filesystem;

using namespace std;

std::string makeOutFileName(std::string name);
void translator(Parser& myParser, CodeWriter& myWriter);

int main(int argc, char **argv) {

	if (argc != 2)
	{
		 std::cerr << "Usage: " << argv[0] << " <file or directory>" << std::endl;
		 return 1;
	}

	string path(argv[1]);
	string outFileName;

	DIR *dir;
	struct dirent *ent;

	Parser *myParser;
	CodeWriter *myWriter;

	size_t foundExtension = path.find(".vm");
	if(foundExtension!=std::string::npos)
	{
		myParser = new Parser(path);
		outFileName = makeOutFileName(path);
		myWriter = new CodeWriter(outFileName);
		translator(*myParser, *myWriter);
	}
	else
	{
//		add "/" at the end of directory if it is not there
		if(path[path.length()-1] != '/')
		{
			path.push_back('/');
		}
		outFileName = makeOutFileName(path);
		myWriter = new CodeWriter(outFileName);
		myWriter->writeInit();

		if ((dir = opendir (path.c_str())) != NULL)
		{
			/* print all the files and directories within directory */
			while ((ent = readdir(dir)) != NULL)
			{
				string inFileName = ent->d_name;
				size_t found = inFileName.find(".vm");

				if(found!=std::string::npos)
				{
					//cout << inFileName << endl;
					myWriter->setFileName(inFileName);
					cout << path + inFileName << endl;
					myParser = new Parser(path + inFileName);
					translator(*myParser, *myWriter);
				}
			}
			closedir(dir);
		} else
		{
			/* could not open directory */
			perror ("");
			return EXIT_FAILURE;
		}
	}

/**
 * Using c++17 <filesystem>
 */
/*	fs::path path(argv[1]);

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
		myWriter->writeInit();
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
		myWriter = new CodeWriter(outFileName);
		myWriter->writeInit();
		for(const auto& it : fs::directory_iterator(path))
		{
			std::string s(fs::absolute( it.path()).string());
			cout << it << endl;
			size_t found = s.find(".vm");
			if( it.is_regular_file() && found!=std::string::npos)
			{
				cout << s << endl;
				myWriter->setFileName(s);
				myParser = new Parser(s);
				translator(*myParser, *myWriter);
			}
//			if(myParser != nullptr) delete myParser;
		}
	}
***********************************************************/

	if(myParser != nullptr) delete myParser;
	if(myWriter != nullptr) delete myWriter;

	return 0;
}


std::string  makeOutFileName(std::string name)
{
	size_t found = name.find(".vm");
	if (found!=std::string::npos)
	{
		return name.substr(0, found);
	}
	else
	{
		std::string s = name.substr(0, name.length()-1);
		found = s.find_last_of("/\\");
		if(found!=std::string::npos)
		{
			return name + s.substr(found + 1, s.length()-1);
		}
	}
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
