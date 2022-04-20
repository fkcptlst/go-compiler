#ifndef INCLUDE_COMMON_REG_H_
#define INCLUDE_COMMON_REG_H_

enum class REG {
    EAX, EBX, ECX, EDX, ESI, EDI, ESP, EBP, EIP,
    None    /* None 不代表寄存器, 数值上等于 REG 个数 */
};

#endif