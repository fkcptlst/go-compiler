#include "Common.h"
#include "BlockTranslator.h"
#include "BaseTranslator.h"

ASMBlock BlockTranslator::BlockTranslate(SymbolManager& SymbolManager_, TACBlock& TACBlock_) {
    ASMBlock ASMBlock_;

    // todo 本函数的名字
    ASMBlock_.name = "";

    // todo 完成对每个语句的翻译
    for (int i = 0; i < TACBlock_.size(); i++) {
        switch (TACBlock_[i].op) {
            default: break;
        }
    }

    return ASMBlock_;
}
