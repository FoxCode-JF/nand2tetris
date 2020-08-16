/*
 * Parser.cpp
 *
 *  Created on: 15 sie 2020
 *      Author: FoxCode
 */

 /**
  * TODO:
  * 	Close file in destructor
  * 	check if file is open if not cerr
  * 	check if there are redundant includes
  */

#include "Parser.h"
#include <algorithm>
#include <string>
#include <iterator>

Parser::Parser(std::string fileName)
{
	this->file.open(fileName);
	this->currentCmdType=NO_COMMAND;
	this->currentCmdLen = 0;
}
Parser::~Parser() {};

bool Parser::hasMoreCommands()
{
	if(!this->file.eof())
		return true;
	else
		return false;
}

std::unordered_map<std::string, cmdType> Parser::command {
		{"add", C_ARITHMETIC}, 	{"sub", C_ARITHMETIC}, 	{"neg", C_ARITHMETIC},
		{"lt", C_ARITHMETIC}, 	{"gt", C_ARITHMETIC}, 	{"eq", C_ARITHMETIC},
		{"and", C_ARITHMETIC}, 	{"or", C_ARITHMETIC}, 	{"not", C_ARITHMETIC},
		{"label", C_LABEL},		{"goto", C_GOTO}, 		{"if-goto", C_IF},
		{"push", C_PUSH}, 		{"pop", C_POP},
		{"call", C_CALL}, 		{"return", C_RETURN}, 	{"function", C_FUNCTION}
	};

/**
 *	Get next command and discard comments
 */
void Parser::advance()
{
	std::getline(this->file, this->currentCommand);
	this->currentCmdLen=this->currentCommand.length();
	for (int i = 0; i < this->currentCmdLen - 2; i++)
	{
		if(!this->currentCommand.empty() && this->currentCommand[i] == '/' && this->currentCommand[i+1]=='/')
		{
			this->currentCommand.erase(i, this->currentCmdLen);
			break;
		}
	}
	std::cout << this->currentCommand <<  " :: ";
}

/**
 *  In command formatted as:
 *  	"cmd"			or
 *  	"cmd arg1"		or
 *  	"cmd arg1 arg2"
 *  get cmd.
*/
cmdType Parser::commandType()
{
	std::istringstream ss(this->currentCommand);
	std::string cmd;

	ss >> cmd;
	std::cout << cmd;
	return command[cmd];
}

std::string Parser::arg1()
{
	std::istringstream ss(this->currentCommand);
	std::string arg1;

	if(this->countWordsInString(this->currentCommand) == 2)
	{
		for (int i = 0; i < 2; ++i)
		{
			ss >> arg1;
		}
		std::cout << " :: " << arg1;
		return arg1;
	}else
		return "";
}

int Parser::arg2()
{
	std::istringstream ss(this->currentCommand);
	std::string arg2str;

	if(this->countWordsInString(this->currentCommand) == 3)
	{
		for (int i = 0; i < 3; ++i)
		{
			ss >> arg2str;
		}
		std::cout << " :: " << std::stoi(arg2str) << std::endl;
		return std::stoi(arg2str);
	} else
		return 0;
}

unsigned int Parser::countWordsInString(std::string const& str)
{
    std::stringstream stream(str);
    return std::distance(std::istream_iterator<std::string>(stream), std::istream_iterator<std::string>());
}
