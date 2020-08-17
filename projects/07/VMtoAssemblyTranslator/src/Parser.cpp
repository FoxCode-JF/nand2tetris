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
  * 	Add exception handling
  */

#include "Parser.h"
#include <algorithm>
#include <string>
#include <iterator>

Parser::Parser(std::string fileName)
{
	this->file.open(fileName);
	this->currentCmdType=cmdType::NO_COMMAND;
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

std::unordered_map<std::string, cmdType> Parser::s_command {
		{"add", cmdType::C_ARITHMETIC}, 	{"sub", cmdType::C_ARITHMETIC},	{"neg", cmdType::C_ARITHMETIC},
		{"lt", cmdType::C_ARITHMETIC}, 		{"gt", cmdType::C_ARITHMETIC}, 	{"eq", cmdType::C_ARITHMETIC},
		{"and", cmdType::C_ARITHMETIC}, 	{"or", cmdType::C_ARITHMETIC}, 	{"not", cmdType::C_ARITHMETIC},
		{"label", cmdType::C_LABEL},		{"goto", cmdType::C_GOTO}, 		{"if-goto", cmdType::C_IF},
		{"push", cmdType::C_PUSH}, 			{"pop", cmdType::C_POP},
		{"call", cmdType::C_CALL}, 			{"return", cmdType::C_RETURN}, 	{"function", cmdType::C_FUNCTION}
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
	ss >> this->arg0;

	return s_command[this->arg0];
}

std::string Parser::arg1()
{
	std::istringstream ss(this->currentCommand);
	std::string arg1;

	if(this->countWordsInString(this->currentCommand) >= 2)
	{
		for (int i = 0; i < 2; ++i)
		{
			ss >> arg1;
		}
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
		return std::stoi(arg2str);
	} else
		return 0;
}

std::string Parser::getArg0()
{
	std::string tmp = this->arg0;
	this->arg0.erase();
	return tmp;
}
unsigned int Parser::countWordsInString(std::string const& str)
{
    std::stringstream stream(str);
    return std::distance(std::istream_iterator<std::string>(stream), std::istream_iterator<std::string>());
}
