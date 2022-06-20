#ifndef INCLUDE_COMMON_SCOPE_H_
#define INCLUDE_COMMON_SCOPE_H_

#include "Common/Common.h"
#include "Common/TAC.h"

#define SUCCESS 1
#define FAIL 0



struct Scope;
struct TACLine;

using TACBlock = std::vector<TACLine>;

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

    std::string         name;
    std::shared_ptr<Scope>   scope;
    // TODO:delete
    SymbolType          symobl_type;
    // 变量有，函数无
    Type                type;
    // 函数的return列表
    std::shared_ptr<std::vector<Type>>   fun_ret_type_list;
    // 函数的parameter列表
    std::shared_ptr<std::vector<Type>>   fun_para_type_list;
    // 是否是数组 0:否，1:是
    bool is_array;
    int array_length;

    Symbol() = default;
    Symbol(std::string name, std::shared_ptr<Scope> scope, SymbolType symobl_type, Type type)
    : name(name),scope(scope), symobl_type(symobl_type), type(type) {}
    Symbol(std::string name, std::shared_ptr<Scope> scope, SymbolType symobl_type, std::shared_ptr<std::vector<Type>> fun_ret_type_list, std::shared_ptr<std::vector<Type>>  fun_para_type_list)
    : name(name),scope(scope), fun_ret_type_list(fun_ret_type_list), fun_para_type_list(fun_para_type_list), symobl_type(symobl_type){}
    // Symbol(std::string name, std::shared_ptr<Scope> scope, SymbolType symobl_type, std::shared_ptr<std::vector<Type>> fun_ret_type_list, std::shared_ptr<std::vector<Type>>  fun_para_type_list, bool is_array, int length)
    // : name(name),scope(scope), fun_ret_type_list(fun_ret_type_list), fun_para_type_list(fun_para_type_list), symobl_type(symobl_type), is_array(is_array), array_length(length){}
    Symbol(std::string name, std::shared_ptr<Scope> scope, SymbolType symobl_type, Type type, bool is_array, int length)
    : name(name),scope(scope), symobl_type(symobl_type), type(type), is_array(is_array), array_length(length) {}
    bool isVar();
    bool isFun();

    static Type toType(std::string s){
        if(s == "int") {
            return Type::INT;
        }
    }
};


struct Scope{
    std::shared_ptr<Scope>                                      enclosing_scope;
    std::unordered_map<std::string, std::shared_ptr<Symbol>>   fun_symbols;
    std::unordered_map<std::string, std::shared_ptr<Symbol>>   para_symbols;

    Scope() : enclosing_scope(nullptr), fun_symbols(),para_symbols() {}
    Scope(std::shared_ptr<Scope> enclosing_scope) : enclosing_scope(enclosing_scope), fun_symbols(),para_symbols() {}

    void fun_define(std::shared_ptr<Symbol> sym);
    void para_define(std::shared_ptr<Symbol> sym);
    void para_delete(std::string str);
    int resolve(std::string name, std::shared_ptr<Symbol> &ret);
    std::shared_ptr<Symbol> resolve(std::string name);
    int cur_resolve(std::string name);
};


inline bool Symbol::isVar() {
    return symobl_type == SymbolType::VAR;
}


inline bool Symbol::isFun() {
    return symobl_type == SymbolType::FUN;
}


#endif