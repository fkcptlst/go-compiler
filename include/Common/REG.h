#ifndef INCLUDE_COMMON_REG_H_
#define INCLUDE_COMMON_REG_H_

enum class REG {
    EAX, EBX, ECX, EDX, ESI, None,    /* None 不代表寄存器, 数值上等于可用通用 REG 个数 */
    EDI, // 保留REG
    ESP, EBP, EIP
};

std::string to_string(const REG reg);

#endif
