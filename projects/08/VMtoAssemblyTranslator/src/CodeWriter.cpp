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

CodeWriter::CodeWriter() :
labelIndex(0),
callIndex(0),
staticIndex(0)
{}
CodeWriter::CodeWriter(std::string filePath) :
labelIndex(0),
callIndex(0),
staticIndex(0)
{
	this->outFile.open(filePath + ".asm");

	if(!outFile.is_open())
	{
		std::cerr << "File didn't open correctly";
	}

	size_t found = filePath.find_last_of("/\\");
	if (found!=std::string::npos)
	{
		this->currentFileName = filePath.substr(found + 1, filePath.size() - found);
//		std::cout << this->currentFileName << std::endl;
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
	size_t foundSlash = fileName.find_last_of("/\\");
	if(foundSlash!=std::string::npos)
	{
		fileName = fileName.substr(foundSlash + 1, fileName.length()-1);
	}
	size_t foundExtention = fileName.find_last_of(".");
	this->currentInputFileName = fileName.substr(0, foundExtention);
/*	if(outFile.is_open())	outFile.close();


	this->currentFileName = filePath + ".asm";
	this->outFile.open(this->currentFileName);

	if(!outFile.is_open())
	{
		std::cerr << "File didn't open correctly";
		return;
	}
	size_t found = filePath.find_last_of("/\\");
	if (found!=std::string::npos)
	{
		this->currentFileName = filePath.substr(found + 1, filePath.size() - found);
		std::cout << this->currentFileName << std::endl;
	}*/
}
void CodeWriter::writeArithmetic(std::string command)
{
//	Write comment with command to file
	writeLine("//\t" + command);

//	Point at y value on the stack and decrement SP
	writeLine("@SP");
	writeLine("A=M-1");

//  Perform operations needing only y value
	if(command == "neg")
	{
		writeLine("M=-M");
		return;
	} else if(command == "not")
	{
		writeLine("M=!M");
		return;
	}

	writeLine("D=M");	//	Save y value in D register
	writeLine("A=A-1");	//	Point at x value on the stack

//	Perform operations on both values x and y
	if(command == "add")
	{
		writeLine("M=D+M");
	} else if(command == "sub")
	{
		writeLine("M=M-D");
	} else if(command == "eq")
	{
		writeLine("D=D-M");		// y-x
		writeLine("@IS_EQ");
		writeLine( "D;JEQ");
		writeLine("@SP");
		writeLine("A=M-1");
		writeLine("A=A-1");
		writeLine("M=0");		// D = false
		writeLine("@END");
		writeLine("0;JMP");
		writeLine("(IS_EQ" + std::to_string(labelIndex) +")");
		writeLine("@SP");
		writeLine("A=M-1");
		writeLine("A=A-1");
		writeLine("M=-1");
		writeLine("(END" + std::to_string(labelIndex) + ")");

	} else if(command == "gt")
	{
		writeLine("D=M-D");
		writeLine("@IS_GREATER" + std::to_string(labelIndex));
		writeLine("D;JGT");
		writeLine("@SP");
		writeLine("A=M-1");
		writeLine("A=A-1");
		writeLine("M=0");		// D = false
		writeLine("@END" + labelIndex);
		writeLine("0;JMP");
		writeLine("(IS_GREATER" + std::to_string(labelIndex) + ")");
		writeLine("@SP");
		writeLine("A=M-1");
		writeLine("A=A-1");
		writeLine("M=-1");		// D = true
		writeLine("(END" + std::to_string(labelIndex) + ")");
	} else if(command == "lt")
	{
		writeLine("D=M-D");
		writeLine("@IS_LOWER" + std::to_string(labelIndex));
		writeLine("D;JLT");
		writeLine("@SP");
		writeLine("A=M-1");
		writeLine("A=A-1");
		writeLine("M=0");		// D = false
		writeLine("@END" + std::to_string(labelIndex));
		writeLine("0;JMP");
		writeLine("(IS_LOWER" + std::to_string(labelIndex) + ")");
		writeLine("@SP");
		writeLine("A=M-1");
		writeLine("A=A-1");
		writeLine("M=-1");		// D = true
		writeLine("(END" + std::to_string(labelIndex) + ")");
	} else if(command == "and")
	{
		writeLine("M=M&D");
	} else if(command == "or")
	{
		writeLine("M=M|D");
	}

	writeLine("@SP");
	writeLine("M=M-1");

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
//		writeLine("@" + std::to_string(index));
//		writeLine("D=D+A");
	}else// if segment == "pointer" or "temp"
	{
		writeLine("@" + asmSegment);
		writeLine("D=A");
	}
	writeLine("@R13");
	writeLine("M=D");
	writeLine("@SP");
	writeLine("AM=M-1");
	writeLine("D=M");
	writeLine("@R13");
	writeLine("A=M");
	writeLine("M=D");
}

