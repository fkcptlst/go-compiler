#include "Common/Common.h"
#include "TCG/SentenceTranslator/CommonTranslator.h"
#include "TCG/ASM.h"
#include "TCG/SymbolManager.h"

ASMLines CommonTranslator::SentenceTranslate(SymbolManager& SymbolManager_, TACLine& TACLine_) {
    ASMLines ASMLines_;
    std::string str_dst = TACLine_.dst.value;
    std::string str_src1 = TACLine_.src1.value;
    std::string str_src2 = TACLine_.src2.value;
    REG reg_dst = SymbolManager_.get_reg(TACLine_);
    REG reg_src1 = SymbolManager_.avalue_reg(TACLine_.src1.value);
    REG reg_src2 = SymbolManager_.avalue_reg(TACLine_.src2.value);
    // todo ljh
    // if (reg_dst != reg_src1) {
    //     construct_asm("mov", reg_dst, reg_src1);
    // }
    // construct_asm("op", reg_dst, reg_src2);
}
