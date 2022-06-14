#include "Common/Common.h"
#include "TCG/SentenceTranslator/ParaTranslator.h"
#include "TCG/ASM.h"
#include "TCG/SymbolManager.h"

ASMLines ParaTranslator::SentenceTranslate_(SymbolManager& SymbolManager_, TACLine& TACLine_) {
    ASMLines asmlines;

    if (TACLine_.src1.OperType == TACOPERANDTYPE::IMM) {
        std::string src = TACLine_.src1.value;
        SymbolManager_.set_esp_bias(4);
        asmlines.push_back(construct_asm("push", src));
    } else if (TACLine_.src1.OperType == TACOPERANDTYPE::VAR) {
        std::string str_src1 = SymbolManager_.encode_var(TACLine_.src1.value);
        POSTYPE pos = SymbolManager_.position(str_src1);

        switch (pos) {
            case POSTYPE::REG: {
                REG reg_src1 = SymbolManager_.avalue_reg(str_src1);
                SymbolManager_.push_reg(reg_src1, 0);
                asmlines.push_back(construct_asm("push", reg_src1));
                break;
            }
            case POSTYPE::MEM: {
                int mem_src1 = SymbolManager_.avalue_mem(str_src1);
                asmlines.push_back(construct_asm("mov", REG::EDI, mem_src1));
                SymbolManager_.push_reg(REG::EDI, 0);
                asmlines.push_back(construct_asm("push", REG::EDI));
                break;
            }
            case POSTYPE::FUNPARA: {
                std::cout << "para funpara error" << std::endl;
                break;
            }
            case POSTYPE::GLOBAL: {
                std::string src = TACLine_.src1.value;
                asmlines.push_back(construct_asm("mov", REG::EDI, src));
                SymbolManager_.push_reg(REG::EDI, 0);
                asmlines.push_back(construct_asm("push", REG::EDI));
                break;
            }
            default: {
                std::cout << "para default error" << std::endl;
                break;
            }
        }
    }

    return asmlines;
}
