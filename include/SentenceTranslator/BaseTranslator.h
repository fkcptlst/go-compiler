#ifndef INCLUDE_SENTENCETRANSLATOR_BASETRANSLATOR_H_
#define INCLUDE_SENTENCETRANSLATOR_BASETRANSLATOR_H_

#include "Common.h"
#include "ASM.h"
#include "TAC.h"

class BaseTranslator {
private:
    /* data */
public:
    virtual ASMLines SentenceTranslate(TACLine) = 0;
};


#endif // INCLUDE_SENTENCETRANSLATOR_BASETRANSLATOR_H_
