#include "Common/TAC.h"


std::string to_string(TACOP op) {
	switch (op) {
		case TACOP::ADD: return "add";
		case TACOP::SUB: return "sub";
		case TACOP::MUL: return "mul";
		case TACOP::DIV: return "div";
		case TACOP::ASSIGN: return "assign";
		case TACOP::CALL: return "call";
		case TACOP::PARA: return "para";
		case TACOP::RET: return "ret";
		case TACOP::ENDCALL: return "endcall";
		case TACOP::FUN_RET: return "funret";
		case TACOP::FUN_PARA: return "funpara";
		case TACOP::IFEXP: return "ifexp";
		case TACOP::ENDIF: return "endif";
		case TACOP::GOTO: return "goto";
		case TACOP::ELSE: return "else";
		case TACOP::IFGT: return "ifgt";
		case TACOP::IFGE: return "ifge";
		case TACOP::IFLT: return "iflt";
		case TACOP::IFLE: return "ifle";
		case TACOP::IFEQ: return "ifeq";
		case TACOP::IFNEQ: return "ifneq";
		case TACOP::LABEL: return "label";
		default: return "error_op";
	}
}
