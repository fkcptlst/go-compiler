from construct_asm import *
from go_compiler.tcg.asm import ASMBlock
from go_compiler.tcg.sentence_translator.CallTranslator import CallTranslator
from go_compiler.tcg.sentence_translator.CommonTranslator import CommonTranslator
from go_compiler.tcg.sentence_translator.CreatelistTranslator import (
    CreatelistTranslator,
)
from go_compiler.tcg.sentence_translator.AssignTranslator import AssignTranslator
from go_compiler.tcg.sentence_translator.FunparaTranslator import FunparaTranslator
from go_compiler.tcg.sentence_translator.FunretTranslator import FunretTranslator
from go_compiler.tcg.sentence_translator.GotoTranslator import GotoTranslator
from go_compiler.tcg.sentence_translator.IfTranslator import IfTranslator
from go_compiler.tcg.sentence_translator.LabelTranslator import LabelTranslator
from go_compiler.tcg.sentence_translator.MulTranslator import MulTranslator
from go_compiler.tcg.sentence_translator.ParaTranslator import ParaTranslator
from go_compiler.tcg.sentence_translator.RetTranslator import RetTranslator
from SymbolManager import *


class BlockTranslator:
    @staticmethod
    def BlockTranslate(SymbolManager_: SymbolManager, TACBlock_: TACBlock) -> ASMBlock:
        ASMBlock_: ASMBlock = ASMBlock("")
        SymbolManager_.push_reg(REG.EBP, 0)
        ASMBlock_.asmlines.append(construct_asm(op="push", src=REG.EBP))
        ASMBlock_.asmlines.append(construct_asm(op="mov", dst=REG.EBP, src=REG.ESP))

        if SymbolManager_.get_name() == "main":
            ASMBlock_.name = "_start"
        else:
            ASMBlock_.name = SymbolManager_.get_name()
            SymbolManager_.push_reg(REG.EAX, 0)
            SymbolManager_.push_reg(REG.EBX, 0)
            SymbolManager_.push_reg(REG.ECX, 0)
            SymbolManager_.push_reg(REG.EDX, 0)
            SymbolManager_.push_reg(REG.ESI, 0)
            ASMBlock_.asmlines.append(construct_asm(op="push", src=REG.EAX))
            ASMBlock_.asmlines.append(construct_asm(op="push", src=REG.EBX))
            ASMBlock_.asmlines.append(construct_asm(op="push", src=REG.ECX))
            ASMBlock_.asmlines.append(construct_asm(op="push", src=REG.EDX))
            ASMBlock_.asmlines.append(construct_asm(op="push", src=REG.ESI))

        SymbolManager_.cal_use_info(TACBlock_)
        SymbolManager_.set_zero_len()
        # todo 完成对每个语句的翻译
        for i in range(len(TACBlock_)):
            SymbolManager_.set_scope(TACBlock_[i].scope)
            if TACBlock_[i].src1.OperType == TACOPERANDTYPE.VAR:
                SymbolManager_.set_use_info(
                    TACBlock_[i].src1.value, TACBlock_[i].src1.use_info
                )
            if TACBlock_[i].src2.OperType == TACOPERANDTYPE.VAR:
                SymbolManager_.set_use_info(
                    TACBlock_[i].src2.value, TACBlock_[i].src2.use_info
                )
            if TACBlock_[i].dst.OperType == TACOPERANDTYPE.VAR:
                SymbolManager_.set_use_info(
                    TACBlock_[i].dst.value, TACBlock_[i].dst.use_info
                )

            # 翻译
            trans: BaseTranslator = None
            if TACBlock_[i].op == TACOP.ASSIGN:
                trans: AssignTranslator = AssignTranslator()
            elif TACBlock_[i].op == TACOP.CALL:
                trans: CallTranslator = CallTranslator()
            elif TACBlock_[i].op == TACOP.FUN_PARA:
                trans: FunparaTranslator = FunparaTranslator()
            elif TACBlock_[i].op == TACOP.FUN_RET:
                trans: FunretTranslator = FunretTranslator()
            elif TACBlock_[i].op == TACOP.PARA:
                trans: ParaTranslator = ParaTranslator()
            elif TACBlock_[i].op == TACOP.RET:
                trans: RetTranslator = RetTranslator()
            elif TACBlock_[i].op == TACOP.GOTO:
                trans: GotoTranslator = GotoTranslator()
            elif TACBlock_[i].op == TACOP.LABEL:
                trans: LabelTranslator = LabelTranslator()
            elif TACBlock_[i].op == TACOP.CREATLIST:
                trans: CreatelistTranslator = CreatelistTranslator()
            elif TACBlock_[i].op == TACOP.IFEQ:
                trans: IfTranslator = IfTranslator()
            elif TACBlock_[i].op == TACOP.IFGE:
                trans: IfTranslator = IfTranslator()
            elif TACBlock_[i].op == TACOP.IFGT:
                trans: IfTranslator = IfTranslator()
            elif TACBlock_[i].op == TACOP.IFLE:
                trans: IfTranslator = IfTranslator()
            elif TACBlock_[i].op == TACOP.IFLT:
                trans: IfTranslator = IfTranslator()
            elif TACBlock_[i].op == TACOP.IFNEQ:
                trans: IfTranslator = IfTranslator()
            elif TACBlock_[i].op == TACOP.DIV or TACBlock_[i].op == TACOP.MUL:
                trans: MulTranslator = MulTranslator()
            else:
                trans: CommonTranslator = CommonTranslator()

            tmp_res: ASMLines = trans.SentenceTranslate(SymbolManager_, TACBlock_[i])
            ASMBlock_.asmlines.extend(tmp_res)

        stack_len: int = SymbolManager_.get_stack_len()
        if stack_len > 0:
            SymbolManager_.set_esp_bias(-4 * stack_len)
            ASMBlock_.asmlines.append(
                construct_asm(op="add", dst=REG.ESP, src=str(stack_len * 4))
            )
        elif stack_len < 0:
            logging.error(f"{stack_len} stack overflow")

        if SymbolManager_.get_name() == "main":
            SymbolManager_.pop_reg(REG.EBP)
            ASMBlock_.asmlines.append(construct_asm(op="pop", src=REG.EBP))
            # mov eax, 1
            # mov ebx, 0
            # int 80h
            ASMBlock_.asmlines.append(construct_asm(op="mov", dst=REG.EAX, src=str(1)))
            ASMBlock_.asmlines.append(construct_asm(op="mov", dst=REG.EBX, src=str(0)))
            ASMBlock_.asmlines.append(construct_asm(op="int", src="80h"))
        else:
            SymbolManager_.pop_reg(REG.ESI)
            SymbolManager_.pop_reg(REG.EDX)
            SymbolManager_.pop_reg(REG.ECX)
            SymbolManager_.pop_reg(REG.EBX)
            SymbolManager_.pop_reg(REG.EAX)
            ASMBlock_.asmlines.append(construct_asm(op="pop", src=REG.ESI))
            ASMBlock_.asmlines.append(construct_asm(op="pop", src=REG.EDX))
            ASMBlock_.asmlines.append(construct_asm(op="pop", src=REG.ECX))
            ASMBlock_.asmlines.append(construct_asm(op="pop", src=REG.EBX))
            ASMBlock_.asmlines.append(construct_asm(op="pop", src=REG.EAX))
            SymbolManager_.pop_reg(REG.EBP)
            ASMBlock_.asmlines.append(construct_asm(op="pop", src=REG.EBP))
            ASMBlock_.asmlines.append(construct_asm(op="ret"))

        return ASMBlock_
