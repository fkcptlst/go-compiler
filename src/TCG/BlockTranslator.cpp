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
    SymbolManager_.push_reg(REG::EBP, 0);
    ASMBlock_.asmlines.push_back(construct_asm("push", REG::EBP));
    ASMBlock_.asmlines.push_back(construct_asm("mov", REG::EBP, REG::ESP));

    if (SymbolManager_.get_name() == "main") {
        ASMBlock_.name = "_start";
    } else {
        ASMBlock_.name = SymbolManager_.get_name();

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
    for (int i = 0; i < TACBlock_->size(); i++) {
        std::shared_ptr<BaseTranslator> trans;
        switch ((*TACBlock_)[i].op) {
            case TACOP::ASSIGN:   trans = std::make_shared<AssignTranslator>(); break;
            case TACOP::CALL:     trans = std::make_shared<CallTranslator>(); break;
            case TACOP::FUN_PARA: trans = std::make_shared<FunparaTranslator>(); break;
            case TACOP::FUN_RET:  trans = std::make_shared<FunretTranslator>(); break;
            case TACOP::PARA:     trans = std::make_shared<ParaTranslator>(); break;
            case TACOP::RET:      trans = std::make_shared<RetTranslator>(); break;
            default: trans = std::make_shared<CommonTranslator>(); break;
        }
        ASMLines tmp_res = trans->SentenceTranslate(SymbolManager_, (*TACBlock_)[i]);
        ASMBlock_.asmlines.insert(ASMBlock_.asmlines.end(), tmp_res.begin(), tmp_res.end());
        LOG(INFO) << (*TACBlock_)[i].to_string();
        // std::cout << (*TACBlock_)[i].to_string() << std::endl;
    }



    int stack_len = SymbolManager_.get_stack_len();
    if (stack_len > 0) {
        SymbolManager_.set_esp_bias(-4 * stack_len);
        ASMBlock_.asmlines.push_back(construct_asm("add", REG::ESP, std::to_string(4 * stack_len)));
    } else if(stack_len < 0)  {
        LOG(ERROR) << stack_len << "stack overflow";
    }

    if (SymbolManager_.get_name() == "main") {
        SymbolManager_.pop_reg(REG::EBP);
        ASMBlock_.asmlines.push_back(construct_asm("pop", REG::EBP));
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
        ASMBlock_.asmlines.push_back(construct_asm("ret"));
    }

    return ASMBlock_;
}
