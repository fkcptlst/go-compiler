#ifndef INCLUDE_TCG_SENTENCETRANSLATOR_FUNRETTRANSLATOR_H_
#define INCLUDE_TCG_SENTENCETRANSLATOR_FUNRETTRANSLATOR_H_

#include "Common/Common.h"
#include "TCG/ASM.h"
#include "TCG/SentenceTranslator/BaseTranslator.h"

class FunretTranslator : public BaseTranslator {
public:
    ASMLines SentenceTranslate_(SymbolManager&, TACLine&) override;
};

#endif // INCLUDE_TCG_SENTENCETRANSLATOR_FUNRETTRANSLATOR_H_