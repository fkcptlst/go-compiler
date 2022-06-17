#include "Common/Common.h"
#include "TCG/BlockTranslator.h"
#include "TCG/ConstructASM.h"
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

    if (SymbolManager_.get_name() == "main") {
        ASMBlock_.name = "_start";
    } else {
        ASMBlock_.name = SymbolManager_.get_name();

        SymbolManager_.push_reg(REG::EBP, 0);
        ASMBlock_.asmlines.push_back(construct_asm("push", REG::EBP));
        ASMBlock_.asmlines.push_back(construct_asm("mov", REG::EBP, REG::ESP));

        SymbolManager_.push_reg(REG::EAX, 0);
        SymbolManager_.push_reg(REG::EBX, 0);
        SymbolManager_.push_reg(REG::ECX, 0);
        SymbolManager_.push_reg(REG::EDX, 0);
        SymbolManager_.push_reg(REG::ESI, 0);
        ASMBlock_.asmlines.push_back(construct_asm("push", REG::EAX));
        ASMBlock_.asmlines.push_back(construct_asm("push", REG::EBX));
        ASMBlock_.asmlines.push_back(construct_asm("push", REG::ECX));
        ASMBlock_.asmlines.push_back(construct_asm("push", REG::EDX));
        ASMBlock_.asmlines.push_back(construct_asm("push", REG::ESI));
    }
    SymbolManager_.set_zero_len();
    // todo 完成对每个语句的翻译
    LOG(WARNING) << "逐行翻译 TAC block 到 ASM";
    for (int i = 0; i < TACBlock_->size(); i++) {
        LOG(INFO) << (*TACBlock_)[i].to_string();
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



    int stack_len = SymbolManager_.get_stack_len();
    if (stack_len > 0) {
        ASMBlock_.asmlines.push_back(construct_asm("add", REG::ESP, std::to_string(4 * stack_len)));
    } else {
        LOG(INFO) << "stack overflow";
    }

    if (SymbolManager_.get_name() == "main") {
        // mov eax,1
        // mov ebx,0
        // int 80h
        ASMBlock_.asmlines.push_back(construct_asm("mov", REG::EAX, std::to_string(1)));
        ASMBlock_.asmlines.push_back(construct_asm("mov", REG::EBX, std::to_string(0)));
        ASMBlock_.asmlines.push_back(construct_asm("int", "80h"));
    } else {
        SymbolManager_.pop_reg(REG::ESI);
        SymbolManager_.pop_reg(REG::EDX);
        SymbolManager_.pop_reg(REG::ECX);
        SymbolManager_.pop_reg(REG::EBX);
        SymbolManager_.pop_reg(REG::EAX);
        ASMBlock_.asmlines.push_back(construct_asm("pop", REG::ESI));
        ASMBlock_.asmlines.push_back(construct_asm("pop", REG::EDX));
        ASMBlock_.asmlines.push_back(construct_asm("pop", REG::ECX));
        ASMBlock_.asmlines.push_back(construct_asm("pop", REG::EBX));
        ASMBlock_.asmlines.push_back(construct_asm("pop", REG::EAX));
        SymbolManager_.pop_reg(REG::EBP);
        ASMBlock_.asmlines.push_back(construct_asm("pop", REG::EBP));
    }

    // ret
    ASMBlock_.asmlines.push_back(construct_asm("ret"));

    return ASMBlock_;
}
