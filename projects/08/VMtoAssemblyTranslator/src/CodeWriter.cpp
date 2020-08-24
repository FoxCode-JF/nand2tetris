/*
 * CodeWriter.cpp
 *
 *  Created on: 16 sie 2020
 *      Author: FoxCode
 */

/**
 * 	TODO:
 * 		Add exception handling
 *
 *
 */
#include "CodeWriter.h"

CodeWriter::CodeWriter()
{
	this->labelIndex = 0;
}
CodeWriter::CodeWriter(std::string filePath)
{
	this->labelIndex = 0;

	this->outFile.open(filePath + ".asm");

	if(!outFile.is_open())
	{
		std::cerr << "File didn't open correctly";
	}

	size_t found = filePath.rfind("/");
	if (found!=std::string::npos)
	{
		this->currentFileName = filePath.substr(found + 1, filePath.size() - found);
		std::cout << this->currentFileName << std::endl;
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
		return;
	}
}
void CodeWriter::writeArithmetic(std::string command)
{
//	Write comment with command to file
	this->outFile << "//\t"<< command << std::endl;

//	Point at y value on the stack and decrement SP
	this->outFile << "@SP" 	 << std::endl;
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
		this->outFile 	<< "D=D-M" 	<< std::endl					// y-x
						<< "@IS_EQ" << labelIndex << std::endl
						<< "D;JEQ" 	<< std::endl
						<< "@SP" 	<< std::endl
						<< "A=M-1"	<< std::endl
						<< "A=A-1"	<< std::endl
						<< "M=0"	<< std::endl	// D = false
						<< "@END"	<< labelIndex << std::endl
						<< "0;JMP"	<< std::endl
						<< "(IS_EQ"	<< labelIndex << ")"<< std::endl
						<< "@SP" 	<< std::endl
						<< "A=M-1"	<< std::endl
						<< "A=A-1"	<< std::endl
						<< "M=-1"	<< std::endl	// D = true
						<< "(END"	<< labelIndex << ")"<< std::endl;

	} else if(command == "gt")
	{
		this->outFile 	<< "D=M-D" 	<< std::endl
						<< "@IS_GREATER" << labelIndex << std::endl
						<< "D;JGT" 	<< std::endl
						<< "@SP" 	<< std::endl
						<< "A=M-1"	<< std::endl
						<< "A=A-1"	<< std::endl
						<< "M=0"	<< std::endl	// D = false
						<< "@END"	<< labelIndex << std::endl
						<< "0;JMP"	<< std::endl
						<< "(IS_GREATER" << labelIndex << ")" << std::endl
						<< "@SP" 	<< std::endl
						<< "A=M-1"	<< std::endl
						<< "A=A-1"	<< std::endl
						<< "M=-1"	<< std::endl	// D = true
		<< "(END"	<< labelIndex << ")"<< std::endl;
	} else if(command == "lt")
	{
		this->outFile 	<< "D=M-D" 	<< std::endl
						<< "@IS_LOWER" << labelIndex << std::endl
						<< "D;JLT" 	<< std::endl
						<< "@SP" 	<< std::endl
						<< "A=M-1"	<< std::endl
						<< "A=A-1"	<< std::endl
						<< "M=0"	<< std::endl	// D = false
						<< "@END"	<< labelIndex << std::endl
						<< "0;JMP"	<< std::endl
						<< "(IS_LOWER" << labelIndex << ")" << std::endl
						<< "@SP" 	<< std::endl
						<< "A=M-1"	<< std::endl
						<< "A=A-1"	<< std::endl
						<< "M=-1"	<< std::endl	// D = true
						<< "(END"	<< labelIndex << ")"<< std::endl;
	} else if(command == "and")
	{
		this->outFile << "M=M&D" << std::endl;
	} else if(command == "or")
	{
		this->outFile << "M=M|D" << std::endl;
	}

	this->outFile << "@SP" 	 << std::endl;
	this->outFile << "M=M-1"<< std::endl;

//	Increment label index
	labelIndex++;
}

