#include "Symbol.h"
#include <string>
#include <map>
using namespace std;

#define SUCCESS 1
#define FAIL 0

class BaseScope{
public:
    BaseScope* enclosingScope_;
    map<string,Symbol> symbols;

    BaseScope(){
        enclosingScope_=nullptr;
    }
    BaseScope(BaseScope* enclosingScope){
        enclosingScope_=enclosingScope;
    }
    int resolve(string name,Symbol& ret){
        if (symbols.count(name)==1){
            Symbol ret = symbols[name];
            return SUCCESS;
        }
        // if not here, check any enclosing scope
        if ( enclosingScope_ != nullptr ){
            int ret_code=enclosingScope_->resolve(name,ret);
            return ret_code;
        }
        return FAIL; // not found
    }

    void define(Symbol& sym){
        symbols[sym.name_]=sym;
    }
    BaseScope* getEnclosingScope(){
        return enclosingScope_;
    }
    
};