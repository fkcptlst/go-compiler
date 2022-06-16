#include "Common/Common.h"
#include "TCG/SentenceTranslator/CallTranslator.h"
#include "TCG/ASM.h"
#include "TCG/SymbolManager.h"
#include "TCG/ConstructASM.h"


ASMLines CallTranslator::SentenceTranslate_(SymbolManager& SymbolManager_, TACLine& TACLine_) {
    ASMLines asmlines;
    std::string fun_name = TACLine_.src1.value;
    asmlines.push_back(construct_asm("call", fun_name));
    std::shared_ptr<Symbol> fun = SymbolManager_.resolve_fun(fun_name);
    if (fun != nullptr) {
        int para_num = fun->fun_para_type_list->size();
        SymbolManager_.set_esp_bias(-4 * para_num);
        asmlines.push_back(construct_asm("add", REG::ESP, std::to_string(4 * para_num)));
    } else {
        std::cout << "not a fun" << std::endl;
    }
    return asmlines;
}
