#ifndef INCLUDE_ASM_H_
#define INCLUDE_ASM_H_

#include "Common.h"

enum OP {
    MOV, PUSH, POP, CALL, JMP, RET,
};

enum REG {
    EAX, EBX, ECX, EDX, ESI, EDI, ESP, EBP, EIP,
};

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

#endif // INCLUDE_ASM_H_
