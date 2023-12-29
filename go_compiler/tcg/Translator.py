import pickle
from loguru import logger
from ..tcg.SymbolManager import SymbolManager
from ..common.tac import TACOP, TACBlock, TACFile
from ..common.Scope import Scope
from ..tcg.BlockTranslator import ASMBlock, BlockTranslator
from ..tcg.asm import ASMFile, ASMSection


class Translator:
    def __init__(self, TACFile_: TACFile, Global_Scope_: Scope | None):
        self.BlockTranslator_: BlockTranslator = BlockTranslator()
        self.TACFile_ = TACFile_
        self.Global_Scope = Global_Scope_
        self.ASMFile_: ASMFile = []

    # BlockTranslator BlockTranslator_;
    # std::shared_ptr<Scope> Global_Scope;
    # std::shared_ptr<TACFile> TACFile_;
    # std::shared_ptr<ASMFile> ASMFile_;

    def dataTranslate(self):
        ASMSection_ = ASMSection()
        ASMSection_.name = ".data"

        # TODO: 根据符号表的静态, SymbolManager_(Scope_) 变量，生成数据段
        # TODO: 需要提供global的scope
        # TODO: 在global的scope里面找全局变量
        ASMBlock_ = ASMBlock()
        ASMBlock_.name = ""
        global_: TACBlock = self.TACFile_["global"]
        # logger.debug(f"TACFile: {self.TACFile_}")
        for i in range(len(global_)):
            ASMLine_: str = ""
            if global_[i].op == TACOP.ASSIGN:
                ASMLine_ = global_[i].dst.value + "   equ  " + global_[i].src1.value
                ASMBlock_.asmlines.append(ASMLine_)
            else:
                logger.error("global op error: " + f"{pickle.dumps(global_[i].op)}")

        ASMSection_.asmblocks.append(ASMBlock_)
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
            # TODO: 根据函数名到block的map初始化
            if key == "global" or key == "myprint" or len(value) == 0:
                continue
            logger.debug("start 翻译函数: " + key)
            SymbolManager_: SymbolManager = SymbolManager(self.Global_Scope, key)
            # logger.debug(f"InputValue: {key} : {value}")
            ASMBlock_: ASMBlock = self.BlockTranslator_.BlockTranslate(
                SymbolManager_, value
            )

            ASMSection_.asmblocks.append(ASMBlock_)

        self.ASMFile_.append(ASMSection_)

    def Translate(self):
        self.dataTranslate()
        self.textTranslate()

    def OutputFile(self, filename: str):
        logger.debug(f"Writing {filename}")
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
