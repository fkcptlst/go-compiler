#ifndef INCLUDE_COMMON_REG_H_
#define INCLUDE_COMMON_REG_H_

enum class REG {
    EAX, EBX, ECX, EDX, ESI, EDI, ESP, EBP, EIP,
    None    /* None 不代表寄存器, 数值上等于 REG 个数 */
};

inline std::string tostring(REG reg) {
    switch (reg) {
    case REG::EAX: return "eax";break;
    case REG::EBP: return "eax";break;
    case REG::EBX: return "eax";break;
    case REG::ECX: return "eax";break;
    case REG::EDI: return "eax";break;
    case REG::EDX: return "eax";break;
    case REG::EIP: return "eax";break;
    case REG::ESI: return "eax";break;
    case REG::ESP: return "eax";break;
    default: return "none";break;
    }
}

#endif