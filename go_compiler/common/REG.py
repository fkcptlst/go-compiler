from enum import Enum, auto


# NOTE: now start with 0
class REG(Enum):
    EAX = 0
    EBX = auto()
    ECX = auto()
    EDX = auto()
    ESI = auto()

    NONE = auto()

    EDI = auto()

    ESP = auto()
    EBP = auto()
    EIP = auto()


def to_string(reg: REG) -> str:
    return reg.name.lower()
