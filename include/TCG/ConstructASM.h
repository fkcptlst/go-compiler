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

inline std::string construct_asm(std::string op, std::string src1, std::string src2) {
    return "\t" + op + "\t" + src1 + "\t , \t" + src2;
}

inline std::string construct_asm(std::string op, REG dst, REG src) {
    return "\t" + op + " \t " + to_string(dst) + " \t , \t " + to_string(src);
}

inline std::string construct_asm(std::string op, REG dst, int src) {
    return "\t" + op + " \t " + to_string(dst) + " \t , \t " + "[ ebp" + (src > 0 ? " - " : " + ") + std::to_string(src > 0 ? src : -src) + " ]";
}

inline std::string construct_asm(std::string op, REG dst, std::string src) {
    return "\t" + op + " \t " + to_string(dst) + " \t , \t " + src;
}

inline std::string construct_asm(std::string op, int dst, REG src) {
    return "\t" + op + " \t " + "[ ebp" + (dst > 0 ? " - " : " + ") + std::to_string(dst > 0 ? dst : -dst) + " ]" + " \t , \t " + to_string(src);
}

inline std::string construct_asm(std::string op, int dst, std::string src) {
    return "\t" + op + " \t " + "[ ebp" + (dst > 0 ? " - " : " + ") + std::to_string(dst > 0 ? dst : -dst) + " ]" + " \t , \t " + src;
}

// 指针寻址
inline std::string construct_asm_mem(REG reg, int offset, std::string data_size = "") {
    return data_size + "[ " + to_string(reg) + (offset > 0 ? " - " : " + ") + std::to_string(offset > 0 ? offset : -offset) + " ]";
}

inline std::string construct_asm(std::string op, REG dst, int dst_offset, REG src1) {
    return "\t" + op + "\t" + construct_asm_mem(dst, dst_offset, "dword") + " \t , \t " + to_string(src1);
}

inline std::string construct_asm(std::string op, REG dst, int dst_offset, int src1_mem) {
    return "\t" + op + "\t" + construct_asm_mem(dst, dst_offset, "dword") + " \t , \t " + "[ ebp" + (src1_mem > 0 ? " - " : " + ") + std::to_string(src1_mem > 0 ? src1_mem : -src1_mem) + " ]";
}

inline std::string construct_asm(std::string op, REG dst, int dst_offset, std::string src1_globle) {
    return "\t" + op + "\t" + construct_asm_mem(dst, dst_offset, "dword") + " \t , \t " + src1_globle;
}


inline std::string construct_asm(std::string op, std::string src) {
    return "\t" + op + " \t " + src;
}

inline std::string construct_asm(std::string op, REG src) {
    return "\t" + op + " \t " + to_string(src);
}

inline std::string construct_asm(std::string op, int src) {
    return "\t" + op + " \t " + "[ ebp" + (src > 0 ? " - " : " + ") + std::to_string(src > 0 ? src : -src) + " ]";
}

inline std::string construct_asm(std::string op) {
    return "\t" + op;
}

#endif // INCLUDE_TCG_CONSTRUCTASM_H_
