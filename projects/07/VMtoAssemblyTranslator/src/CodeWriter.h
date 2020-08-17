/*
 * CodeWriter.h
 *
 *  Created on: 16 sie 2020
 *      Author: FoxCode
 *
 * Translates VM commands into Hack assembly code.
 */

#ifndef CODEWRITER_H_
#define CODEWRITER_H_

#include "VMcommands.h"
#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>

class CodeWriter {
public:
	CodeWriter(std::string fileName);
	virtual ~CodeWriter();

	void setFileName(std::string fileName);								// Informs the code writer that the translation of a new VM file is started.
	void writeArithmetic(std::string command);							// Writes the assembly code that is the translation of the given arithmetic command.
	void writePushPop(cmdType command, std::string segment, int index); // Writes the assembly code which is translation of a given command (C_PUSH or C_POP)
	void Close();														// Closes the output file

private:
	std::ofstream outFile;
	std::string currentFileName;
	static std::unordered_map<std::string, std::string> s_directMemorySegments;
	static std::unordered_map<cmdType, std::string> s_commandName;
	static unsigned int s_labelIndex;
};

#endif /* CODEWRITER_H_ */
