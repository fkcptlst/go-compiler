from typing_extensions import override
from go_compiler.tcg.SymbolManager import SymbolManager
from go_compiler.tcg.asm import ASMLines
from go_compiler.tcg.construct_asm import construct_asm
from go_compiler.tcg.sentence_translator.BaseTranslator import BaseTranslator
from go_compiler.common.tac import TACLine


class LabelTranslator(BaseTranslator):
    def __init__(self) -> None:
        super().__init__()

    @override
    def SentenceTranslate_(
        self, SymbolManager_: SymbolManager, TACLine_: TACLine
    ) -> ASMLines:
        asmlines: ASMLines = ASMLines()

        label_str = TACLine_.src1.value
        asmlines.append(construct_asm(label_str + ":"))
        return asmlines
