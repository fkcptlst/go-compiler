#include "Common/Common.h"
#include "TCG/SentenceTranslator/AssignTranslator.h"
#include "TCG/ASM.h"
#include "TCG/SymbolManager.h"
#include "TCG/ConstructASM.h"

ASMLines AssignTranslator::SentenceTranslate_(SymbolManager& SymbolManager_, TACLine& TACLine_) {
    ASMLines asmlines;
    std::string str_dst_encode = SymbolManager_.encode_var(TACLine_.dst.value);
    POSTYPE pos_dst = SymbolManager_.position(str_dst_encode);

    RelacedEeg replaced_reg;
    REG dst_reg = SymbolManager_.get_reg(str_dst_encode, "");
    if (dst_reg == REG::None) {
        replaced_reg = SymbolManager_.get_replaced_reg();
        dst_reg = replaced_reg.reg;
    }

    // 如果需要进行备份原来寄存器
    if (!replaced_reg.no_use) {
        if (replaced_reg.mem == -1) {
            // 如果该变量内存中没有位置，push备份
            SymbolManager_.push_reg(dst_reg);
            asmlines.push_back(construct_asm("push", dst_reg));
        } else {
            // 如果该变量内存中有位置，直接更新该变量在内存中的值
            asmlines.push_back(construct_asm("mov", replaced_reg.mem, replaced_reg.reg));
        }
    }

    // 开始翻译赋值语句 (ASSIGN dst src1)
    std::string str_src1 = TACLine_.src1.value;
    if (TACLine_.src1.OperType == TACOPERANDTYPE::IMM) {
        // 如果 src1 是 立即数
        asmlines.push_back(construct_asm("mov", dst_reg, str_src1));
    } else if (TACLine_.src1.OperType == TACOPERANDTYPE::VAR) {
        // 如果 src1 是 变量，可能会是 寄存器、栈、全局变量
        std::string encode_str_src1 = SymbolManager_.encode_var(str_src1);
        POSTYPE pos = SymbolManager_.position(encode_str_src1);
        switch (pos) {
            case POSTYPE::REG: {
                REG src_reg = SymbolManager_.avalue_reg(encode_str_src1);
                asmlines.push_back(construct_asm("mov", dst_reg, src_reg));
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
                LOG(ERROR) << "assign sentence: str1's pos wrong";
                break;
            }
        }
    }

    // TODO: 似乎不需要 如果 dst 也在内存中，更新其内存中的位置
    int dst_mem = SymbolManager_.avalue_mem(str_dst_encode);
    if (dst_mem != -1) {
        asmlines.push_back(construct_asm("mov", dst_mem, dst_reg));
    }

    // 更新 SymbolManager 中变量信息
    SymbolManager_.set_avalue_reg(str_dst_encode, dst_reg);
    return asmlines;
}
