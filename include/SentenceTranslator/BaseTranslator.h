#ifndef INCLUDE_SENTENCETRANSLATOR_BASETRANSLATOR_H_
#define INCLUDE_SENTENCETRANSLATOR_BASETRANSLATOR_H_

#include "Common.h"
#include "ASM.h"
#include "TAC.h"

class BaseTranslator {
public:
    // 完成对语句的翻译
    virtual ASMLines SentenceTranslate(TACLine) = 0;
};


#endif // INCLUDE_SENTENCETRANSLATOR_BASETRANSLATOR_H_
