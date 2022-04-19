#include "Common.h"
#include "TCG/SentenceTranslator/CommonTranslator.h"
#include "TCG/ASM.h"
#include "TCG/SymbolManager.h"

ASMLines CommonTranslator::SentenceTranslate(SymbolManager& SymbolManager_, TACLine& TACLine_) {
    ASMLines ASMLines_;
    REG reg_des = SymbolManager_.get_reg(TACLine_);
    REG reg_src1 = SymbolManager_.avalue_reg(TACLine_.src1.value);
    REG reg_src2 = SymbolManager_.avalue_reg(TACLine_.src2.value);
    
}
