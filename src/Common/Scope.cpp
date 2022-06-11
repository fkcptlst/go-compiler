#include "Common/Scope.h"

using namespace std;




int Scope::resolve(string name, std::shared_ptr<Symbol>& ret){
    if (fun_symbols.count(name)==1){
        ret = fun_symbols.at(name);
        return SUCCESS;
    }
    else if (para_symbols.count(name)==1){
        ret = para_symbols.at(name);
        return SUCCESS;
    }
    // if not here, check any enclosing scope
    if (enclosing_scope != nullptr ){
        int ret_code=enclosing_scope->resolve(name, ret);
        return ret_code;
    }
    assert(0);
    return FAIL; // not found
}

std::shared_ptr<Symbol> Scope::resolve(string name){
    std::shared_ptr<Symbol> ret;
    int success = resolve(name, ret);
    if (success == SUCCESS) {
        return ret;
    } else {
        return nullptr;
    }
}

int Scope::cur_resolve(string name){
    if (fun_symbols.count(name)==1){
        return SUCCESS;
    }
    else if (para_symbols.count(name)==1){
        return SUCCESS;
    }
    return FAIL; // not found
}

void Scope::fun_define(std::shared_ptr<Symbol> sym){
    fun_symbols[sym->name] = sym;
}

void Scope::para_define(std::shared_ptr<Symbol> sym){
    para_symbols[sym->name] = sym;
}
