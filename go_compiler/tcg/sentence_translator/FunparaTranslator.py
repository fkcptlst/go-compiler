from typing_extensions import override
from go_compiler.common.REG import REG
from go_compiler.common.tac import TACLine
from go_compiler.tcg.asm import ASMLines
from go_compiler.tcg.construct_asm import construct_asm
from go_compiler.tcg.sentence_translator.BaseTranslator import BaseTranslator
from ...tcg.SymbolManager import RelacedEeg, SymbolManager


class FunparaTranslator(BaseTranslator):
    def __init__(self) -> None:
        super().__init__()

    @override
    def SentenceTranslate_(
        self, SymbolManager_: SymbolManager, TACLine_: TACLine
    ) -> ASMLines:
        asmlines: ASMLines = ASMLines()
        par_encode = SymbolManager_.encode_var(TACLine_.src1.value)
        para_mem = SymbolManager_.para()
        dst_reg = SymbolManager_.get_reg(par_encode, "")

        if dst_reg == REG.NONE:
            replaced_reg: RelacedEeg = SymbolManager_.get_replaced_reg()
            dst_reg = replaced_reg.reg
            SymbolManager_.push_reg(dst_reg)
            asmlines.append(construct_asm("push", dst_reg))

        asmlines.append(construct_asm("mov", dst_reg, para_mem))
        SymbolManager_.set_avalue_reg(par_encode, dst_reg)
        if SymbolManager_.avalue_mem(par_encode) != -1:
            dst_mem = SymbolManager_.avalue_mem(par_encode)
            asmlines.append(construct_asm("mov", dst_mem, dst_reg))

        return asmlines
