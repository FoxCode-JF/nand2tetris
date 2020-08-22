/*
 * SymbolTable.h
 *
 *  Created on: 21 Aug 2020
 *      Author: Fox
 */

#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

#include <unordered_map>
#include <string>
#include <bitset>

class SymbolTable{
public:
	SymbolTable();
	virtual ~SymbolTable();

	void addEntry(std::string, std::bitset<16>);
	bool Contains(std::string);
	std::bitset<16> GetAddress(std::string);

private:
	std::unordered_map<std::string, std::bitset<16>> s_symbols;
};

#endif /* SYMBOLTABLE_H_ */
