#ifndef INCLUDE_TCG_ASM_H_
#define INCLUDE_TCG_ASM_H_

#include "Common.h"

enum class OP {
    MOV, PUSH, POP, CALL, JMP, RET,
};

enum class REG {
    EAX, EBX, ECX, EDX, ESI, EDI, ESP, EBP, EIP,
    None    /* None 不代表寄存器, 数值上等于 REG 个数 */
};


inline std::string to_string(REG reg) {
    switch (reg) {
        case REG::EAX : return "eax";
        case REG::EBX : return "ebx";
        case REG::ECX : return "ecx";
        case REG::EDX : return "edx";
        case REG::ESI : return "esi";
        case REG::EDI : return "edi";
        case REG::ESP : return "esp";
        case REG::EBP : return "ebp";
        case REG::EIP : return "eip";
        default: return "None";
    }
}

struct ASMLine {
    OP op;
    std::string oprand_1;  // 这里先待定
    std::string oprand_2;
    std::string oprand_3;
};

using ASMLines = std::vector<ASMLine>;

struct ASMBlock {
    std::string name;
    ASMLines asmlines;
};

struct ASMSection {
    std::string name;
    std::vector<ASMBlock> asmblocks;
};

using ASMFile = std::vector<ASMSection>;

#endif // INCLUDE_TCG_ASM_H_
