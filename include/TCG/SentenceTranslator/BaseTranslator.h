#ifndef INCLUDE_TCG_SENTENCETRANSLATOR_BASETRANSLATOR_H_
#define INCLUDE_TCG_SENTENCETRANSLATOR_BASETRANSLATOR_H_

#include "Common/Common.h"
#include "Common/TAC.h"
#include "TCG/SymbolManager.h"
#include "TCG/ASM.h"

class BaseTranslator {
public:
    // 完成对语句的翻译
    std::string construct_asm(std::string op, REG dst, REG src);
    std::string construct_asm(std::string op, REG dst, int src);
    std::string construct_asm(std::string op, REG dst, std::string src);
    std::string construct_asm(std::string op, std::string src);
    std::string construct_asm(std::string op, REG src);
    std::string construct_asm(std::string op, int src);
    std::string construct_asm(std::string op);
    virtual ASMLines SentenceTranslate(SymbolManager&, TACLine&) = 0;
};


#endif // INCLUDE_TCG_SENTENCETRANSLATOR_BASETRANSLATOR_H_
