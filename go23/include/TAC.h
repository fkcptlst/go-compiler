#ifndef INCLUDE_TAC_H_
#define INCLUDE_TAC_H_


#include "Common.h"

/* lxyTODO */
enum class TACOP {
	ADD, SUB, MUL, DIV, ASSIGN,DEF, 
};


struct TACLine {
	int64_t line;				/* 行号 */
	TACOP op;
	std::string src1, src2;
	std::string dst;
	
	TACLine(int64_t _line, TACOP _op, std::string _src1, 
	std::string _src2, std::string _dst) : 
	line(_line), op(_op), src1(_src1), src2(_src2), dst(_dst){}
};


using TACBlock = std::vector<TACLine>;

using TACFile = std::vector<TACBlock>;

#endif	// INCLUDE_TAC_H_