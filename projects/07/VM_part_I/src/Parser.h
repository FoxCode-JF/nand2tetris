/*
 * Parser.h
 *
 *  Created on: 15 sie 2020
 *      Author: FoxCode
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>


enum cmdType
{
	NO_COMMAND,
	//PART I
	C_ARITHMETIC,
	C_PUSH,
	C_POP,

	//PART II
	C_LABEL,
	C_GOTO,
	C_IF,
	C_FUNCTION,
	C_RETURN,
	C_CALL
};


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

	static std::unordered_map<std::string, cmdType> command; /*{
		{"add", C_ARITHMETIC}, 	{"sub", C_ARITHMETIC}, 	{"neg", C_ARITHMETIC},
		{"lt", C_ARITHMETIC}, 	{"gt", C_ARITHMETIC}, 	{"eq", C_ARITHMETIC},
		{"and", C_ARITHMETIC}, 	{"or", C_ARITHMETIC}, 	{"not", C_ARITHMETIC},
		{"label", C_LABEL},		{"goto", C_GOTO}, 		{"if-goto", C_IF},
		{"push", C_PUSH}, 		{"pop", C_POP},
		{"call", C_CALL}, 		{"return", C_RETURN}, 	{"function", C_FUNCTION}
	};*/

};

#endif /* PARSER_H_ */
