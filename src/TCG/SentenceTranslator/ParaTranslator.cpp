#include "Common/Common.h"
#include "TCG/SentenceTranslator/ParaTranslator.h"
#include "TCG/ASM.h"
#include "TCG/SymbolManager.h"

ASMLines ParaTranslator::SentenceTranslate_(SymbolManager& SymbolManager_, TACLine& TACLine_) {
    ASMLines asmlines;
    std::string str_src1 = SymbolManager_.encode_var(TACLine_.src1.value);

    POSTYPE pos = SymbolManager_.position(str_src1);
    switch (pos) {
    case POSTYPE::REG: {
        REG reg_src1 = SymbolManager_.avalue_reg(str_src1);
        asmlines.push_back(construct_asm("push", reg_src1));
        SymbolManager_.push_reg(reg_src1);
        break;
    }
    case POSTYPE::MEM: {
        // int mem_src1 = SymbolManager_.avalue_mem(str_src1);
        // REG free_reg = SymbolManager_.get_free_reg();
        // if (free_reg == REG::None) {
        //     free_reg = SymbolManager_.get_reg();
        //     SymbolManager_.set_esp_bias(4);
        //     asmlines.push_back(construct_asm("sub", REG::ESP, 4));
        //     SymbolManager_.push_reg(free_reg);
        //     asmlines.push_back(construct_asm("push", free_reg));
        //     SymbolManager_.set_avalue_reg(str_src1, free_reg);
        //     asmlines.push_back(construct_asm("mov", free_reg, mem_src1));
        //     SymbolManager_.set_avalue_mem(str_src1, )

        // }
        std::cout << "error" << std::endl;
        break;
    }
    case POSTYPE::FUNPARA: {
        std::cout << "error" << std::endl;
        break;
    }
    case POSTYPE::GLOBAL: {
        std::cout << "error" << std::endl;
        break;
    }
    default: {
        std::cout << "error" << std::endl;
        break;
    }
    }
    return asmlines;
}
