#ifndef INCLUDE_TCG_SENTENCETRANSLATOR_IFTRANSLATOR_H_
#define INCLUDE_TCG_SENTENCETRANSLATOR_IFTRANSLATOR_H_

#include "Common/Common.h"
#include "TCG/ASM.h"
#include "TCG/SentenceTranslator/BaseTranslator.h"

class IfTranslator : public BaseTranslator {
public:
    ASMLines SentenceTranslate_(SymbolManager&, TACLine&) override;
};

#endif // INCLUDE_TCG_SENTENCETRANSLATOR_IFTRANSLATOR_H_