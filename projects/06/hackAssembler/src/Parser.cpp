/*
 * Parser.cpp
 *
 *  Created on: 10 sie 2020
 *      Author: FoxCode
 */

#include "Parser.h"
#include <algorithm>
#include <cstdio>

Parser::Parser(std::string asmFileName)
{
	this->asmFile.open(asmFileName);
	this->currentCmdType=NO_COMMAND;
	this->currentCmdLen=0;
}
Parser::~Parser() {
	asmFile.close();
};

bool Parser::hasMoreCommands()
{
	if(!this->asmFile.eof())
		return true;
	else
		return false;
}
/**
 *	get next command and discard comments
 */
void Parser::advance()
{
	std::getline(this->asmFile, this->currentCommand);
	this->currentCmdLen=this->currentCommand.length();

	for (int i = 0; i < this->currentCmdLen - 2; i++)
	{
		if(!this->currentCommand.empty() && this->currentCommand[i] == '/' && this->currentCommand[i+1]=='/')
		{
			this->currentCommand.erase(i, this->currentCmdLen);
			break;
		}
	}
	this->currentCommand.erase(std::remove_if(	this->currentCommand.begin(),
												this->currentCommand.end(),
												::isspace),
												this->currentCommand.end());

//	Determine current command type
	if(this->currentCommand.empty())
		{
			this->currentCmdType=NO_COMMAND;
		}
		else if(this->currentCommand.find("@") == 0)
		{
			this->currentCmdType=A_COMMAND;
		}
		else if(this->currentCommand.find("(") == 0)
		{
			this->currentCmdType=L_COMMAND;
		}
		else
		{
			this->currentCmdType=C_COMMAND;
		}
}

cmdType Parser::commandType()
{
	return this->currentCmdType;
}

std::string Parser::symbol()
{
	if(this->currentCmdType == A_COMMAND)
	{
		return this->currentCommand.substr(1);
	}
	else if(this->currentCmdType == L_COMMAND)
	{
		return this->currentCommand.substr(1, this->currentCmdLen - 2);
	}
	else return "";
}
std::string Parser::dest()
{
	if(this->currentCmdType == C_COMMAND)
	{
		size_t eqInd = this->currentCommand.find("=");
		if(eqInd != std::string::npos)
		{
			return this->currentCommand.substr(0, eqInd);
		}
		else return "";
	}
	else return "";
}
std::string Parser::comp()
{
	if(this->currentCmdType == C_COMMAND)
		{
			size_t eqInd = this->currentCommand.find("=");
			size_t semiColInd = this->currentCommand.find(";");
			if(eqInd != std::string::npos && semiColInd != std::string::npos)
			{
				return this->currentCommand.substr(semiColInd, eqInd);
			}
			else if(eqInd != std::string::npos)
			{
				return this->currentCommand.substr(eqInd + 1, this->currentCommand.length() - 1);
			}
			else
				return this->currentCommand.substr(0, semiColInd);
		}
		return "";
}
std::string Parser::jump()
{
	if(this->currentCmdType == C_COMMAND)
	{
			size_t semiColInd = this->currentCommand.find(";");
		if(semiColInd != std::string::npos)
		{
			return this->currentCommand.substr(semiColInd + 1);
		}
			else return "";
	}
		else return "";
}

void Parser::rewindFile()
{
	this->asmFile.clear();
	this->asmFile.seekg(0);
}
