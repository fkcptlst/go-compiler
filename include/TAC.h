#ifndef INCLUDE_TAC_H_
#define INCLUDE_TAC_H_


#include "Common.h"

/* lxyTODO */
enum class TACOP {
	ADD, SUB, MUL, DIV,
};


struct TACLine {
	int64_t line;				/* 行号 */
	TACOP op;
	std::string src1, src2;
	std::string dst;
};


using TACBlock = std::vector<TACLine>;

using TACFile = std::vector<TACBlock>;

#endif	// INCLUDE_TAC_H_