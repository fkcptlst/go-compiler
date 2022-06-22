#include "Common/Common.h"
#include "TCG/SentenceTranslator/LabelTranslator.h"
#include "TCG/ASM.h"
#include "TCG/SymbolManager.h"
#include "TCG/ConstructASM.h"


ASMLines LabelTranslator::SentenceTranslate_(SymbolManager& SymbolManager_, TACLine& TACLine_) {
    ASMLines asmlines;
    std::string label_str = TACLine_.src1.value;
    asmlines.push_back(construct_asm(label_str + ":"));
    return asmlines;
}
