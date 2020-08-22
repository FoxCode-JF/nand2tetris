/*
 * Code.cpp
 *
 *  Created on: 20 Aug 2020
 *      Author: Fox
 */

#include "Code.h"
#include <iostream>

Code::Code():
s_opcodes({
//		Comp. (a=0)	a c1 c2 c3 c4 c5 c6
		{"0",		"0101010"},
		{"1",		"0111111"},
		{"-1", 		"0111010"},
		{"D",		"0001100"},
		{"A",		"0110000"},
		{"!D", 		"0001101"},
		{"!A", 		"0110001"},
		{"-D", 		"0001111"},
		{"-A", 		"0110011"},
		{"D+1", 	"0011111"},
		{"A+1", 	"0110111"},
		{"D-1", 	"0001110"},
		{"A-1", 	"0110010"},
		{"D+A", 	"0000010"},
		{"D-A", 	"0010011"},
		{"A-D", 	"0000111"},
		{"D&A", 	"0000000"},
		{"D|A", 	"0010101"},
// 		Comp. (a=1)
		{"M",		"1110000"},
		{"!M", 		"1110001"},
		{"-M", 		"1110011"},
		{"M+1", 	"1110111"},
		{"M-1", 	"1110010"},
		{"D+M", 	"1000010"},
		{"D-M", 	"1010011"},
		{"M-D", 	"1000111"},
		{"D&M", 	"1000000"},
		{"D|M", 	"1010101"}
	}),
s_dests({
// 					d1 d2 d3
		{"", 		"000"},
		{"M", 		"001"},
		{"D", 		"010"},
		{"MD", 		"011"},
		{"A", 		"100"},
		{"AM", 		"101"},
		{"AD", 		"110"},
		{"AMD",		"111"}
	}),
s_jumps({
//					j1 j2 j3
		{"", 		"000"},
		{"JGT", 	"001"},
		{"JEQ", 	"010"},
		{"JGE", 	"011"},
		{"JLT", 	"100"},
		{"JNE", 	"101"},
		{"JLE", 	"110"},
		{"JMP",		"111"}
	})
{}

Code::~Code() {

}
std::string Code::dest(std::string mnemonic)
{
	if(s_dests.count(mnemonic))	return s_dests[mnemonic];

	return "";
}
std::string Code::comp(std::string mnemonic)
{
	if(s_opcodes.count(mnemonic))	return s_opcodes[mnemonic];

	return "";
}
std::string Code::jump(std::string mnemonic)
{
	if(s_jumps.count(mnemonic))	return s_jumps[mnemonic];

	return "";
}
std::string Code::symbolBits(std::string symbol)
{
	if(this->hackTab.Contains(symbol))
	{
		std::bitset<16> tmp(hackTab.GetAddress(symbol));
		return tmp.to_string();
	}
	else if(std::isdigit(symbol[0]))
	{
		std::bitset<16> tmp(stoul(symbol, nullptr, 10));
		return tmp.to_string();
	}
	else return "";
}
