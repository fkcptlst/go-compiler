#include "Common/Common.h"
#include "TCG/SentenceTranslator/FunparaTranslator.h"
#include "TCG/ASM.h"
#include "TCG/SymbolManager.h"
#include "TCG/ConstructASM.h"


ASMLines FunparaTranslator::SentenceTranslate_(SymbolManager& SymbolManager_, TACLine& TACLine_) {
    ASMLines asmlines;
    std::string par_encode = SymbolManager_.encode_var(TACLine_.src1.value);
    int para_mem = SymbolManager_.para();
    REG dst_reg = SymbolManager_.get_reg(par_encode, "");
    if (dst_reg == REG::None) {
        dst_reg = SymbolManager_.get_reg();
        SymbolManager_.push_reg(dst_reg);
        asmlines.push_back(construct_asm("push", dst_reg));
    }
    asmlines.push_back(construct_asm("mov", dst_reg, para_mem));
    SymbolManager_.set_avalue_reg(par_encode, dst_reg);
    if (SymbolManager_.avalue_mem(par_encode) != -1) {
        int dst_mem = SymbolManager_.avalue_mem(par_encode);
        asmlines.push_back(construct_asm("mov", dst_mem, dst_reg));
    }
    return asmlines;
}
