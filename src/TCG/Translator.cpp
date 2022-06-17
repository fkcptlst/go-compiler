#include "Common/Common.h"
#include "TCG/Translator.h"

Translator::Translator(std::shared_ptr<TACFile> TACFile_, std::shared_ptr<Scope> Global_Scpoe_) : TACFile_(TACFile_), Global_Scope(Global_Scpoe_) {
     // todo 关于如何输入TAC待定
     ASMFile_ = std::shared_ptr<ASMFile>(new ASMFile);
}


void Translator::Translate() {
    this->dataTranslate();
    this->textTranslate();
}

void Translator::dataTranslate() {
    ASMSection ASMSection_;
    ASMSection_.name = ".data";

    // todo 根据符号表的静态, SymbolManager_(Scope_) 变量，生成数据段
    // todo 需要提供global的scope
    // todo 在global的scope里面找全局变量
    ASMBlock ASMBlock_;
    ASMBlock_.name = "";
    std::shared_ptr<TACBlock> global = (*TACFile_).at("global");
    for (int i = 0; i < global->size(); i++) {
        std::string ASMLine_;
        switch ((*global)[i].op)
        {
            case TACOP::ASSIGN: {
                ASMLine_ = (*global)[i].dst.value + "   equ " + (*global)[i].src1.value;
                ASMBlock_.asmlines.push_back(ASMLine_);
                break;
            }
            default: {
                LOG(ERROR) << "global op error: " << to_string((*global)[i].op);
                break;
            }
        }

    }

    ASMSection_.asmblocks.push_back(ASMBlock_);
    ASMFile_->push_back(ASMSection_);
}

void Translator::textTranslate() {
    ASMSection ASMSection_;
    ASMSection_.name = ".text";

    // todo 加入global start语句
    ASMBlock ASMBlock_;
    ASMBlock_.name = "";
    ASMBlock_.asmlines.push_back("global _start");
    ASMBlock_.asmlines.push_back("extern myprint");
    ASMSection_.asmblocks.push_back(ASMBlock_);

    // todo 如何遍历TACblock待定
    // todo 以函数为单位
    for(auto i = TACFile_->begin(); i != TACFile_->end(); i++) {
        /* crTODO: 将 SymbolManager_ 改为 一个快一个 ? ljh 不用 */
        // todo 根据函数名到block的map初始化
        if (i->first == "global" || i->second->size() == 0) continue;
        SymbolManager SymbolManager_(Global_Scope, i->first);
        ASMBlock ASMBlock_ = BlockTranslator_.BlockTranslate(SymbolManager_, i->second);
        ASMSection_.asmblocks.push_back(ASMBlock_);
    }

    ASMFile_->push_back(ASMSection_);
}

void Translator::OutputFile(std::string filename) {
    std::ofstream outfile;
    outfile.open(filename);


    for (int i = 0; i < ASMFile_->size(); i++) {
        if ((*ASMFile_)[i].name != "") {
            outfile << (*ASMFile_)[i].name << ":" << std::endl;
        }
        for (int j = 0; j < (*ASMFile_)[i].asmblocks.size(); j++) {
            if ((*ASMFile_)[i].asmblocks[j].name != "") {
                outfile << (*ASMFile_)[i].asmblocks[j].name << ":" << std::endl;
            }
            for (int k = 0; k < (*ASMFile_)[i].asmblocks[j].asmlines.size(); k++) {
                outfile << (*ASMFile_)[i].asmblocks[j].asmlines[k] << std::endl;
            }
            outfile << std::endl;
        }
        outfile << std::endl;
    }

    outfile.close();
}
