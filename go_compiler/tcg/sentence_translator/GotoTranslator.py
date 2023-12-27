from ...common.tac import TACLine
from ...tcg.SymbolManager import SymbolManager
from ...tcg.asm import ASMLines
from ...tcg.construct_asm import construct_asm
from ...tcg.sentence_translator.BaseTranslator import BaseTranslator


class GotoTranslator(BaseTranslator):
    def __init__(self) -> None:
        super().__init__()

    def SentenceTranslate_(
        self, SymbolManager_: SymbolManager, TACLine_: TACLine
    ) -> ASMLines:
        asmlines: ASMLines = ASMLines()
        str_label = TACLine_.src1.value
        asmlines.append(construct_asm("jmp", str_label))
        return asmlines
