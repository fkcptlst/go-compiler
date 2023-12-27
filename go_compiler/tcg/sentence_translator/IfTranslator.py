from ...common.REG import REG
from ...common.tac import TACOPERANDTYPE, TACLine
from ...common.tac import tacopmap
from ...tcg.SymbolManager import POSTYPE, SymbolManager
from ...tcg.asm import ASMLines
from ...tcg.construct_asm import construct_asm
from ...tcg.sentence_translator.BaseTranslator import BaseTranslator
import logging


class IfTranslator(BaseTranslator):
    def __init__(self) -> None:
        super().__init__()

    def SentenceTranslate_(
        self, SymbolManager_: SymbolManager, TACLine_: TACLine
    ) -> ASMLines:
        asmlines: ASMLines = ASMLines()
        str_op = tacopmap[TACLine_.op]
        str_label = TACLine_.dst.value

        str_src1 = TACLine_.src1.value
        str_src2 = TACLine_.src2.value

        reg_src1 = REG.NONE

        if TACLine_.src1.OperType == TACOPERANDTYPE.IMM:
            reg_src1 = REG.EDI
            asmlines.append(construct_asm("mov", reg_src1, str_src1))
        else:
            str_src1_encode = SymbolManager_.encode_var(str_src1)
            match SymbolManager_.position(str_src1_encode):
                case POSTYPE.REG:
                    reg_src1 = SymbolManager_.avalue_reg(str_src1_encode)
                case POSTYPE.MEM:
                    mem_src1 = SymbolManager_.avalue_mem(str_src1_encode)
                    reg_src1 = REG.EDI
                    asmlines.append(construct_asm("mov", reg_src1, mem_src1))
                case POSTYPE.GLOBAL:
                    reg_src1 = REG.EDI
                    asmlines.append(construct_asm("mov", reg_src1, str_src1))
                case _:
                    logging.error("if src1 default error")

        if TACLine_.src2.OperType == TACOPERANDTYPE.IMM:
            asmlines.append(construct_asm("cmp", reg_src1, str_src2))
        elif TACLine_.src2.OperType == TACOPERANDTYPE.VAR:
            str_src2_encode = SymbolManager_.encode_var(str_src2)
            pos = SymbolManager_.position(str_src2_encode)
            match pos:
                case POSTYPE.REG:
                    reg_src2 = SymbolManager_.avalue_reg(str_src2_encode)
                    asmlines.append(construct_asm("cmp", reg_src1, reg_src2))
                case POSTYPE.MEM:
                    mem_src2 = SymbolManager_.avalue_mem(str_src2_encode)
                    asmlines.append(construct_asm("cmp", reg_src1, mem_src2))
                case POSTYPE.GLOBAL:
                    asmlines.append(construct_asm("cmp", reg_src1, str_src2))
                case _:
                    logging.error("if src2 default error")
        asmlines.append(construct_asm(str_op, str_label))
        return asmlines
