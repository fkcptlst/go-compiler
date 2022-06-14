#include "Common/Common.h"
#include "TCG/SentenceTranslator/CommonTranslator.h"
#include "TCG/ASM.h"
#include "TCG/SymbolManager.h"
#include "TCG/ConstructASM.h"

ASMLines CommonTranslator::SentenceTranslate_(SymbolManager& SymbolManager_, TACLine& TACLine_) {
    ASMLines asmlines;
    std::string str_dst_encode = SymbolManager_.encode_var(TACLine_.dst.value);
    std::string str_src1 = TACLine_.src1.value;
    std::string str_src2 = TACLine_.src2.value;

    REG reg_dst = REG::None;
    if (TACLine_.src1.OperType == TACOPERANDTYPE::IMM) {
        reg_dst = SymbolManager_.get_reg(str_dst_encode, "");
        if (reg_dst == REG::None) {
            reg_dst = SymbolManager_.get_reg();
            SymbolManager_.push_reg(reg_dst);
            asmlines.push_back(construct_asm("push", reg_dst));
        }
        asmlines.push_back(construct_asm("mov", reg_dst, str_src1));

    } else {
        std::string str_src1_encode = SymbolManager_.encode_var(str_src1);
        reg_dst = SymbolManager_.get_reg(str_dst_encode, str_src1);
        if (reg_dst == REG::None) {
            reg_dst = SymbolManager_.get_reg();
            SymbolManager_.push_reg(reg_dst);
            asmlines.push_back(construct_asm("push", reg_dst));
        }
        POSTYPE pos = SymbolManager_.position(str_src1_encode);
        switch (pos) {
            case POSTYPE::REG: {
                REG reg_src1 = SymbolManager_.avalue_reg(str_src1_encode);
                if (reg_dst != reg_src1) {
                    asmlines.push_back(construct_asm("mov", reg_dst, reg_src1));
                } else {
                    SymbolManager_.push_reg(reg_src1);
                    asmlines.push_back(construct_asm("push", reg_src1));
                }
                break;
            }
            case POSTYPE::MEM: {
                int mem_src1 = SymbolManager_.avalue_mem(str_src1_encode);
                asmlines.push_back(construct_asm("mov", reg_dst, mem_src1));
                break;
            }
            case POSTYPE::GLOBAL: {
                asmlines.push_back(construct_asm("mov", reg_dst, str_src1));
                break;
            }
            default: {
                std::cout << "common default error" << std::endl;
                break;
            }
        }
    }


    if (TACLine_.src2.OperType == TACOPERANDTYPE::IMM) {
        asmlines.push_back(construct_asm("oopp", reg_dst, str_src2));
    } else if (TACLine_.src2.OperType == TACOPERANDTYPE::VAR) {
        std::string str_src2_encode = SymbolManager_.encode_var(str_src2);
        
        POSTYPE pos = SymbolManager_.position(str_src2_encode);
        switch (pos) {
            case POSTYPE::REG: {
                REG reg_src2 = SymbolManager_.avalue_reg(str_src2_encode);
                asmlines.push_back(construct_asm("oopp", reg_dst, reg_src2));
                break;
            }
            case POSTYPE::MEM: {
                int mem_src2 = SymbolManager_.avalue_mem(str_src2_encode);
                asmlines.push_back(construct_asm("oopp", reg_dst, mem_src2));
                break;
            }
            case POSTYPE::GLOBAL: {
                asmlines.push_back(construct_asm("oopp", reg_dst, str_src2));
                break;
            }
            default: {
                std::cout << "common default error" << std::endl;
                break;
            }
        }
    }
    SymbolManager_.set_avalue_reg(str_dst_encode, reg_dst);
    if (SymbolManager_.avalue_mem(str_dst_encode) != -1) {
        int dst_mem = SymbolManager_.avalue_mem(str_dst_encode);
        asmlines.push_back(construct_asm("mov", dst_mem, reg_dst));
    }
    return asmlines;
}
