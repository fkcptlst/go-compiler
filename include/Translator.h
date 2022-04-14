#ifndef INCLUDE_TRANSLATOR_H_
#define INCLUDE_TRANSLATOR_H_

#include "Common.h"
#include "ASM.h"
#include "TAC.h"
#include "SymbolTabel.h"
#include "BlockTranslator.h"

class Translator {
private:
    SymbolManager SymbolManager_;
    BlockTranslator BlockTranslator_;
    TACFile TACFile_;
    ASMFile ASMFile_;

public:
    Translator(SymbolTabels&, TACFile&);
    ~Translator();
    void Translate();
    void dataTranslate();
    void textTranslate();
    void OutputFile();
};


#endif // INCLUDE_TRANSLATOR_H_
