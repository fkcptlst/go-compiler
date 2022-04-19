#ifndef INCLUDE_COMMON_SCOPE_H_
#define INCLUDE_COMMON_SCOPE_H_

#include "Common/Common.h"
#include "Common/TAC.h"
#include "Common/Symbol.h"

#define SUCCESS 1
#define FAIL 0

class Scope{
public:
    Scope* enclosingScope_;
    std::map<std::string ,Symbol*> symbols_;
    TACBlock TACBlock_;

    Scope(){
        enclosingScope_=nullptr;
    }
    Scope(Scope* enclosingScope){
        enclosingScope_=enclosingScope;
    }
    int resolve(std::string name ,Symbol& ret);

    void define(Symbol* sym);

    Scope* getEnclosingScope();

    ~Scope(){
        // cout<<"fuck"<<endl;
    }
};

#endif