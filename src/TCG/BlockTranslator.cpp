#include "Common/Common.h"
#include "TCG/BlockTranslator.h"
#include "TCG/SentenceTranslator/BaseTranslator.h"
#include "TCG/SentenceTranslator/CommonTranslator.h"


ASMBlock BlockTranslator::BlockTranslate(SymbolManager& SymbolManager_, std::shared_ptr<TACBlock> TACBlock_) {
    ASMBlock ASMBlock_;

    // todo 本函数的名字 退出函数以后再弄
    ASMBlock_.name = "";

    // todo 完成对每个语句的翻译
    for (int i = 0; i < TACBlock_->size(); i++) {
        std::shared_ptr<BaseTranslator> trans;
        switch ((*TACBlock_)[i].op) {
            default: trans = std::shared_ptr<CommonTranslator>(new CommonTranslator()); break;
        }
        ASMLines tmp_res = trans->SentenceTranslate(SymbolManager_, (*TACBlock_)[i]);
        ASMBlock_.asmlines.insert(ASMBlock_.asmlines.end(), tmp_res.begin(), tmp_res.end());
    }

    return ASMBlock_;
}
