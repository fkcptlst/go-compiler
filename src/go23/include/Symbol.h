#include <string>
#include <map>
// #include "BaseScope.h"

using namespace std;
class BaseScope;

class Symbol{
public:
    enum class symbolType{
        VAR,
        FUN,
    };

    string name_;
    BaseScope scope_;
    symbolType type_;

    // string getName(){
    //     return name_;
    // }
    // void setScope(BaseScope scp){
    //     scope_=scp;
    // }
    bool isVar(){
        return type_== symbolType::VAR;
    }
    bool isFun(){
        return type_== symbolType::FUN;
    }

};