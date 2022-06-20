#include "Common/Common.h"
#include "TCG/SentenceTranslator/AssignTranslator.h"
#include "TCG/ASM.h"
#include "TCG/SymbolManager.h"
#include "TCG/ConstructASM.h"

ASMLines AssignTranslator::SentenceTranslate_(SymbolManager& SymbolManager_, TACLine& TACLine_) {
    ASMLines asmlines;
    std::string str_dst_encode = SymbolManager_.encode_var(TACLine_.dst.value);
    REG dst_reg = SymbolManager_.get_reg(TACLine_);
    if (dst_reg == REG::None) {
        dst_reg = SymbolManager_.get_reg();
        SymbolManager_.push_reg(dst_reg);
        asmlines.push_back(construct_asm("push", dst_reg));
    }
    std::string str_src1 = TACLine_.src1.value;
    if (TACLine_.src1.OperType == TACOPERANDTYPE::IMM) {
        asmlines.push_back(construct_asm("mov", dst_reg, str_src1));
    } else if (TACLine_.src1.OperType == TACOPERANDTYPE::VAR) {
        std::string encode_str_src1 = SymbolManager_.encode_var(str_src1);
        POSTYPE pos = SymbolManager_.position(encode_str_src1);
        switch (pos) {
            case POSTYPE::REG: {
                REG src_reg = SymbolManager_.avalue_reg(encode_str_src1);
                if (dst_reg == src_reg) {
                    SymbolManager_.push_reg(src_reg);
                    asmlines.push_back(construct_asm("push", src_reg));
                } else {
                    asmlines.push_back(construct_asm("mov", dst_reg, src_reg));
                }
                break;
            }
            case POSTYPE::MEM: {
                int src_mem = SymbolManager_.avalue_mem(encode_str_src1);
                asmlines.push_back(construct_asm("mov", dst_reg, src_mem));
                break;
            }
            case POSTYPE::GLOBAL: {
                asmlines.push_back(construct_asm("mov", dst_reg, str_src1));
                break;
            }
            default: {
                std::cout << "assign default error" << std::endl;
                break;
            }
        }
    }
    SymbolManager_.set_avalue_reg(str_dst_encode, dst_reg);
    if (SymbolManager_.avalue_mem(str_dst_encode) != -1) {
        int dst_mem = SymbolManager_.avalue_mem(str_dst_encode);
        asmlines.push_back(construct_asm("mov", dst_mem, dst_reg));
    }
    return asmlines;
}
