#include "Common/Common.h"
#include "Common/REG.h"


std::string to_string(const REG reg) {
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
