from ...common.REG import REG
from ...common.tac import TACLine
from ...tcg.asm import ASMLines
from ...tcg.SymbolManager import SymbolManager
from ...tcg.construct_asm import construct_asm
from ...tcg.sentence_translator.BaseTranslator import BaseTranslator


class RetTranslator(BaseTranslator):
    def __init__(self) -> None:
        super().__init__()

    def SentenceTranslate_(
        self, SymbolManager_: SymbolManager, TACLine_: TACLine
    ) -> ASMLines:
        asmlines: ASMLines = ASMLines()
        str_src1 = SymbolManager_.encode_var(TACLine_.src1.value)
        SymbolManager_.set_esp_bias(4)
        SymbolManager_.set_avalue_mem(str_src1, SymbolManager_.get_esp() - 4)
        asmlines.append(construct_asm("sub", REG.ESP, "4"))
        return asmlines
