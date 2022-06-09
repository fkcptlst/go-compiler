#include "Common/Common.h"
#include "TCG/SentenceTranslator/RetTranslator.h"
#include "TCG/ASM.h"
#include "TCG/SymbolManager.h"

ASMLines RetTranslator::SentenceTranslate_(SymbolManager& SymbolManager_, TACLine& TACLine_) {
    ASMLines asmlines;
    std::string str_src1 = SymbolManager_.encode_var(TACLine_.src1.value);
    SymbolManager_.set_esp_bias(4);
    SymbolManager_.set_avalue_mem(str_src1, SymbolManager_.get_esp() - 4);
    asmlines.push_back(construct_asm("sub", REG::ESP, 4));
    return asmlines;
}
