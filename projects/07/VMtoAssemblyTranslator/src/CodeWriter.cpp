/*
 * CodeWriter.cpp
 *
 *  Created on: 16 sie 2020
 *      Author: FoxCode
 */

/**
 * 	TODO:
 * 		Add exception handling
 */
#include "CodeWriter.h"


CodeWriter::CodeWriter(std::string fileName)
{
	currentFileName = fileName + ".asm";
	this->outFile.open(currentFileName);

	if(!outFile.is_open())
	{
		std::cerr << "File didn't open correctly";
	}
}

CodeWriter::~CodeWriter() {
	outFile.close();
}

std::unordered_map<cmdType, std::string> CodeWriter::s_commandName {
		{cmdType::C_LABEL, "label"},		{cmdType::C_GOTO, "goto"}, 		{cmdType::C_IF, "if-goto"},
		{cmdType::C_PUSH, "push"}, 			{cmdType::C_POP, "pop"},
		{cmdType::C_CALL, "call"}, 			{cmdType::C_RETURN, "return"}, 	{cmdType::C_FUNCTION, "function"}
};

std::unordered_map<std::string, std::string> CodeWriter::s_directMemorySegments {
		{"local", "LCL"}, {"argument", "ARG"}, {"this", "THIS"}, {"that", "THAT"}};

