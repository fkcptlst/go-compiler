#include "Common.h"
#include "TCG/Translator.h"

Translator::Translator(SymbolTabels& SymbolTabels_, TACFile& TACFile_) {
    TACFile_ = TACFile_;
    ASMFile_.clear();
}


void Translator::Translate() {
    this->dataTranslate();
    this->textTranslate();
}

void Translator::dataTranslate() {
    ASMSection ASMSection_;
    ASMSection_.name = ".data";

    // todo 根据符号表的静态变量，生成数据段
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

    for(int i = 0; i < TACFile_.size(); i++) {
        ASMBlock ASMBlock_ = BlockTranslator_.BlockTranslate(this->SymbolManager_, TACFile_[i]);
        ASMSection_.asmblocks.push_back(ASMBlock_);
    }

    ASMFile_.push_back(ASMSection_);
}

void Translator::OutputFile(std::string filename) {
    std::ofstream outfile;
    outfile.open(filename);

    outfile.close();
}
