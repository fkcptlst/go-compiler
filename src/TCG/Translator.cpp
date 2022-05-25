#include "Common/Common.h"
#include "TCG/Translator.h"

Translator::Translator(Scope& Scopes_, TACFile& TACFile_) : TACFile_(TACFile_), SymbolManager_(SymbolManager(Scopes_)) {
     // todo 关于如何输入TAC待定
}


void Translator::Translate() {
    this->dataTranslate();
    this->textTranslate();
}

void Translator::dataTranslate() {
    ASMSection ASMSection_;
    ASMSection_.name = ".data";

    // todo 根据符号表的静态变量，生成数据段
    // todo 需要提供global的scope
    // todo 在global的scope里面找全局变量
    ASMBlock ASMBlock_;
    ASMBlock_.name = "";
    for (;;) {
        ASMLine ASMLine_;
        ASMBlock_.asmlines.push_back(ASMLine_);
    }

    ASMSection_.asmblocks.push_back(ASMBlock_);
    ASMFile_.push_back(ASMSection_);
}

void Translator::textTranslate() {
    ASMSection ASMSection_;
    ASMSection_.name = ".text";

    // todo 加入global start语句

    // todo 如何遍历TACblock待定
    // todo 以函数为单位
    for(int i = 0; i < TACFile_.size(); i++) {
        /* crTODO: 将 SymbolManager_ 改为 一个快一个 ? ljh 不用 */
        // todo 根据函数名到block的map初始化
        SymbolManager_.init_block(name_block["name"]);
        // ASMBlock ASMBlock_ = BlockTranslator_.BlockTranslate(this->SymbolManager_, TACFile_[i]);
        // ASMSection_.asmblocks.push_back(ASMBlock_);
    }

    ASMFile_.push_back(ASMSection_);
}

void Translator::OutputFile(std::string filename) {
    std::ofstream outfile;
    outfile.open(filename);

    outfile.close();
}
