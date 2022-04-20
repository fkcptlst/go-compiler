#ifndef INCLUDE_COMMON_SCOPE_H_
#define INCLUDE_COMMON_SCOPE_H_

#include "Common/Common.h"
#include "Common/Scope.h"


#define SUCCESS 1
#define FAIL 0


struct UseInfo {
	int64_t next_use;		/* 待用信息 */
	bool active;			/* 活跃信息 */

	UseInfo(int64_t next_use=0, bool active=false) : next_use(next_use), active(active) {}

	UseInfo& operator=(const UseInfo& rhs) {
		next_use = rhs.next_use;
		active = rhs.active;
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& os, const UseInfo& rhs) {
		os << rhs.next_use << "," << ((rhs.active) ? "Y" : "^");
		return os;
	}

	bool no_use() const {
		return (next_use == 0 && active == false);
	}
};


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

    std::string name_;
    Scope* scope_;
    SymbolType symoblType_;
    Type type_;
    std::vector<Type> funRetTypeList_;
    /* 待用信息和活跃信息 */
    UseInfo use_info;

    Symbol() {}
    Symbol(std::string name, Scope* scope, SymbolType symoblType, Type type)
    : name_(name),scope_(scope), symoblType_(symoblType), type_(type), use_info() {}
    Symbol(std::string name, Scope* scope, SymbolType symoblType, std::vector<Type> funRetTypeList)
    : name_(name),scope_(scope), symoblType_(symoblType), funRetTypeList_(funRetTypeList), use_info() {}

    bool isVar() {return symoblType_ == SymbolType::VAR;}
    bool isFun() {return symoblType_ == SymbolType::FUN;}

    static Type toType(std::string s){
        if(s=="int"){
            return Type::INT;
        }
        
    }
};

struct Scope{
    Scope* enclosing_scope;
    std::map<std::string, Symbol> symbols;

    Scope() : enclosing_scope(nullptr), symbols() {}
    Scope(Scope* enclosing_scope) : enclosing_scope(enclosing_scope), symbols() {}

    int resolve(std::string name ,Symbol* ret);
    void define(Symbol* sym);
};

#endif