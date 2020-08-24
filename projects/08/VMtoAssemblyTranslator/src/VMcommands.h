/*
 * VMcommands.h
 *
 *  Created on: 16 sie 2020
 *      Author: FoxCode
 */

#ifndef VMCOMMANDS_H_
#define VMCOMMANDS_H_

enum class cmdType
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

#endif /* VMCOMMANDS_H_ */
