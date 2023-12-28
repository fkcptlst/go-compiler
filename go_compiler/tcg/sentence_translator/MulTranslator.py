from loguru import logger
from ...common.REG import REG
from ...common.tac import to_string
from ...tcg.asm import ASMLines
from ...common.tac import TACOPERANDTYPE, TACLine
from ...tcg.SymbolManager import POSTYPE, SymbolManager
from ...tcg.sentence_translator.BaseTranslator import BaseTranslator
from ...tcg.construct_asm import construct_asm


class MulTranslator(BaseTranslator):
    def __init__(self) -> None:
        super().__init__()

    def SentenceTranslate_(
        self, SymbolManager_: SymbolManager, TACLine_: TACLine
    ) -> ASMLines:
        asmlines: ASMLines = ASMLines()
        str_dst = TACLine_.dst.value
        str_src1 = TACLine_.src1.value
        str_src2 = TACLine_.src2.value

        # 备份 EAX
        eax_val_pre = SymbolManager_.rvalue(REG.EAX)
        eax_val_pre_mem: int
        if not SymbolManager_.use_info(eax_val_pre, True).no_use():
            eax_val_pre_mem = SymbolManager_.avalue_mem(eax_val_pre)
            if eax_val_pre_mem == -1:
                # 内存中没有位置, push 保存
                SymbolManager_.push_reg(REG.EAX)
                asmlines.append(construct_asm("push", REG.EAX))
            else:
                # 内存中有位置, 更新内存中的值
                asmlines.append(construct_asm("mov", eax_val_pre_mem, REG.EAX))

        # EAX = src1 类似 assign
        if TACLine_.src1.OperType == TACOPERANDTYPE.IMM:
            # 如果 src1 是 立即数
            asmlines.append(construct_asm("mov", REG.EAX, str_src1))
        elif TACLine_.src1.OperType == TACOPERANDTYPE.VAR:
            # 如果 src1 是 变量，可能会是 寄存器、栈、全局变量
            encode_str_src1 = SymbolManager_.encode_var(str_src1)
            pos = SymbolManager_.position(encode_str_src1)
            match pos:
                case POSTYPE.REG:
                    src_reg = SymbolManager_.avalue_reg(encode_str_src1)
                    asmlines.append(construct_asm("mov", REG.EAX, src_reg))

                case POSTYPE.MEM:
                    src_mem = SymbolManager_.avalue_mem(encode_str_src1)
                    asmlines.append(construct_asm("mov", REG.EAX, src_mem))

                case POSTYPE.GLOBAL:
                    asmlines.append(construct_asm("mov", REG.EAX, str_src1))
                case _:
                    logger.error("mul sentence: str1's pos wrong")

        SymbolManager_.set_avalue_reg(SymbolManager_.encode_var(str_src1), REG.EAX)

        # 找一个寄存器给 src2，给这个寄存器做备份 TODO: 直接用 EBX
        reg_src2 = REG.EBX
        reg_src2_pre = SymbolManager_.rvalue(reg_src2)
        mem_src2_pre: int
        if not SymbolManager_.use_info(reg_src2_pre, True).no_use():
            mem_src2_pre = SymbolManager_.avalue_mem(reg_src2_pre)
            if mem_src2_pre == -1:
                # 内存中没有位置, push 保存
                SymbolManager_.push_reg(reg_src2)
                asmlines.append(construct_asm("push", reg_src2))
            else:
                # 内存中有位置, 更新内存中的值
                asmlines.append(construct_asm("mov", mem_src2_pre, reg_src2))

        # EBX = src2 类似 assign
        if TACLine_.src2.OperType == TACOPERANDTYPE.IMM:
            # 如果 src2 是 立即数
            asmlines.append(construct_asm("mov", reg_src2, str_src2))
        elif TACLine_.src2.OperType == TACOPERANDTYPE.VAR:
            # 如果 src2 是 变量，可能会是 寄存器、栈、全局变量
            encode_str_src2 = SymbolManager_.encode_var(str_src2)
            pos = SymbolManager_.position(encode_str_src2)
            match pos:
                case POSTYPE.REG:
                    src_reg = SymbolManager_.avalue_reg(encode_str_src2)
                    asmlines.append(construct_asm("mov", reg_src2, src_reg))

                case POSTYPE.MEM:
                    src_mem = SymbolManager_.avalue_mem(encode_str_src2)
                    asmlines.append(construct_asm("mov", reg_src2, src_mem))

                case POSTYPE.GLOBAL:
                    asmlines.append(construct_asm("mov", reg_src2, str_src2))
                case _:
                    logger.error("mul sentence: str2's pos wrong")

        SymbolManager_.set_avalue_reg(SymbolManager_.encode_var(str_src2), reg_src2)

        # 在进行 EAX = EAX * src2 之前, 由于 在 EAX 中的 src1 将被覆盖，所以需要再次对 EAX 做一次备份
        eax_src1 = SymbolManager_.rvalue(REG.EAX)
        eax_src1_mem: int
        if not SymbolManager_.use_info(eax_src1, True).no_use():
            eax_src1_mem = SymbolManager_.avalue_mem(eax_src1)
            if eax_src1_mem == -1:
                # 内存中没有位置, push 保存
                SymbolManager_.push_reg(REG.EAX)
                asmlines.append(construct_asm("push", REG.EAX))
            else:
                # 内存中有位置, 更新内存中的值
                asmlines.append(construct_asm("mov", eax_src1_mem, REG.EAX))

        # 如果是 div，因为会将 余数存在 EDX，所以需要备份
        edx_symbol = SymbolManager_.rvalue(REG.EDX)
        edx_mem: int
        if not SymbolManager_.use_info(edx_symbol, True).no_use():
            edx_mem = SymbolManager_.avalue_mem(edx_symbol)
            if edx_mem == -1:
                # 内存中没有位置, push 保存
                SymbolManager_.push_reg(REG.EDX)
                asmlines.append(construct_asm("push", REG.EDX))
            else:
                # 内存中有位置, 更新内存中的值
                asmlines.append(construct_asm("mov", edx_mem, REG.EDX))

        # MUL
        op = to_string(TACLine_.op)
        asmlines.append(construct_asm(op, reg_src2))
        SymbolManager_.set_avalue_reg(SymbolManager_.encode_var(str_dst), REG.EAX)

        return asmlines