void CodeWriter::writePushPop(cmdType command, std::string segment, int index)
{
	//	Write comment with command to file
	writeLine("//\t" + s_commandName[command] + " " + segment + " " + std::to_string(index));

	if(command == cmdType::C_PUSH)
	{
		writePush(segment, index);
	}
	else if(command == cmdType::C_POP)
	{
		this->writePop(segment, index);
	}
}

void CodeWriter::writePop(std::string segment, int index)
{
	std::string asmSegment = segmentToAsm(segment, index);
	auto search = s_directMemorySegments.find(segment);
	if(search != s_directMemorySegments.end()) 	// if segment == "local" or "argument" or "this" or "that"
	{
		writeLine("@" + asmSegment);
		writeLine("D=M");
		writeLine("@" + std::to_string(index));
		writeLine("D=A+D");
	}
	else if(segment == "static")
	{
		writeLine("@" + asmSegment);
		writeLine("D=A");
		writeLine("@" + std::to_string(index));
		writeLine("D=D+A");
	}else// if segment == "pointer" or "temp"
	{
		writeLine("@" + asmSegment);
		writeLine("D=A");
	}
	writeLine("@R14");
	writeLine("M=D");
	writeLine("@SP");
	writeLine("AM=M-1");
	writeLine("D=M");
	writeLine("@R14");
	writeLine("A=M");
	writeLine("M=D");
}

void CodeWriter::writePush(std::string segment, int index)
{
	std::string asmSegment = segmentToAsm(segment, index);
	auto search = s_directMemorySegments.find(segment);

	if(search != s_directMemorySegments.end()) 	// if segment == "local" or "argument" or "this" or "that"
	{
		writeLine("@" + std::to_string(index));
		writeLine("D=A");
		writeLine("@" + asmSegment);
		writeLine("A=M+D");
		writeLine("D=M");
	}
	else if(segment == "static")
	{
		writeLine("@" + asmSegment);
		writeLine("D=A");
		writeLine("@" + std::to_string(index));
		writeLine("A=D+A");
		writeLine("D=M");
	}
	else if(segment == "constant")
	{
		writeLine("@" + asmSegment);
				writeLine("D=A");
	}
	else// if segment == "pointer" or "temp"
	{
		writeLine("@" + asmSegment);
		writeLine("D=M");
	}
//	Push value on stack and increment SP
	writeLine("@SP");
	writeLine("A=M");
	writeLine("M=D");
	writeLine("@SP");
	writeLine("M=M+1");
}

std::string CodeWriter::segmentToAsm(std::string segment, int index)
{
	auto search = s_directMemorySegments.find(segment);
	if(search != s_directMemorySegments.end())		return s_directMemorySegments[segment];
	else if(segment == "temp")						return "R" + std::to_string(5 + index);
	else if(segment == "pointer")					return "R" + std::to_string(3 + index);
	else if(segment == "static")					return this->currentFileName + "." + std::to_string(index); 		//if segment == "static"
	else return std::to_string(index);
}
void CodeWriter::writeLine(std::string line)
{
	this->outFile << line << std::endl;
}
void CodeWriter::Close()
{
	this->outFile.close();
}

void CodeWriter::writeInit()
{
	// Set SP to 256:
	writeLine("@256");
	writeLine("D=A");
	writeLine("@SP");
	writeLine("M=D");

	writeCall("sys.init", 0);
}

void CodeWriter::writeLabel(std::string label)
{
	writeLine("(" + this->currentFileName + ":" + label + ")");
}

void CodeWriter::writeGoto(std::string label)
{
	writeLine("@" + label);
	writeLine("0;JMP");
}

void CodeWriter::writeIfGoto(std::string label)
{
	writeLine("@SP");
	writeLine("AM=M-1");
	writeLine("D=M");
	writeLine("@" + this->currentFileName + ":" + label);
	writeLine("D;JNE");
}