void CodeWriter::writePush(std::string segment, int index)
{
	std::string asmSegment = segmentToAsm(segment, index);
	auto search = s_directMemorySegments.find(segment);

	if(search != s_directMemorySegments.end() /*|| segment == "static"*/) 	// if segment == "local" or "argument" or "this" or "that" or "static"
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
//		writeLine("D=A");
//		writeLine("@" + std::to_string(index));
//		writeLine("A=D+A");
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
	else if(segment == "static")					return "static." + this->currentInputFileName + "." + std::to_string(index); 		//if segment == "static"
	else return std::to_string(index);
}
void CodeWriter::writeLine(std::string line)
{
	this->outFile << line << std::endl;
}
void CodeWriter::Close()
{
	writeLine("File closed\n");
	this->outFile.close();
}

void CodeWriter::writeInit()
{
	writeLine("//\tsys.Init");
	// Set SP to 256:
	writeLine("@256");
	writeLine("D=A");
	writeLine("@SP");
	writeLine("M=D");

	writeCall("Sys.init", 0);
}

void CodeWriter::writeLabel(std::string label)
{
	writeLine("//\tlabel " + label);
	writeLine("(" + this->currentFileName + "." + label + ")");
}

void CodeWriter::writeGoto(std::string label)
{
	writeLine("//\tgoto " + label);
	writeLine("@" + this->currentFileName + "." + label);
	writeLine("0;JMP");
}

void CodeWriter::writeIfGoto(std::string label)
{
	writeLine("//\tif-goto " + label);
	writeLine("@SP");
	writeLine("AM=M-1");
	writeLine("D=M");
	writeLine("@" + this->currentFileName + "." + label);
	writeLine("D;JNE");
}

// TODO fix writeCall()
void CodeWriter::writeCall(std::string functionName, int numArgs)
{
	writeLine("//\tcall " + functionName + " " + std::to_string(numArgs));
	std::string returnAddrLabel = /*this->currentFileName + */"." + functionName + std::to_string(this->callIndex);
//	Return addr

	writeLine("@" + returnAddrLabel);
	writeLine("D=A");
	writeLine("@SP");
	writeLine("A=M");
	writeLine("M=D");
	writeLine("@SP");
	writeLine("M=M+1");
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

	writeGoto(functionName);
	writeLine("(" + returnAddrLabel + ")");

	this->callIndex++;
}
void CodeWriter::writeReturn()
{
	writeLine("//\treturn");
	writeLine("@LCL");
	writeLine("D=M");
	writeLine("@R14");
	writeLine("M=D");

	//RET
	writeLine("@5");
	writeLine("A=D-A");
	writeLine("D=M");
	writeLine("@R15");
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
	writeLine("@R14");
	writeLine("A=M-1");
	writeLine("D=M");
	writeLine("@THAT");
	writeLine("M=D");
	//	Restore This of calling function
	writeLine("@R14");
	writeLine("A=M-1");
	writeLine("A=A-1");
	writeLine("D=M");
	writeLine("@THIS");
	writeLine("M=D");
	//	Restore ARG of calling function
	writeLine("@3");
	writeLine("D=A");
	writeLine("@R14");
	writeLine("A=M-D");
	writeLine("D=M");
	writeLine("@ARG");
	writeLine("M=D");
	//	Restore LCL of calling function
	writeLine("@4");
	writeLine("D=A");
	writeLine("@R14");
	writeLine("A=M-D");
	writeLine("D=M");
	writeLine("@LCL");
	writeLine("M=D");
	// goto the return address
	writeLine("@R15");
	writeLine("A=M");
	writeLine("0;JMP");
}
void CodeWriter::writeFunction(std::string functionName, int numLocals)
{
	writeLine("//\tfunction " + functionName + " " + std::to_string(numLocals));
//	declare label for function entry
	writeLine("(" + this->currentFileName + "." + functionName + ")");

//	Initialize local segment
	writeLine("@" + std::to_string(numLocals));
	writeLine("D=A");
	writeLine("@i");
	writeLine("M=D");

	writeLine("@" + functionName + "NOARGS");
	writeLine("D;JEQ");
	writeLine("(" + functionName + ".LCL.INIT)");
	writeLine("@SP");
	writeLine("M=M+1");
	writeLine("A=M-1");
	writeLine("M=0");
	writeLine("@i");
	writeLine("D=M");
	writeLine("M=M-1");
	writeLine("@" + functionName + ".LCL.INIT");
	writeLine("D;JGT");
	writeLine("(" + functionName + "NOARGS)");
}
