#ifndef INCLUDE_COMMON_SYMBOL_H_
#define INCLUDE_COMMON_SYMBOL_H_

#include "Common/Common.h"

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
    std::string name_;
    Scope* scope_;
    SymbolType symoblType_;
    Type type_;
    std::vector<Type> funRetTypeList_;

    Symbol(){
        
    }
    Symbol(std::string name,Scope* scope,SymbolType symoblType,Type type):
      name_(name),scope_(scope),symoblType_(symoblType),type_(type){

    }
    Symbol(std::string name,Scope* scope,SymbolType symoblType,std::vector<Type> funRetTypeList):
      name_(name),scope_(scope),symoblType_(symoblType),funRetTypeList_(funRetTypeList){

    }
    bool isVar();

    bool isFun();

    static Type toType(std::string s){
        if(s=="int"){
            return Type::tINT;
        }
        
    }
};

#endif