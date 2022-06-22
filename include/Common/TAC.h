#ifndef INCLUDE_COMMON_TAC_H_
#define INCLUDE_COMMON_TAC_H_

#include "Common/Common.h"
#include "Common/UseInfo.h"
#include "Common/Scope.h"

struct Scope;


enum class TACOP {
	ADD, SUB, MUL, DIV, ASSIGN, CALL, PARA, RET, ENDCALL, FUN_RET, FUN_PARA, GOTO,
	IFGT, IFGE, IFLT, IFLE, IFEQ, IFNEQ, LABEL, CREATLIST
};

std::string to_string(const TACOP op);


enum class TACOPERANDTYPE {
	IMM, VAR, NULL_, PTR,
	LABEL // eg: ELSE0
};

struct Operand {
	std::string 		value;
	UseInfo 			use_info;
	TACOPERANDTYPE		OperType;


	Operand(std::string value, TACOPERANDTYPE type)
	: value(value), use_info(), OperType(type) {}

	Operand() {}
};


struct TACLine {
	int64_t line;				/* 行号 */
	TACOP op;
	Operand src1, src2;
	Operand dst;
	std::shared_ptr<Scope> scope;

	TACLine(int64_t line, TACOP op, Operand src1, Operand src2, Operand dst, std::shared_ptr<Scope> scope_)
	: line(line), op(op), src1(src1), src2(src2), dst(dst), scope(scope_) {}

	TACLine() {}

	std::string to_string() const;
};


using TACBlock = std::vector<TACLine>;

using TACFile = std::unordered_map<std::string,std::shared_ptr<TACBlock>>;


inline std::string TACLine::to_string() const {
	return std::to_string(line) + ":(" + ::to_string(op) + "," + src1.value + "," + src2.value + "," + dst.value + ")";
}

#endif	// INCLUDE_COMMON_TAC_H_
