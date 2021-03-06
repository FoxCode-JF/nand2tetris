/*
 * Parser.h
 *
 *  Created on: 10 sie 2020
 *      Author: FoxCode
 */

#ifndef PARSER_H_
#define PARSER_H_
#include <string>
#include <iostream>
#include <fstream>

enum cmdType
{
	NO_COMMAND,
	A_COMMAND,
	C_COMMAND,
	L_COMMAND
};

class Parser
{
public:
	Parser(std::string asmFileName);
	virtual ~Parser();

	bool hasMoreCommands();
	void advance();
	cmdType commandType();
	std::string symbol();
	std::string dest();
	std::string comp();
	std::string jump();
	void rewindFile();

private:
	std::ifstream asmFile;
	std::string currentCommand;
	cmdType currentCmdType;
	int currentCmdLen;

};

#endif /* PARSER_H_ */
