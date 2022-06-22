#include "Common/Common.h"
#include "TCG/SentenceTranslator/GotoTranslator.h"
#include "TCG/ASM.h"
#include "TCG/SymbolManager.h"
#include "TCG/ConstructASM.h"


ASMLines GotoTranslator::SentenceTranslate_(SymbolManager& SymbolManager_, TACLine& TACLine_) {
    ASMLines asmlines;
    std::string str_label = TACLine_.src1.value;
    asmlines.push_back(construct_asm("jmp", str_label));
    return asmlines;
}