void CodeWriter::setFileName(std::string fileName)
{
	if(outFile.is_open())	outFile.close();

	this->currentFileName = fileName + ".asm";
	this->outFile.open(this->currentFileName);

	if(!outFile.is_open())
	{
		std::cerr << "File didn't open correctly";
	}
}
void CodeWriter::writeArithmetic(std::string command)
{
//	Write comment with command to file
	this->outFile << "//\t"<< command << std::endl;
//	Point at y value on the stack (decrement SP)
	this->outFile << "@SP" 	<< std::endl;
	this->outFile << "A=M-1"<< std::endl;

//  Perform operations needing only y value
	if(command == "neg")
	{
		this->outFile << "M=-M" << std::endl;
		return;
	} else if(command == "not")
	{
		this->outFile << "M=!M" << std::endl;
		return;
	}

	this->outFile << "D=M" 	<< std::endl;	//	Save y value in D register
	this->outFile << "A=A-1"<< std::endl;	//	Point at x value on the stack

//	Perform operations on both values x and y
	if(command == "add")
	{
		this->outFile 	<< "M=D+M" << std::endl;
	} else if(command == "sub")
	{
		this->outFile 	<< "M=M-D" << std::endl;
	} else if(command == "eq")
	{
		this->outFile 	<< "D=D-M" 	<< std::endl
						<< "@IS_EQ" << std::endl
						<< "D;JEQ" 	<< std::endl
						<< "@SP" 	<< std::endl
						<< "A=M-1"	<< std::endl
						<< "A=A-1"	<< std::endl
						<< "M=0"	<< std::endl	// D = false
						<< "(IS_EQ)"<< std::endl
						<< "@SP" 	<< std::endl
						<< "A=M-1"	<< std::endl
						<< "A=A-1"	<< std::endl
						<< "M=-1"	<< std::endl;	// D = true

	} else if(command == "gt")
	{
		this->outFile 	<< "D=D-M" 	<< std::endl
						<< "@IS_GREATER" << std::endl
						<< "D;JGT" 	<< std::endl
						<< "@SP" 	<< std::endl
						<< "A=M-1"	<< std::endl
						<< "A=A-1"	<< std::endl
						<< "M=0"	<< std::endl	// D = false
						<< "(IS_GREATER)"<< std::endl
						<< "@SP" 	<< std::endl
						<< "A=M-1"	<< std::endl
						<< "A=A-1"	<< std::endl
						<< "M=-1"	<< std::endl;	// D = true
	} else if(command == "lt")
	{
		this->outFile 	<< "D=D-M" 	<< std::endl
						<< "@IS_LOWER" << std::endl
						<< "D;JLT" 	<< std::endl
						<< "@SP" 	<< std::endl
						<< "A=M-1"	<< std::endl
						<< "A=A-1"	<< std::endl
						<< "M=0"	<< std::endl	// D = false
						<< "(IS_LOWER)"<< std::endl
						<< "@SP" 	<< std::endl
						<< "A=M-1"	<< std::endl
						<< "A=A-1"	<< std::endl
						<< "M=-1"	<< std::endl;	// D = true
	} else if(command == "and")
	{
		this->outFile << "M=M&D" << std::endl;
	} else if(command == "or")
	{
		this->outFile << "M=M|D" << std::endl;
	}

//	Decrement Stack Pointer
	this->outFile << "@SP" 		<< std::endl;
	this->outFile << "M=M-1" 	<< std::endl;
}
void CodeWriter::writePushPop(cmdType command, std::string segment, int index)
{
	//	Write comment with command to file
	this->outFile << "//\t"<< s_commandName[command] << " " << segment << " " << index << std::endl;
	if(command == cmdType::C_PUSH)
	{
		if((s_directMemorySegments.contains(segment)))
		{
			this->outFile 	<< "@" << index					 			<< std::endl
							<< "D=A" 									<< std::endl
							<< "@" << s_directMemorySegments[segment] 	<< std::endl
							<< "A=A+D"									<< std::endl
							<< "D=M"						 			<< std::endl;
		}
		else if(segment == "pointer")
		{
			if (index == 0)
				this->outFile << "@THIS" << std::endl;
			else if(index == 1)
				this->outFile << "@THAT" << std::endl;

			this->outFile << "D=M" << std::endl;
		}
		else if(segment == "temp")
		{
			this->outFile 	<< "@R"	<< index+5 << std::endl
							<< "D=M"<< std::endl;
		}
		else if(segment == "constant")
		{
			this->outFile 	<< "@" 	<< index << std::endl
							<< "D=A"<< std::endl;
		}
		else if (segment == "static")
		{
			this->outFile << "@" << this->currentFileName << "." << index << std::endl
						  << "D=M" << std::endl;

		}


		this->outFile 	<< "@SP"	<< std::endl
						<< "A=M"	<< std::endl
						<< "M=D"	<< std::endl
						<< "@SP"	<< std::endl
						<< "M=M+1"	<< std::endl;
	}

	if(command == cmdType::C_POP)
	{
		if((s_directMemorySegments.contains(segment)))
		{
			this->outFile 	<< "@" << index					 			<< std::endl
							<< "D=A" 									<< std::endl
							<< "@" << s_directMemorySegments[segment] 	<< std::endl
							<< "D=A+D"									<< std::endl;
		}
		else if(segment == "constant")
		{
			this->outFile 	<< "@" << index	<< std::endl
							<< "D=A"		<< std::endl;
		}
		else if (segment == "static")
		{
			this->outFile 	<< "@" << this->currentFileName << "." << index << std::endl
							<< "D=A" 	<< std::endl;

		}
		else if(segment == "temp")
		{
			this->outFile 	<< "@R"	<< index+5 << std::endl
							<< "D=A"<< std::endl;
		}
		else if(segment == "pointer")
			{
				if (index == 0)
					this->outFile << "@THIS" << std::endl;
				else if(index == 1)
					this->outFile << "@THAT" << std::endl;

				this->outFile << "D=A" << std::endl;
			}
	}
	this->outFile 	<< "@R14"	<< std::endl
					<< "M=D"	<< std::endl
					<< "@SP"	<< std::endl
					<< "AM=M-1"	<< std::endl
					<< "D=M"	<< std::endl
					<< "@R14"	<< std::endl
					<< "A=M"	<< std::endl
					<< "M=D"	<< std::endl;

}
void CodeWriter::Close()
{

}
