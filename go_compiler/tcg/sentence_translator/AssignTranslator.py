from loguru import logger

from ...common.REG import REG, to_string
from ...common.tac import TACOPERANDTYPE, TACLine
from ...tcg.SymbolManager import POSTYPE, RelacedEeg, SymbolManager
from ...tcg.asm import ASMLines
from ...tcg.construct_asm import construct_asm, construct_asm_mem
from ...tcg.sentence_translator.BaseTranslator import BaseTranslator


class AssignTranslator(BaseTranslator):
    def __init__(self) -> None:
        super().__init__()

    def SentenceTranslate_(
        self, SymbolManager_: SymbolManager, TACLine_: TACLine
    ) -> ASMLines:
        logger.debug(f"{SymbolManager_}: {TACLine_}")

        asmlines: ASMLines = ASMLines()
        str_dst_encode = SymbolManager_.encode_var(TACLine_.dst.value)
        # pos_dst = SymbolManager_.position(str_dst_encode)

        replaced_reg: RelacedEeg = RelacedEeg()
        dst_reg: REG = SymbolManager_.get_reg(str_dst_encode, "")
        if dst_reg == REG.NONE:
            replaced_reg = SymbolManager_.get_replaced_reg()
            dst_reg = replaced_reg.reg

        logger.debug(f"replaced_reg:{replaced_reg}")
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
                # 如果 src1 是 立即数
                asmlines.append(construct_asm("mov", dst_reg, str_src1))
            elif TACLine_.src1.OperType == TACOPERANDTYPE.VAR:
                # 如果 src1 是 变量，可能会是 寄存器、栈、全局变量
                encode_str_src1 = SymbolManager_.encode_var(str_src1)
                pos = SymbolManager_.position(encode_str_src1)
                match pos:
                    case POSTYPE.REG:
                        src_reg = SymbolManager_.avalue_reg(encode_str_src1)
                        asmlines.append(construct_asm("mov", dst_reg, src_reg))
                    case POSTYPE.MEM:
                        src_mem = SymbolManager_.avalue_mem(encode_str_src1)
                        asmlines.append(construct_asm("mov", dst_reg, src_mem))
                    case POSTYPE.GLOBAL:
                        asmlines.append(construct_asm("mov", dst_reg, str_src1))
                    case _:
                        # 如果是 临时变量 Tx 类型则无所谓
                        logger.warning(
                            f"assign sentence: str1's pos wrong src1:{TACLine_.src1} (If Temp, ignore)"
                        )

            elif TACLine_.src1.OperType == TACOPERANDTYPE.PTR:
                # 由于 TAC 对 operator[] 的处理，暂时可以认为这时指针一定在寄存器中
                encode_str_src1 = SymbolManager_.encode_var(str_src1)
                src_reg = SymbolManager_.avalue_reg(encode_str_src1)
                asmlines.append(
                    construct_asm(
                        "mov", to_string(dst_reg), construct_asm_mem(src_reg, 0)
                    )
                )
        elif TACLine_.dst.OperType == TACOPERANDTYPE.PTR:
            str_src1 = TACLine_.src1.value
            if TACLine_.src1.OperType == TACOPERANDTYPE.IMM:
                # 如果 src1 是 立即数
                asmlines.append(construct_asm("mov", dst_reg, 0, str_src1))
            elif TACLine_.src1.OperType == TACOPERANDTYPE.VAR:
                # 如果 src1 是 变量，可能会是 寄存器、栈、全局变量
                encode_str_src1 = SymbolManager_.encode_var(str_src1)
                pos = SymbolManager_.position(encode_str_src1)
                match pos:
                    case POSTYPE.REG:
                        src_reg = SymbolManager_.avalue_reg(encode_str_src1)
                        asmlines.append(construct_asm("mov", dst_reg, 0, src_reg))
                    case POSTYPE.MEM:
                        src_mem = SymbolManager_.avalue_mem(encode_str_src1)
                        asmlines.append(construct_asm("mov", dst_reg, 0, src_mem))
                    case POSTYPE.GLOBAL:
                        asmlines.append(construct_asm("mov", dst_reg, 0, str_src1))
                    case _:
                        logger.error("assign sentence: str1's pos wrong")

            elif TACLine_.src1.OperType == TACOPERANDTYPE.PTR:
                encode_str_src1 = SymbolManager_.encode_var(str_src1)
                src_reg = SymbolManager_.avalue_reg(encode_str_src1)
                asmlines.append(
                    construct_asm(
                        "mov",
                        construct_asm_mem(dst_reg, 0, "dword"),
                        construct_asm_mem(src_reg, 0),
                    )
                )
        else:
            logger.error("assign sentence: dst' TACOPERANDTYPE wrong")

        # TODO: 似乎不需要 如果 dst 也在内存中，更新其内存中的位置
        dst_mem = SymbolManager_.avalue_mem(str_dst_encode)
        if dst_mem != -1:
            asmlines.append(construct_asm("mov", dst_mem, dst_reg))
        SymbolManager_.set_avalue_reg(str_dst_encode, dst_reg)

        logger.debug(f"asmlines {asmlines}")

        return asmlines
