#include "Common/Common.h"
#include "TCG/SentenceTranslator/CommonTranslator.h"
#include "TCG/ASM.h"
#include "TCG/SymbolManager.h"

ASMLines CommonTranslator::SentenceTranslate_(SymbolManager& SymbolManager_, TACLine& TACLine_) {
    ASMLines asmlines;
    std::string str_dst = SymbolManager_.encode_var(TACLine_.dst.value);
    std::string str_src1 = SymbolManager_.encode_var(TACLine_.src1.value);
    std::string str_src2 = SymbolManager_.encode_var(TACLine_.src2.value);
    REG reg_dst = SymbolManager_.get_reg(str_dst, str_src1, str_src2);
    REG reg_src1 = SymbolManager_.avalue_reg(str_src1);
    REG reg_src2 = SymbolManager_.avalue_reg(str_src2);
    // todo ljh
    if (reg_dst != reg_src1) {
        asmlines.push_back(construct_asm("mov", reg_dst, reg_src1));
    }
    asmlines.push_back(construct_asm("oopp", reg_dst, reg_src2));
    return asmlines;
}
