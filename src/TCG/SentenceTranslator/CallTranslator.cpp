#include "Common/Common.h"
#include "TCG/SentenceTranslator/CallTranslator.h"
#include "TCG/ASM.h"
#include "TCG/SymbolManager.h"

ASMLines CallTranslator::SentenceTranslate_(SymbolManager& SymbolManager_, TACLine& TACLine_) {
    ASMLines asmlines;
    asmlines.push_back(construct_asm("call", TACLine_.src1.value));
    return asmlines;
}
