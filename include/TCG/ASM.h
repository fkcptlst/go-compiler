#ifndef INCLUDE_TCG_ASM_H_
#define INCLUDE_TCG_ASM_H_

#include "Common/Common.h"
#include "Common/REG.h"

enum class ASMOP {
    MOV, PUSH, POP, CALL, JMP, RET,
};

enum class OprandType {
    t_NULL, t_IMM, t_REG, t_MEM,
};

struct ASMOprand {
    OprandType type;
    int32_t value;
    ASMOprand() {}
    ASMOprand(OprandType type, int32_t value)
    : type(type), value(value) {}
};

struct ASMLine {
    ASMOP op;
    std::vector<ASMOprand> oprands;

    ASMLine() {}
    ASMLine(ASMOP op, std::vector<ASMOprand> oprands)
    : op(op), oprands(oprands) {}
};

// using ASMLines = std::vector<ASMLine>;
using ASMLines = std::vector<std::string>;

struct ASMBlock {
    std::string name;
    ASMLines asmlines;

    ASMBlock() {}
    ASMBlock(std::string name, ASMLines asmlines)
    : name(name), asmlines(asmlines) {}
};

struct ASMSection {
    std::string name;
    std::vector<ASMBlock> asmblocks;

    ASMSection() {}
    ASMSection(std::string name, std::vector<ASMBlock> asmblocks)
    : name(name), asmblocks(asmblocks) {}
};

using ASMFile = std::vector<ASMSection>;

// inline std::string to_string(REG reg) {
//     switch (reg) {
//         case REG::EAX : return "eax";
//         case REG::EBX : return "ebx";
//         case REG::ECX : return "ecx";
//         case REG::EDX : return "edx";
//         case REG::ESI : return "esi";
//         case REG::EDI : return "edi";
//         case REG::ESP : return "esp";
//         case REG::EBP : return "ebp";
//         case REG::EIP : return "eip";
//         default: return "None";
//     }
// }

// inline std::string to_string(ASMOprand ASMOprand_) {
//     std::stringstream ss;
//     std::string hex_s;
//     ss << std::hex << ASMOprand_.value;
//     ss >> hex_s;
//     switch (ASMOprand_.type) {
//         case OprandType::t_IMM : return "0x" + hex_s;
//         case OprandType::t_REG : return to_string(REG(ASMOprand_.value));
//         case OprandType::t_MEM : return "[" + hex_s + "]";
//         case OprandType::t_NULL : return "";
//         default: return "None";
//     }
// }

// inline std::string to_string(ASMLine ASMLine_) {
//     std::string asmop;
//     switch (ASMLine_.op) {
//         case ASMOP::MOV : asmop = "mov";
//         default: asmop = "?";
//     }
//     std::string asmline = asmop;
//     for (int i = 0; i < ASMLine_.oprands.size(); i++) {
//         if (i != 0) {
//             asmline = asmline + ",";
//         }
//         asmline = asmline + to_string(ASMLine_.oprands[i]);
//     }
//     return asmline;
// }


#endif // INCLUDE_TCG_ASM_H_
