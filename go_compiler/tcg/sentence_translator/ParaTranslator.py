from ...common.REG import REG
from ...common.tac import TACLine
from ...tcg.asm import ASMLines
from ...common.tac import TACOPERANDTYPE
from ...tcg.SymbolManager import POSTYPE, SymbolManager
from ...tcg.construct_asm import construct_asm
from ...tcg.sentence_translator.BaseTranslator import BaseTranslator
from ...logger.logger import logger


class ParaTranslator(BaseTranslator):
    def __init__(self) -> None:
        super().__init__()

    def SentenceTranslate_(
        self, SymbolManager_: SymbolManager, TACLine_: TACLine
    ) -> ASMLines:
        asmlines: ASMLines = ASMLines()

        if TACLine_.src1.OperType == TACOPERANDTYPE.IMM:
            src = TACLine_.src1.value
            SymbolManager_.set_esp_bias(4)
            asmlines.append(construct_asm("push", src))
        elif TACLine_.src1.OperType == TACOPERANDTYPE.VAR:
            str_src1 = SymbolManager_.encode_var(TACLine_.src1.value)
            pos = SymbolManager_.position(str_src1)

            match pos:
                case POSTYPE.REG:
                    reg_src1 = SymbolManager_.avalue_reg(str_src1)
                    SymbolManager_.push_reg(reg_src1, 0)
                    asmlines.append(construct_asm("push", reg_src1))

                case POSTYPE.MEM:
                    mem_src1 = SymbolManager_.avalue_mem(str_src1)
                    asmlines.append(construct_asm("mov", REG.EDI, mem_src1))
                    SymbolManager_.push_reg(REG.EDI, 0)
                    asmlines.append(construct_asm("push", REG.EDI))

                case POSTYPE.GLOBAL:
                    src = TACLine_.src1.value
                    asmlines.append(construct_asm("mov", REG.EDI, src))
                    SymbolManager_.push_reg(REG.EDI, 0)
                    asmlines.append(construct_asm("push", REG.EDI))

                case _:
                    logger.error("para case _ error")

        return asmlines
