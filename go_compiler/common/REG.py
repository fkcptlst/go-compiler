from enum import Enum, auto


class REG(Enum):
    EAX = auto()
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
