/*
 * Code.h
 *
 *  Created on: 20 Aug 2020
 *      Author: Fox
 */

#ifndef CODE_H_
#define CODE_H_

#include "SymbolTable.h"
#include <string>
#include <unordered_map>
#include <bitset>

class Code {
public:
	Code();
	virtual ~Code();

	std::string dest(std::string mnemonic);
	std::string comp(std::string mnemonic);
	std::string jump(std::string mnemonic);
	std::string symbolBits(std::string symbol);
	SymbolTable hackTab;

private:
	std::unordered_map<std::string, std::string> 		s_opcodes;
	std::unordered_map<std::string, std::string> 		s_jumps;
	std::unordered_map<std::string, std::string> 		s_dests;
};

#endif /* CODE_H_ */
