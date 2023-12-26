#ifndef INCLUDE_TCG_SENTENCETRANSLATOR_FUNPARATRANSLATOR_H_
#define INCLUDE_TCG_SENTENCETRANSLATOR_FUNPARATRANSLATOR_H_

#include "Common/Common.h"
#include "TCG/ASM.h"
#include "TCG/SentenceTranslator/BaseTranslator.h"

class FunparaTranslator : public BaseTranslator {
public:
    ASMLines SentenceTranslate_(SymbolManager&, TACLine&) override;
};

#endif // INCLUDE_TCG_SENTENCETRANSLATOR_FUNPARATRANSLATOR_H_