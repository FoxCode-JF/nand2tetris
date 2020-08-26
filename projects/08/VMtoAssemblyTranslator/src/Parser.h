/*
 * Parser.h
 *
 *  Created on: 15 sie 2020
 */

/**
 *  TODO:
 *		Add comments to functions and class
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
	std::string getArg0();
	std::string arg1();
	int arg2();

private:
	std::ifstream file;
	std::string currentCommand;
	std::string arg0;
	cmdType currentCmdType;
	int currentCmdLen;
	unsigned int countWordsInString(std::string const& str);

	static std::unordered_map<std::string, cmdType> s_command;
};

#endif /* PARSER_H_ */
