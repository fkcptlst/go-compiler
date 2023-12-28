from loguru import logger
from ...common.REG import REG
from ...common.tac import TACOPERANDTYPE, TACLine
from ...tcg.construct_asm import construct_asm
from ...tcg.SymbolManager import POSTYPE, SymbolManager
from ...tcg.asm import ASMLines
from ...tcg.sentence_translator.BaseTranslator import BaseTranslator


class FunretTranslator(BaseTranslator):
    def __init__(self) -> None:
        super().__init__()

    def SentenceTranslate_(
        self, SymbolManager_: SymbolManager, TACLine_: TACLine
    ) -> ASMLines:
        asmlines: ASMLines = ASMLines()
        ret_src = TACLine_.src1.value
        ret_mem = SymbolManager_.ret()

        if TACLine_.src1.OperType == TACOPERANDTYPE.IMM:
            asmlines.append(construct_asm("mov", ret_mem, ret_src))
        elif TACLine_.src1.OperType == TACOPERANDTYPE.VAR:
            ret_src_encode = SymbolManager_.encode_var(ret_src)

            match SymbolManager_.position(ret_src_encode):
                case POSTYPE.REG:
                    ret_src_reg = SymbolManager_.avalue_reg(ret_src_encode)
                    asmlines.append(construct_asm("mov", ret_mem, ret_src_reg))
                case POSTYPE.MEM:
                    ret_src_mem = SymbolManager_.avalue_mem(ret_src_encode)
                    asmlines.append(construct_asm("mov", REG.EDI, ret_src_mem))
                    asmlines.append(construct_asm("mov", ret_mem, REG.EDI))
                case POSTYPE.GLOBAL:
                    asmlines.append(construct_asm("mov", ret_mem, ret_src))
                case _:
                    logger.error("funret default error")

        return asmlines
