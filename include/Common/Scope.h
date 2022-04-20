#ifndef INCLUDE_COMMON_SCOPE_H_
#define INCLUDE_COMMON_SCOPE_H_

#include "Common/Common.h"
#include "Common/UseInfo.h"

#define SUCCESS 1
#define FAIL 0


struct Scope;

struct Symbol{
    enum class SymbolType{
        VAR,
        FUN,
    };

    enum class Type {
        /* 布尔 */
        BOOL,
        /* 数字 */
        INT8, INT16, INT32, INT64, INT,
        UINT8, UINT16, UINT32, UINT64, UINT,
        FLOAT32, FLOAT64,
        COMPLEX64, COMPLEX128,
        BYTE,	/* 等价于 uint8 */
        RUNE,	/* 等价于 int32 */
        /* 字符串 */
        STRING,
        /* crTODO: 其他牛逼类型 */
    };

    std::string name;
    Scope* scope;
    SymbolType symobl_type;
    Type type;
    std::vector<Type> fun_ret_type_list;
    UseInfo use_info;                           /* 待用信息和活跃信息 */

    Symbol() = default;
    Symbol(std::string name, Scope* scope, SymbolType symobl_type, Type type)
    : name(name),scope(scope), symobl_type(symobl_type), type(type), use_info() {}
    Symbol(std::string name, Scope* scope, SymbolType symobl_type, std::vector<Type> fun_ret_type_list)
    : name(name),scope(scope), symobl_type(symobl_type), fun_ret_type_list(fun_ret_type_list), use_info() {}

    bool isVar();
    bool isFun();

    static Type toType(std::string s){
        if(s == "int") {
            return Type::INT;
        }
    }
};


struct Scope{
    Scope* enclosing_scope;
    std::map<std::string, Symbol> symbols;

    Scope() : enclosing_scope(nullptr), symbols() {}
    Scope(Scope* enclosing_scope) : enclosing_scope(enclosing_scope), symbols() {}

    void define(Symbol* sym);
    int resolve(std::string name, Symbol* ret);
};


inline bool Symbol::isVar() {
    return symobl_type == SymbolType::VAR;
}


inline bool Symbol::isFun() {
    return symobl_type == SymbolType::FUN;
}

#endif