#ifndef INCLUDE_TCG_SENTENCETRANSLATOR_COMMONTRANSLATOR_H_
#define INCLUDE_TCG_SENTENCETRANSLATOR_COMMONTRANSLATOR_H_

#include "Common.h"
#include "TCG/SentenceTranslator/BaseTranslator.h"

class CommonTranslator : public BaseTranslator {
public:
    ASMLines SentenceTranslate(SymbolManager&, TACLine&) override;
};

#endif // INCLUDE_TCG_SENTENCETRANSLATOR_COMMONTRANSLATOR_H_