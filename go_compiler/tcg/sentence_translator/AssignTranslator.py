from typing_extensions import override
from go_compiler.common.REG import REG, to_string
from go_compiler.common.tac import TACOPERANDTYPE, TACLine
from go_compiler.tcg.SymbolManager import POSTYPE, RelacedEeg, SymbolManager
from go_compiler.tcg.asm import ASMLines
from go_compiler.tcg.construct_asm import construct_asm, construct_asm_mem
from go_compiler.tcg.sentence_translator.BaseTranslator import BaseTranslator
import logging


class AssignTranslator(BaseTranslator):
    def __init__(self) -> None:
        super().__init__()

    @override
    def SentenceTranslate_(
        self, SymbolManager_: SymbolManager, TACLine_: TACLine
    ) -> ASMLines:
        asmlines: ASMLines = ASMLines()
        str_dst_encode = SymbolManager_.encode_var(TACLine_.dst.value)
        # post_dst = SymbolManager_.position(str_dst_encode)
        replaced_reg: RelacedEeg = RelacedEeg()
        dst_reg: REG = SymbolManager_.get_reg(str_dst_encode, "")
        if dst_reg == REG.NONE:
            replaced_reg = SymbolManager_.get_replaced_reg()
            dst_reg = replaced_reg.reg

        if not replaced_reg.no_use:
            if replaced_reg.mem == -1:
                # 如果该变量内存中没有位置，push备份
                SymbolManager_.push_reg(dst_reg)
                asmlines.append(construct_asm("push", dst_reg))
            else:
                asmlines.append(
                    construct_asm("mov", replaced_reg.mem, replaced_reg.reg)
                )

        # 开始翻译赋值语句 (ASSIGN dst src1)
        if TACLine_.dst.OperType == TACOPERANDTYPE.VAR:
            str_src1 = TACLine_.src1.value
            if TACLine_.src1.OperType == TACOPERANDTYPE.IMM:
                asmlines.append(construct_asm("mov", dst_reg, str_src1))
            elif TACLine_.src1.OperType == TACOPERANDTYPE.VAR:
                encode_str_src1 = SymbolManager_.encode_var(str_src1)
                pos: POSTYPE = SymbolManager_.position(encode_str_src1)
                if pos == POSTYPE.REG:
                    src_reg: REG = SymbolManager_.avalue_reg(encode_str_src1)
                    asmlines.append(construct_asm("mov", dst_reg, src_reg))
                elif pos == POSTYPE.MEM:
                    src_mem = SymbolManager_.avalue_mem(encode_str_src1)
                    asmlines.append(construct_asm("mov", dst_reg, src_mem))
                elif pos == POSTYPE.GLOBAL:
                    asmlines.append(construct_asm("mov", dst_reg, str_src1))
                else:
                    logging.error("assign sentence: str1's pos wrong")
            elif TACLine_.src1.OperType == TACOPERANDTYPE.PTR:
                encode_str_src1 = SymbolManager_.encode_var(str_src1)
                src_reg = SymbolManager_.avalue_reg(encode_str_src1)
                asmlines.append(
                    construct_asm(
                        "mov", to_string(dst_reg), construct_asm_mem(src_reg, 0)
                    )
                )
        else:
            logging.error("assign sentence: dst' TACOPERANDTYPE wrong")

        # TODO: 似乎不需要 如果 dst 也在内存中，更新其内存中的位置
        dst_mem = SymbolManager_.avalue_mem(str_dst_encode)
        if dst_mem != -1:
            asmlines.append(construct_asm("mov", dst_mem, dst_reg))
        SymbolManager_.set_avalue_reg(str_dst_encode, dst_reg)
        return asmlines
