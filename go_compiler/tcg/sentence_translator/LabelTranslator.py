from ...tcg.SymbolManager import SymbolManager
from ...tcg.asm import ASMLines
from ...tcg.construct_asm import construct_asm
from ...tcg.sentence_translator.BaseTranslator import BaseTranslator
from ...common.tac import TACLine


class LabelTranslator(BaseTranslator):
    def __init__(self) -> None:
        super().__init__()

    def SentenceTranslate_(
        self, SymbolManager_: SymbolManager, TACLine_: TACLine
    ) -> ASMLines:
        asmlines: ASMLines = ASMLines()

        label_str = TACLine_.src1.value
        asmlines.append(construct_asm(label_str + ":"))
        return asmlines
