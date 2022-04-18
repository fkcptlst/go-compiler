#include "Symbol.h"
#include <string>
#include <map>
using namespace std;

#define SUCCESS 1
#define FAIL 0

class Scope{
public:
    Scope* enclosingScope_;
    map<string,Symbol*> symbols_;
    TACBlock TACBlock_;


    Scope(){
        enclosingScope_=nullptr;
    }
    Scope(Scope* enclosingScope){
        enclosingScope_=enclosingScope;
    }
    int resolve(string name,Symbol& ret){
        if (symbols_.count(name)==1){
            Symbol* ret = symbols_[name];
            return SUCCESS;
        }
        // if not here, check any enclosing scope
        if ( enclosingScope_ != nullptr ){
            int ret_code=enclosingScope_->resolve(name,ret);
            return ret_code;
        }
        return FAIL; // not found
    }

    void define(Symbol* sym){
        symbols_[sym->name_]=sym;
    }

    Scope* getEnclosingScope(){
        return enclosingScope_;
    }
    ~Scope(){
        // cout<<"fuck"<<endl;
    }
};