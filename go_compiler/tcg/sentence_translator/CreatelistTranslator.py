from ...common.REG import REG
from ...common.tac import TACLine
from ...tcg.asm import ASMLines
from ...tcg.construct_asm import construct_asm
from ...tcg.sentence_translator.BaseTranslator import BaseTranslator
from ...tcg.SymbolManager import SymbolManager


class CreatelistTranslator(BaseTranslator):
    def __init__(self) -> None:
        super().__init__()

    def SentenceTranslate_(
        self, SymbolManager_: SymbolManager, TACLine_: TACLine
    ) -> ASMLines:
        asmlines: ASMLines = ASMLines()
        array_str_encode = SymbolManager_.encode_var(TACLine_.src1.value)
        array_space = 4 * int(TACLine_.src2.value)
        SymbolManager_.set_esp_bias(4)
        asmlines.append(construct_asm("sub", REG.ESP, "4"))
        array_ptr_mem = SymbolManager_.get_esp() - 4
        SymbolManager_.set_avalue_mem(array_str_encode, array_ptr_mem)
        SymbolManager_.set_esp_bias(array_space)
        asmlines.append(construct_asm("sub", REG.ESP, str(array_space)))
        asmlines.append(construct_asm("mov", array_ptr_mem, REG.ESP))

        return asmlines
