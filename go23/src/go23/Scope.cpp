#include "Symbol.h"
#include <string>
#include <map>
#include "Scope.h"
using namespace std;

#define SUCCESS 1
#define FAIL 0

int Scope::resolve(string name,Symbol& ret){
    if (symbols_.count(name)==1){
        Symbol* ret = symbols_[name];
        return SUCCESS;
    }
    // if not here, check any enclosing scope
    if (enclosingScope_ != nullptr ){
        int ret_code=enclosingScope_->resolve(name,ret);
        return ret_code;
    }
    return FAIL; // not found
}

void Scope::define(Symbol* sym){
    symbols_[sym->name_]=sym;
}

Scope* Scope::getEnclosingScope(){
    return enclosingScope_;
}