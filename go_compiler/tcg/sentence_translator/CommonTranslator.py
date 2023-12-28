from loguru import logger
from ...common.REG import REG
from ...common.tac import TACOP, TACOPERANDTYPE, TACLine
from ...tcg.SymbolManager import POSTYPE, SymbolManager
from ...tcg.asm import ASMLines
from ...tcg.construct_asm import construct_asm
from ...tcg.sentence_translator.BaseTranslator import BaseTranslator


class CommonTranslator(BaseTranslator):
    def __init__(self) -> None:
        super().__init__()

    def commonop2string(self, op: TACOP) -> str:
        if op == TACOP.ADD:
            return "add"
        if op == TACOP.SUB:
            return "sub"
        if op == TACOP.MUL:
            return "mul"
        if op == TACOP.DIV:
            return "div"
        return "oopp"

    def SentenceTranslate_(
        self, SymbolManager_: SymbolManager, TACLine_: TACLine
    ) -> ASMLines:
        asmlines: ASMLines = ASMLines()
        str_dst_encode = SymbolManager_.encode_var(TACLine_.dst.value)
        str_src1 = TACLine_.src1.value
        str_src2 = TACLine_.src2.value
        op_string = self.commonop2string(TACLine_.op)

        reg_dst = REG.NONE

        if TACLine_.src1.OperType == TACOPERANDTYPE.IMM:
            reg_dst = SymbolManager_.get_reg(str_dst_encode, "")
            if reg_dst == REG.NONE:
                replaced_reg = SymbolManager_.get_replaced_reg()
                reg_dst = replaced_reg.reg
                SymbolManager_.push_reg(reg_dst)
                asmlines.append(construct_asm("push", reg_dst))

            asmlines.append(construct_asm("mov", reg_dst, str_src1))
        else:
            str_src1_encode = SymbolManager_.encode_var(str_src1)
            reg_dst = SymbolManager_.get_reg(str_dst_encode, str_src1_encode)
            if reg_dst == REG.NONE:
                replaced_reg = SymbolManager_.get_replaced_reg()
                reg_dst = replaced_reg.reg
                SymbolManager_.push_reg(reg_dst)
                asmlines.append(construct_asm("push", reg_dst))
            pos = SymbolManager_.position(str_src1_encode)
            if pos == POSTYPE.REG:
                reg_src1 = SymbolManager_.avalue_reg(str_src1_encode)
                if reg_dst != reg_src1:
                    asmlines.append(construct_asm("mov", reg_dst, reg_src1))
                else:
                    SymbolManager_.push_reg(reg_src1)
                    asmlines.append(construct_asm("push", reg_src1))
            elif pos == POSTYPE.MEM:
                mem_src1 = SymbolManager_.avalue_mem(str_src1_encode)
                asmlines.append(construct_asm("mov", reg_dst, mem_src1))
            elif pos == POSTYPE.GLOBAL:
                asmlines.append(construct_asm("mov", reg_dst, str_src1))
            else:
                logger.error("common default error")

        if TACLine_.src2.OperType == TACOPERANDTYPE.IMM:
            asmlines.append(construct_asm(op_string, reg_dst, str_src2))
        elif TACLine_.src2.OperType == TACOPERANDTYPE.VAR:
            str_src2_encode = SymbolManager_.encode_var(str_src2)
            pos = SymbolManager_.position(str_src2_encode)
            if pos == POSTYPE.REG:
                reg_src2 = SymbolManager_.avalue_reg(str_src2_encode)
                asmlines.append(construct_asm(op_string, reg_dst, reg_src2))
            elif pos == POSTYPE.MEM:
                mem_src2 = SymbolManager_.avalue_mem(str_src2_encode)
                asmlines.append(construct_asm(op_string, reg_dst, mem_src2))
            elif pos == POSTYPE.GLOBAL:
                asmlines.append(construct_asm(op_string, reg_dst, str_src2))
            else:
                logger.error("Common Default Error")

        if reg_dst != REG.EDI:
            SymbolManager_.set_avalue_reg(str_dst_encode, reg_dst)

        if SymbolManager_.avalue_mem(str_dst_encode) != -1:
            dst_mem = SymbolManager_.avalue_mem(str_dst_encode)
            asmlines.append(construct_asm("mov", dst_mem, reg_dst))

        return asmlines
