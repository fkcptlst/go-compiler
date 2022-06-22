#include "Common/Common.h"
#include "TCG/SentenceTranslator/CreatelistTranslator.h"
#include "TCG/ASM.h"
#include "TCG/SymbolManager.h"
#include "TCG/ConstructASM.h"


ASMLines CreatelistTranslator::SentenceTranslate_(SymbolManager& SymbolManager_, TACLine& TACLine_) {
    ASMLines asmlines;
    std::string array_str_encode = SymbolManager_.encode_var(TACLine_.src1.value);
    int array_space = 4 * std::stoi(TACLine_.src2.value);
    SymbolManager_.set_esp_bias(4);
    asmlines.push_back(construct_asm("sub", REG::ESP, std::to_string(4)));
    int array_ptr_mem = SymbolManager_.get_esp() - 4;
    SymbolManager_.set_avalue_mem(array_str_encode, array_ptr_mem);
    SymbolManager_.set_esp_bias(array_space);
    asmlines.push_back(construct_asm("sub", REG::ESP, std::to_string(array_space)));
    asmlines.push_back(construct_asm("mov", array_ptr_mem, REG::ESP));

    return asmlines;
}
