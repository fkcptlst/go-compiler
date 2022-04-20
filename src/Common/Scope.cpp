#include "Common/Scope.h"

using namespace std;

#define SUCCESS 1
#define FAIL 0


int Scope::resolve(string name, Symbol* ret){
    if (symbols.count(name)==1){
        ret = &symbols[name];
        return SUCCESS;
    }
    // if not here, check any enclosing scope
    if (enclosing_scope != nullptr ){
        int ret_code=enclosing_scope->resolve(name, ret);
        return ret_code;
    }
    return FAIL; // not found
}


void Scope::define(Symbol* sym){
    symbols[sym->name] = *sym;
}
