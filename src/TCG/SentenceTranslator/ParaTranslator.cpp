#include "Common/Common.h"
#include "TCG/SentenceTranslator/ParaTranslator.h"
#include "TCG/ASM.h"
#include "TCG/SymbolManager.h"

ASMLines ParaTranslator::SentenceTranslate_(SymbolManager& SymbolManager_, TACLine& TACLine_) {
    ASMLines asmlines;
    std::string str_src1 = SymbolManager_.encode_var(TACLine_.src1.value);
    REG reg_src1 = SymbolManager_.avalue_reg(str_src1);
    asmlines.push_back(construct_asm("push", reg_src1));
    SymbolManager_.push_reg(reg_src1);
    return asmlines;
}
