#include "Common/Common.h"
#include "TCG/SentenceTranslator/IfTranslator.h"
#include "TCG/ASM.h"
#include "TCG/SymbolManager.h"
#include "TCG/ConstructASM.h"


ASMLines IfTranslator::SentenceTranslate_(SymbolManager& SymbolManager_, TACLine& TACLine_) {
    ASMLines asmlines;
    std::string str_op = "";
    switch (TACLine_.op) {
        case TACOP::IFEQ: str_op = "je"; break;
        case TACOP::IFGE: str_op = "jge"; break;
        case TACOP::IFGT: str_op = "jg"; break;
        case TACOP::IFLE: str_op = "jle"; break;
        case TACOP::IFLT: str_op = "jl"; break;
        case TACOP::IFNEQ: str_op = "jne"; break;
    }
    std::string str_label = TACLine_.dst.value;

    std::string str_src1 = TACLine_.src1.value;
    std::string str_src2 = TACLine_.src2.value;
    
    REG reg_src1 = REG::None;
    if (TACLine_.src1.OperType == TACOPERANDTYPE::IMM) {
        reg_src1 = REG::EDI;
        asmlines.push_back(construct_asm("mov", reg_src1, str_src1));
    } else {
        std::string str_src1_encode = SymbolManager_.encode_var(str_src1);
        POSTYPE pos = SymbolManager_.position(str_src1_encode);
        switch (pos) {
            case POSTYPE::REG: {
                reg_src1 = SymbolManager_.avalue_reg(str_src1_encode);
                break;
            }
            case POSTYPE::MEM: {
                int mem_src1 = SymbolManager_.avalue_mem(str_src1_encode);
                reg_src1 = REG::EDI;
                asmlines.push_back(construct_asm("mov", reg_src1, mem_src1));
                break;
            }
            case POSTYPE::GLOBAL: {
                reg_src1 = REG::EDI;
                asmlines.push_back(construct_asm("mov", reg_src1, str_src1));
                break;
            }
            default: {
                LOG(ERROR) << "if src1 default error";
                break;
            }
        }
    }

    if (TACLine_.src2.OperType == TACOPERANDTYPE::IMM) {
        asmlines.push_back(construct_asm("cmp", reg_src1, str_src2));
    } else if (TACLine_.src2.OperType == TACOPERANDTYPE::VAR) {
        std::string str_src2_encode = SymbolManager_.encode_var(str_src2);
        POSTYPE pos = SymbolManager_.position(str_src2_encode);
        switch (pos) {
            case POSTYPE::REG: {
                REG reg_src2 = SymbolManager_.avalue_reg(str_src2_encode);
                asmlines.push_back(construct_asm("cmp", reg_src1, reg_src2));
                break;
            }
            case POSTYPE::MEM: {
                int mem_src2 = SymbolManager_.avalue_mem(str_src2_encode);
                asmlines.push_back(construct_asm("cmp", reg_src1, mem_src2));
                break;
            }
            case POSTYPE::GLOBAL: {
                asmlines.push_back(construct_asm("cmp", reg_src1, str_src2));
                break;
            }
            default: {
                std::cout << "if src2 default error" << std::endl;
                break;
            }
        }
    }
    asmlines.push_back(construct_asm(str_op, str_label));    
    return asmlines;
}
