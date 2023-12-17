from ..common.common import *
from ..common.tac import *
from ..common.Scope import *
from asm import *
from BlockTranslator import *
import logging
import pickle

class Translator:
    def __init__(self, TACFile_: TACFile, Global_Scope_: Scope):
        self.BlockTranslator_: BlockTranslator = BlockTranslator()
        self.TACFile_ = TACFile_
        self.Global_Scope = Global_Scope_
        self.ASMFile_: ASMFile = []

    def dataTranslate(self):
        ASMSection_ = ASMSection()
        ASMSection_.name = ".data"

        # todo 根据符号表的静态, SymbolManager_(Scope_) 变量，生成数据段
        # todo 需要提供global的scope
        # todo 在global的scope里面找全局变量
        ASMBlock_ = ASMBlock()
        ASMBlock_.name = ""
        global_: TACBlock = self.TACFile_["global"]
        for i in range(len(global_)):
            ASMLine_: str = ""
            if global_[i].op == TACOP.ASSIGN:
                ASMLine_ = global_[i].dst.value + "   equ  " + global_[i].src1.value
                ASMBlock_.asmlines.append(ASMLine_)
            else:
                logging.error("global op error: " + f'{pickle.dumps(global_[i].op)}')

        ASMSection_.asmBlocks.append(ASMBlock_)
        self.ASMFile_.append(ASMSection_)

    def textTranslate(self):
        ASMSection_ = ASMSection()
        ASMSection_.name = ".text"

        # todo 加入global start语句
        ASMBlock_ = ASMBlock()
        ASMBlock_.name = ""
        ASMBlock_.asmlines.append("global _start")
        ASMBlock_.asmlines.append("extern myprint")
        ASMSection_.asmblocks.append(ASMBlock_)

        # todo 如何遍历TACblock待定
        # todo 以函数为单位
        for key, value in self.TACFile_.items():
            # todo 根据函数名到block的map初始化
            if key == "global" or key == 'myprint' or len(value) == 0:
                continue
            logging.warning("start 翻译函数: " + key)
            SymbolManager_: SymbolManager = SymbolManager(self.Global_Scope, key)
            ASMBlock_: ASMBlock = self.BlockTranslator_.BlockTranslate(SymbolManager_, value)
            ASMSection_.asmblocks.append(ASMBlock_)

        self.ASMFile_.append(ASMSection_)


    def Translate(self):
        self.dataTranslate()
        self.textTranslate()

    def OutputFile(self, filename: str):
        with open(filename, "w") as outfile:
            for i in range(len(self.ASMFile_)):
                if self.ASMFile_[i].name != "":
                    outfile.write(self.ASMFile_[i].name + ":\n")
                for j in range(len(self.ASMFile_[i].asmblocks)):
                    if self.ASMFile_[i].asmblocks[j].name != "":
                        outfile.write(self.ASMFile_[i].asmblocks[j].name + ":\n")
                    for k in range(len(self.ASMFile_[i].asmblocks[j].asmlines)):
                        outfile.write(self.ASMFile_[i].asmblocks[j].asmlines[k] + "\n")
                    outfile.write("\n")
                outfile.write("\n")

