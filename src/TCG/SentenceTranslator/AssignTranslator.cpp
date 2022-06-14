#include "Common/Common.h"
#include "TCG/SentenceTranslator/AssignTranslator.h"
#include "TCG/ASM.h"
#include "TCG/SymbolManager.h"

ASMLines AssignTranslator::SentenceTranslate_(SymbolManager& SymbolManager_, TACLine& TACLine_) {
    ASMLines asmlines;
    std::string str_dst = SymbolManager_.encode_var(TACLine_.dst.value);
    REG freereg = SymbolManager_.get_free_reg();
    if (freereg == REG::None) {
        freereg = SymbolManager_.get_reg();
        SymbolManager_.push_reg(freereg);
        asmlines.push_back(construct_asm("push", freereg));
    }
    SymbolManager_.set_avalue_reg(str_dst, freereg);
    std::string str_src1 = TACLine_.src1.value;
    if (TACLine_.src1.OperType == TACOPERANDTYPE::IMM) {
        asmlines.push_back(construct_asm("mov", freereg, str_src1));
    } else if (TACLine_.src1.OperType == TACOPERANDTYPE::VAR) {
        std::string encode_str = SymbolManager_.encode_var(str_src1);
        POSTYPE pos = SymbolManager_.position(encode_str);
        switch (pos) {
            case POSTYPE::REG: {
                REG src_reg = SymbolManager_.avalue_reg(encode_str);
                asmlines.push_back(construct_asm("mov", freereg, src_reg));
                break;
            }
            case POSTYPE::MEM: {
                int src_mem = SymbolManager_.avalue_mem(encode_str);
                asmlines.push_back(construct_asm("mov", freereg, src_mem));
                break;
            }
            case POSTYPE::GLOBAL: {
                asmlines.push_back(construct_asm("mov", freereg, str_src1));
                break;
            }
            case POSTYPE::FUNPARA: {
                std::cout << "assign funpara error" << std::endl;
                break;
            }
            default: {
                std::cout << "assign default error" << std::endl;
                break;
            }
        }
    }
    return asmlines;
}
