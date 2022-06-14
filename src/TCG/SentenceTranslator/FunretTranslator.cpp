#include "Common/Common.h"
#include "TCG/SentenceTranslator/FunretTranslator.h"
#include "TCG/ASM.h"
#include "TCG/SymbolManager.h"
#include "TCG/ConstructASM.h"

ASMLines FunretTranslator::SentenceTranslate_(SymbolManager& SymbolManager_, TACLine& TACLine_) {
    ASMLines asmlines;
    std::string ret_src = TACLine_.src1.value;
    int ret_mem = SymbolManager_.ret();
    if (TACLine_.src1.OperType == TACOPERANDTYPE::IMM) {
        asmlines.push_back(construct_asm("mov", ret_mem, ret_src));
    } else if (TACLine_.src1.OperType == TACOPERANDTYPE::VAR) {
        std::string ret_src_encode = SymbolManager_.encode_var(ret_src);
        POSTYPE pos = SymbolManager_.position(ret_src_encode);
        switch (pos) {
            case POSTYPE::REG: {
                REG ret_src_reg = SymbolManager_.avalue_reg(ret_src_encode);
                asmlines.push_back(construct_asm("mov", ret_mem, ret_src_reg));
                break;
            }
            case POSTYPE::MEM: {
                int ret_src_mem = SymbolManager_.avalue_mem(ret_src_encode);
                asmlines.push_back(construct_asm("mov", REG::EDI, ret_src_mem));
                asmlines.push_back(construct_asm("mov", ret_mem, REG::EDI));
                break;
            }
            case POSTYPE::GLOBAL: {
                asmlines.push_back(construct_asm("mov", ret_mem, ret_src));
                break;
            }
            default: {
                std::cout << "funret default error" << std::endl;
                break;
            }
        }
    }
    return asmlines;
}
