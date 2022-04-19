#ifndef INCLUDE_SYMBOL_H_
#define INCLUDE_SYMBOL_H_

#include <string>
#include <vector>
#include <map>
// #include "Scope.h"

using namespace std;
class Scope;

class Symbol{
public:
    enum class SymbolType{
        VAR,
        FUN,
    };
    enum class Type{
        tINT,
    };
    string name_;
    Scope* scope_;
    SymbolType symoblType_;
    Type type_;
    vector<Type> funRetTypeList_;

    Symbol(){
        
    }
    Symbol(string name,Scope* scope,SymbolType symoblType,Type type):
      name_(name),scope_(scope),symoblType_(symoblType),type_(type){

    }
    Symbol(string name,Scope* scope,SymbolType symoblType,vector<Type> funRetTypeList):
      name_(name),scope_(scope),symoblType_(symoblType),funRetTypeList_(funRetTypeList){

    }
    bool isVar();

    bool isFun();

    static Type toType(string s){
        if(s=="int"){
            return Type::tINT;
        }
        
    }
};

#endif