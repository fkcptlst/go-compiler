#ifndef INCLUDE_SCOPE_H_
#define INCLUDE_SCOPE_H_

#include "Symbol.h"
#include "TAC.h"
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
    int resolve(string name,Symbol& ret);

    void define(Symbol* sym);

    Scope* getEnclosingScope();

    ~Scope(){
        // cout<<"fuck"<<endl;
    }
};

#endif