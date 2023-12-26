from typing_extensions import override
from go_compiler.common.tac import TACLine
from go_compiler.tcg.SymbolManager import SymbolManager
from go_compiler.tcg.asm import ASMLines
from go_compiler.tcg.construct_asm import construct_asm
from go_compiler.tcg.sentence_translator.BaseTranslator import BaseTranslator


class GotoTranslator(BaseTranslator):
    def __init__(self) -> None:
        super().__init__()

    @override
    def SentenceTranslate_(
        self, SymbolManager_: SymbolManager, TACLine_: TACLine
    ) -> ASMLines:
        asmlines: ASMLines = ASMLines()
        str_label = TACLine_.src1.value
        asmlines.append(construct_asm("jmp", str_label))
        return asmlines
