#ifndef INCLUDE_COMMON_REG_H_
#define INCLUDE_COMMON_REG_H_

enum class REG {
    EAX, EBX, ECX, EDX, ESI, None,    /* None 不代表寄存器, 数值上等于可用通用 REG 个数 */
    EDI, // 保留REG
    ESP, EBP, EIP
};

inline std::string tostring(REG reg) {
    switch (reg) {
    case REG::EAX: return "eax";break;
    case REG::EBP: return "ebp";break;
    case REG::EBX: return "ebx";break;
    case REG::ECX: return "ecx";break;
    case REG::EDI: return "edi";break;
    case REG::EDX: return "edx";break;
    case REG::EIP: return "eip";break;
    case REG::ESI: return "esi";break;
    case REG::ESP: return "esp";break;
    default: return "none";break;
    }
}

#endif