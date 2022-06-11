#include "Common/Common.h"
#include "TCG/SentenceTranslator/AssignTranslator.h"
#include "TCG/ASM.h"
#include "TCG/SymbolManager.h"

ASMLines AssignTranslator::SentenceTranslate_(SymbolManager& SymbolManager_, TACLine& TACLine_) {
    ASMLines asmlines;
    std::string str_dst = SymbolManager_.encode_var(TACLine_.dst.value);
    std::string str_src1 = TACLine_.src1.value;
    REG freereg = SymbolManager_.get_free_reg();
    if (freereg == REG::None) {
        freereg = SymbolManager_.get_reg();
        SymbolManager_.push_reg(freereg);
        asmlines.push_back(construct_asm("push", freereg));
    }
    SymbolManager_.set_avalue_reg(str_dst, freereg);
    asmlines.push_back(construct_asm("mov", freereg, str_src1));
    return asmlines;
}
