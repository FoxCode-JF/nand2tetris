/*
 * Parser.h
 *
 *  Created on: 15 sie 2020
 *      Author: FoxCode
 */

#ifndef PARSER_H_
#define PARSER_H_

#include "VMcommands.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

class Parser {
public:
	Parser(std::string fileName);
	virtual ~Parser();

	bool hasMoreCommands();
	cmdType commandType();
	void advance();
	std::string arg1();
	int arg2();

private:
	std::ifstream file;
	std::string currentCommand;
	cmdType currentCmdType=NO_COMMAND;
	int currentCmdLen;
	unsigned int countWordsInString(std::string const& str);

	static std::unordered_map<std::string, cmdType> command;
};

#endif /* PARSER_H_ */
