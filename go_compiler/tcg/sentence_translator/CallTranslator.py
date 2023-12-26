import logging
from go_compiler.common.REG import REG
from go_compiler.common.tac import TACLine
from go_compiler.tcg.SymbolManager import SymbolManager
from go_compiler.tcg.asm import ASMLines
from go_compiler.tcg.construct_asm import construct_asm
from go_compiler.tcg.sentence_translator.BaseTranslator import BaseTranslator


class CallTranslator(BaseTranslator):
    def __init__(self) -> None:
        super().__init__()

    def SentenceTranslate_(self, SymbolManager_: SymbolManager, TACLine_: TACLine):
        asmlines: ASMLines = ASMLines()
        fun_name = TACLine_.src1.value
        asmlines.append(construct_asm("call", fun_name))
        fun = SymbolManager_.resolve_fun(fun_name)

        if fun is not None:
            para_num = len(fun.fun_para_type_list)
            SymbolManager_.set_esp_bias(-4 * para_num)
            asmlines.append(construct_asm("add", REG.ESP, str(4 * para_num)))
        else:
            logging.error("call func error: ", fun_name)

        return asmlines
