#ifndef INCLUDE_TCG_BLOCKTRANSLATOR_H_
#define INCLUDE_TCG_BLOCKTRANSLATOR_H_

#include "Common/Common.h"
#include "Common/TAC.h"
#include "TCG/ASM.h"
#include "TCG/SymbolManager.h"

class BlockTranslator {
public:
    // 翻译一个三地址代码快
    ASMBlock BlockTranslate(SymbolManager&, std::shared_ptr<TACBlock>);
};


#endif // INCLUDE_TCG_BLOCKTRANSLATOR_H_
