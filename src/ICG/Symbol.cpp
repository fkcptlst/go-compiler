#include "Common/Symbol.h"


bool Symbol::isVar(){
    return symoblType_== SymbolType::VAR;
}

bool Symbol::isFun(){
    return symoblType_== SymbolType::FUN;
}

