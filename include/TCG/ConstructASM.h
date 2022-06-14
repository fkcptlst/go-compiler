#ifndef INCLUDE_TCG_CONSTRUCTASM_H_
#define INCLUDE_TCG_CONSTRUCTASM_H_
#include "Common/Common.h"
#include "Common/REG.h"

std::string construct_asm(std::string op, REG dst, REG src);
std::string construct_asm(std::string op, REG dst, int src);
std::string construct_asm(std::string op, REG dst, std::string src);
std::string construct_asm(std::string op, int dst, REG src);
std::string construct_asm(std::string op, int dst, std::string src);
std::string construct_asm(std::string op, std::string src);
std::string construct_asm(std::string op, REG src);
std::string construct_asm(std::string op, int src);
std::string construct_asm(std::string op);

inline std::string construct_asm(std::string op, REG dst, REG src) {
    return op + " \t " + tostring(dst) + " \t , \t " + tostring(src);
}

inline std::string construct_asm(std::string op, REG dst, int src) {
    return op + " \t " + tostring(dst) + " \t , \t " + "[ ebp" + (src > 0 ? " - " : " + ") + std::to_string(src > 0 ? src : -src) + " ]";
}

inline std::string construct_asm(std::string op, REG dst, std::string src) {
    return op + " \t " + tostring(dst) + " \t , \t " + src;
}

inline std::string construct_asm(std::string op, int dst, REG src) {
    return op + " \t " + "[ ebp" + (dst > 0 ? " - " : " + ") + std::to_string(dst > 0 ? dst : -dst) + " ]" + " \t , \t " + tostring(src);
}

inline std::string construct_asm(std::string op, int dst, std::string src) {
    return op + " \t " + "[ ebp" + (dst > 0 ? " - " : " + ") + std::to_string(dst > 0 ? dst : -dst) + " ]" + " \t , \t " + src;
}


inline std::string construct_asm(std::string op, std::string src) {
    return op + " \t " + src;
}

inline std::string construct_asm(std::string op, REG src) {
    return op + " \t " + tostring(src);
}

inline std::string construct_asm(std::string op, int src) {
    return op + " \t " + "[ ebp" + (src > 0 ? " - " : " + ") + std::to_string(src > 0 ? src : -src) + " ]";
}

inline std::string construct_asm(std::string op) {
    return op;
}

#endif // INCLUDE_TCG_CONSTRUCTASM_H_
