#include "Common/Common.h"
#include "TCG/BlockTranslator.h"
#include "TCG/SentenceTranslator/BaseTranslator.h"
#include "TCG/SentenceTranslator/CommonTranslator.h"
#include "TCG/SentenceTranslator/AssignTranslator.h"
#include "TCG/SentenceTranslator/CallTranslator.h"
#include "TCG/SentenceTranslator/FunparaTranslator.h"
#include "TCG/SentenceTranslator/FunretTranslator.h"
#include "TCG/SentenceTranslator/ParaTranslator.h"
#include "TCG/SentenceTranslator/RetTranslator.h"


ASMBlock BlockTranslator::BlockTranslate(SymbolManager& SymbolManager_, std::shared_ptr<TACBlock> TACBlock_) {
    ASMBlock ASMBlock_;

    // todo 本函数的名字 退出函数以后再弄
    ASMBlock_.name = "";

    // todo 完成对每个语句的翻译
    for (int i = 0; i < TACBlock_->size(); i++) {
        std::shared_ptr<BaseTranslator> trans;
        switch ((*TACBlock_)[i].op) {
            case TACOP::ASSIGN:   trans = std::shared_ptr<AssignTranslator>(new AssignTranslator()); break;
            case TACOP::CALL:     trans = std::shared_ptr<CallTranslator>(new CallTranslator()); break;
            case TACOP::FUN_PARA: trans = std::shared_ptr<FunparaTranslator>(new FunparaTranslator()); break;
            case TACOP::FUN_RET:  trans = std::shared_ptr<FunretTranslator>(new FunretTranslator()); break;
            case TACOP::PARA:     trans = std::shared_ptr<ParaTranslator>(new ParaTranslator()); break;
            case TACOP::RET:      trans = std::shared_ptr<RetTranslator>(new RetTranslator()); break;
            default: trans = std::shared_ptr<CommonTranslator>(new CommonTranslator()); break;
        }
        ASMLines tmp_res = trans->SentenceTranslate(SymbolManager_, (*TACBlock_)[i]);
        ASMBlock_.asmlines.insert(ASMBlock_.asmlines.end(), tmp_res.begin(), tmp_res.end());
    }

    return ASMBlock_;
}