void CodeWriter::writeCall(std::string functionName, int numArgs)
{
	std::string returnAddrLabel = "@" + this->currentFileName + "." + functionName + std::to_string(this->labelIndex);
//	Return addr
	writeLine("@" + std::to_string(numArgs));
	writeLine("D=A");
	writeLine("@SP");
	writeLine("D=M-D");
	writeLine("A=M");
	writeLine("M=D");
	writeLine(returnAddrLabel);
	writeLine("M=D");
//	save LCL of calling function
	writeLine("@LCL");
	writeLine("D=M");
	writeLine("@SP");
	writeLine("M=M+1");
	writeLine("A=M-1");
	writeLine("M=D");
//	save ARG of calling function
	writeLine("@ARG");
	writeLine("D=M");
	writeLine("@SP");
	writeLine("M=M+1");
	writeLine("A=M-1");
	writeLine("M=D");
//	save THIS of calling function
	writeLine("@THIS");
	writeLine("D=M");
	writeLine("@SP");
	writeLine("M=M+1");
	writeLine("A=M-1");
	writeLine("M=D");
//	save THAT of calling function
	writeLine("@THAT");
	writeLine("D=M");
	writeLine("@SP");
	writeLine("M=M+1");
	writeLine("A=M-1");
	writeLine("M=D");
//	Reposition ARG
	writeLine("@" + std::to_string(numArgs));
	writeLine("D=A");
	writeLine("@5");
	writeLine("D=D+A");
	writeLine("@SP");
	writeLine("D=M-D");
	writeLine("@ARG");
	writeLine("M=D");
	writeLine("@SP");
	writeLine("D=M");
	writeLine("@LCL");
	writeLine("M=D");

	writeLine("@" + functionName);
	writeLine("0;JMP");
	writeLine("(" + returnAddrLabel + ")");

	this->labelIndex++;
}
void CodeWriter::writeReturn()
{
	writeLine("@LCL");
	writeLine("D=M");
	writeLine("@FRAME");
	writeLine("M=D");

	//RET
	writeLine("@5");
	writeLine("A=D-A");
	writeLine("D=M");
	writeLine("@RET");
	writeLine("M=D");	// store return address

	//	Reposition return value for caller
	writeLine("@SP");
	writeLine("A=M-1");
	writeLine("D=M");
	writeLine("@ARG");
	writeLine("A=M");
	writeLine("M=D");

	//	Restore SP for caller
	writeLine("D=A+1");
	writeLine("@SP");
	writeLine("M=D");

	// 	Restore THAT of calling function
	writeLine("@FRAME");
	writeLine("A=M-1");
	writeLine("D=M");
	writeLine("@THAT");
	writeLine("M=D");
	//	Restore This of calling function
	writeLine("@FRAME");
	writeLine("A=M-1");
	writeLine("A=A-1");
	writeLine("D=M");
	writeLine("@THIS");
	writeLine("M=D");
	//	Restore ARG of calling function
	writeLine("@3");
	writeLine("D=A");
	writeLine("@FRAME");
	writeLine("A=M-D");
	writeLine("D=M");
	writeLine("@ARG");
	writeLine("M=D");
	//	Restore LCL of calling function
	writeLine("@4");
	writeLine("D=A");
	writeLine("@FRAME");
	writeLine("A=M-D");
	writeLine("D=M");
	writeLine("@LCL");
	writeLine("M=D");
	// goto the return address
	writeLine("@RET");
	writeLine("A=M");
	writeLine("0;JMP");
}
void CodeWriter::writeFunction(std::string functionName, int numLocals)
{
//	declare label for function entry
	writeLine("(" + this->currentFileName + "." + functionName + ")");

//	Initialize local segment
	writeLine("@" + std::to_string(numLocals));
	writeLine("D=A");
	writeLine("@i");
	writeLine("M=D-1");

	writeLine("(" + functionName + ".LCL.INIT" + ")");
	writeLine("@SP");
	writeLine("M=M+1");
	writeLine("A=M-1");
	writeLine("M=0");
	writeLine("@i");
	writeLine("D=M");
	writeLine("M=M-1");
	writeLine("@" + functionName + ".LCL.INIT");
	writeLine("D;JNE");
}
