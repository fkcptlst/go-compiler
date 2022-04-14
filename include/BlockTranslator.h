#ifndef INCLUDE_BLOCKTRANSLATOR_H_
#define INCLUDE_BLOCKTRANSLATOR_H_

#include "Common.h"
#include "ASM.h"
#include "TAC.h"
#include "SymbolManager.h"

class BlockTranslator {
public:
    ASMBlock BlockTranslate(SymbolManager&, TACBlock&);
};


#endif // INCLUDE_BLOCKTRANSLATOR_H_
