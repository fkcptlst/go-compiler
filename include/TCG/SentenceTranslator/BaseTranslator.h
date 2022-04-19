#ifndef INCLUDE_TCG_SENTENCETRANSLATOR_BASETRANSLATOR_H_
#define INCLUDE_TCG_SENTENCETRANSLATOR_BASETRANSLATOR_H_

#include "Common.h"
#include "SymbolManager.h"
#include "TCG/ASM.h"
#include "TAC.h"

class BaseTranslator {
public:
    // 完成对语句的翻译
    virtual ASMLines SentenceTranslate(SymbolManager&, TACLine&) = 0;
};


#endif // INCLUDE_TCG_SENTENCETRANSLATOR_BASETRANSLATOR_H_
