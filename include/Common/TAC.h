#ifndef INCLUDE_COMMON_TAC_H_
#define INCLUDE_COMMON_TAC_H_

#include "Common/Common.h"
#include "Common/UseInfo.h"


enum class TACOP {
	ADD, SUB, MUL, DIV, DEF, ASSIGN,
};


struct Operand {
	std::string 		value;
	UseInfo 			use_info;

	Operand(std::string value)
	: value(value), use_info() {}
};


struct TACLine {
	int64_t line;				/* 行号 */
	TACOP op;
	Operand src1, src2;
	Operand dst;

	TACLine(int64_t line, TACOP op, Operand src1, Operand src2, Operand dst)
	: line(line), op(op), src1(src1), src2(src2), dst(dst) {}

	std::string to_string() const;
};


using TACBlock = std::vector<TACLine>;

using TACFile = std::vector<TACBlock>;


inline std::string TACLine::to_string() const {
	std::string op_str;
	switch (op) {
		case TACOP::ADD: {
			op_str = "+";
			break;
		} case TACOP::SUB: {
			op_str = "-";
			break;
		} case TACOP::MUL: {
			op_str = "*";
			break;
		} case TACOP::DIV: {
			op_str = "/";
			break;
		} default: {
			op_str = "?";
			break;
		}
	}
	return std::to_string(line) + ":(" + op_str + "," + src1.value + "," + src2.value + "," + dst.value + ")";
}

#endif	// INCLUDE_COMMON_TAC_H_