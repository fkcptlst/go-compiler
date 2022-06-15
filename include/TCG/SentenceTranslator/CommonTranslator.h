#ifndef INCLUDE_TCG_SENTENCETRANSLATOR_COMMONTRANSLATOR_H_
#define INCLUDE_TCG_SENTENCETRANSLATOR_COMMONTRANSLATOR_H_

#include "Common/Common.h"
#include "TCG/ASM.h"
#include "TCG/SentenceTranslator/BaseTranslator.h"

class CommonTranslator : public BaseTranslator {
public:
    std::string commonop2string(TACOP op);
    ASMLines SentenceTranslate_(SymbolManager&, TACLine&) override;
};

inline std::string CommonTranslator::commonop2string(TACOP op) {
    switch (op) {
        case TACOP::ADD: return "add";
        case TACOP::SUB: return "sub";
        case TACOP::MUL: return "mul";
        case TACOP::DIV: return "div";
        default: return "oopp";
    }
}


#endif // INCLUDE_TCG_SENTENCETRANSLATOR_COMMONTRANSLATOR_H_