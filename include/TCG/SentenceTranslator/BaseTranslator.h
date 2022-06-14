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
    ASMLines SentenceTranslate(SymbolManager&, TACLine&);
    virtual ASMLines SentenceTranslate_(SymbolManager&, TACLine&) = 0;
};

inline ASMLines BaseTranslator::SentenceTranslate(SymbolManager& SymbolManager_, TACLine& TACLine_) {
    SymbolManager_.set_scope(TACLine_.scope);
    return this->SentenceTranslate_(SymbolManager_, TACLine_);
}

inline std::string BaseTranslator::construct_asm(std::string op, REG dst, REG src) {
    return op + " \t " + tostring(dst) + " \t , \t " + tostring(src);
}

inline std::string BaseTranslator::construct_asm(std::string op, REG dst, int src) {
    return op + " \t " + tostring(dst) + " \t , \t " + "[ ebp" + (src > 0 ? " - " : " + ") + std::to_string(src) + " ]";
}

inline std::string BaseTranslator::construct_asm(std::string op, REG dst, std::string src) {
    return op + " \t " + tostring(dst) + " \t , \t " + src;
}

inline std::string BaseTranslator::construct_asm(std::string op, std::string src) {
    return op + " \t " + src;
}

inline std::string BaseTranslator::construct_asm(std::string op, REG src) {
    return op + " \t " + tostring(src);
}

inline std::string BaseTranslator::construct_asm(std::string op, int src) {
    return op + " \t " + "[ ebp" + (src > 0 ? " - " : " + ") + std::to_string(src) + " ]";
}

inline std::string BaseTranslator::construct_asm(std::string op) {
    return op;
}


#endif // INCLUDE_TCG_SENTENCETRANSLATOR_BASETRANSLATOR_H_
