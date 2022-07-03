#include "Common/Common.h"
#include "TCG/SentenceTranslator/MulTranslator.h"
#include "TCG/ASM.h"
#include "TCG/SymbolManager.h"
#include "TCG/ConstructASM.h"


/**
 * TAC: dst = src1 mul src2
 * ASM: EAX = EAX * xxx (PS: xxx 不可为 imm)
 * 需要将 EAX 做好备份，然后选择一个寄存器(或mem) 进行乘法运算，并且如果 src1 之后还要用，也需要对 src1 做好备份
 */
ASMLines MulTranslator::SentenceTranslate_(SymbolManager& SymbolManager_, TACLine& TACLine_) {
    ASMLines asmlines;
    std::string str_dst(TACLine_.dst.value), str_src1(TACLine_.src1.value), str_src2(TACLine_.src2.value);

    // 备份 EAX
    std::string eax_val_pre = SymbolManager_.rvalue(REG::EAX);
    int eax_val_pre_mem;
    if (!SymbolManager_.use_info(eax_val_pre, true).no_use()) {
        eax_val_pre_mem = SymbolManager_.avalue_mem(eax_val_pre);
        if (eax_val_pre_mem == -1) {
            // 内存中没有位置, push 保存
            SymbolManager_.push_reg(REG::EAX);
            asmlines.push_back(construct_asm("push", REG::EAX));
        } else {
            // 内存中有位置, 更新内存中的值
            asmlines.push_back(construct_asm("mov", eax_val_pre_mem, REG::EAX));
        }
    }

    // EAX = src1 类似 assign
    if (TACLine_.src1.OperType == TACOPERANDTYPE::IMM) {
        // 如果 src1 是 立即数
        asmlines.push_back(construct_asm("mov", REG::EAX, str_src1));
    } else if (TACLine_.src1.OperType == TACOPERANDTYPE::VAR) {
        // 如果 src1 是 变量，可能会是 寄存器、栈、全局变量
        std::string encode_str_src1 = SymbolManager_.encode_var(str_src1);
        POSTYPE pos = SymbolManager_.position(encode_str_src1);
        switch (pos) {
            case POSTYPE::REG: {
                REG src_reg = SymbolManager_.avalue_reg(encode_str_src1);
                asmlines.push_back(construct_asm("mov", REG::EAX, src_reg));
                break;
            }
            case POSTYPE::MEM: {
                int src_mem = SymbolManager_.avalue_mem(encode_str_src1);
                asmlines.push_back(construct_asm("mov", REG::EAX, src_mem));
                break;
            }
            case POSTYPE::GLOBAL: {
                asmlines.push_back(construct_asm("mov", REG::EAX, str_src1));
                break;
            }
            default: {
                LOG(ERROR) << "mul sentence: str1's pos wrong";
                break;
            }
        }
    }
    SymbolManager_.set_avalue_reg(SymbolManager_.encode_var(str_src1), REG::EAX);

    // 找一个寄存器给 src2，给这个寄存器做备份 TODO: 直接用 EBX
    REG reg_src2 = REG::EBX;
    std::string reg_src2_pre = SymbolManager_.rvalue(reg_src2);
    int mem_src2_pre;
    if (!SymbolManager_.use_info(reg_src2_pre, true).no_use()) {
        mem_src2_pre = SymbolManager_.avalue_mem(reg_src2_pre);
        if (mem_src2_pre == -1) {
            // 内存中没有位置, push 保存
            SymbolManager_.push_reg(reg_src2);
            asmlines.push_back(construct_asm("push", reg_src2));
        } else {
            // 内存中有位置, 更新内存中的值
            asmlines.push_back(construct_asm("mov", mem_src2_pre, reg_src2));
        }
    }

    // EBX = src2 类似 assign
    if (TACLine_.src2.OperType == TACOPERANDTYPE::IMM) {
        // 如果 src2 是 立即数
        asmlines.push_back(construct_asm("mov", reg_src2, str_src2));
    } else if (TACLine_.src2.OperType == TACOPERANDTYPE::VAR) {
        // 如果 src2 是 变量，可能会是 寄存器、栈、全局变量
        std::string encode_str_src2 = SymbolManager_.encode_var(str_src2);
        POSTYPE pos = SymbolManager_.position(encode_str_src2);
        switch (pos) {
            case POSTYPE::REG: {
                REG src_reg = SymbolManager_.avalue_reg(encode_str_src2);
                asmlines.push_back(construct_asm("mov", reg_src2, src_reg));
                break;
            }
            case POSTYPE::MEM: {
                int src_mem = SymbolManager_.avalue_mem(encode_str_src2);
                asmlines.push_back(construct_asm("mov", reg_src2, src_mem));
                break;
            }
            case POSTYPE::GLOBAL: {
                asmlines.push_back(construct_asm("mov", reg_src2, str_src2));
                break;
            }
            default: {
                LOG(ERROR) << "mul sentence: str2's pos wrong";
                break;
            }
        }
    }
    SymbolManager_.set_avalue_reg(SymbolManager_.encode_var(str_src2), reg_src2);

    // 在进行 EAX = EAX * src2 之前, 由于 在 EAX 中的 src1 将被覆盖，所以需要再次对 EAX 做一次备份
    std::string eax_src1 = SymbolManager_.rvalue(REG::EAX);
    int eax_src1_mem;
    if (!SymbolManager_.use_info(eax_src1, true).no_use()) {
        eax_src1_mem = SymbolManager_.avalue_mem(eax_src1);
        if (eax_src1_mem == -1) {
            // 内存中没有位置, push 保存
            SymbolManager_.push_reg(REG::EAX);
            asmlines.push_back(construct_asm("push", REG::EAX));
        } else {
            // 内存中有位置, 更新内存中的值
            asmlines.push_back(construct_asm("mov", eax_src1_mem, REG::EAX));
        }
    }

    // 如果是 div，因为会将 余数存在 EDX，所以需要备份
    std::string edx_symbol = SymbolManager_.rvalue(REG::EDX);
    int edx_mem;
    if (!SymbolManager_.use_info(edx_symbol, true).no_use()) {
        edx_mem = SymbolManager_.avalue_mem(edx_symbol);
        if (edx_mem == -1) {
            // 内存中没有位置, push 保存
            SymbolManager_.push_reg(REG::EDX);
            asmlines.push_back(construct_asm("push", REG::EDX));
        } else {
            // 内存中有位置, 更新内存中的值
            asmlines.push_back(construct_asm("mov", edx_mem, REG::EDX));
        }
    }

    // MUL
    std::string op = to_string(TACLine_.op);
    asmlines.push_back(construct_asm(op, reg_src2));
    SymbolManager_.set_avalue_reg(SymbolManager_.encode_var(str_dst), REG::EAX);

    return asmlines;
}
