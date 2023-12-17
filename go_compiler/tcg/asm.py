from enum import Enum
from typing import TypeAlias


class ASMOP(Enum):
    MOV = auto()
    PUSH = auto()
    POP = auto()
    CALL = auto()
    JMP = auto()
    RET = auto()


class OprandType(Enum):
    t_NULL = auto()
    t_IMM = auto()
    t_REG = auto()
    t_MEM = auto()


class ASMOperand:
    def __init__(self, type: OprandType, value: int):
        self.type: OprandType = type
        self.value: int = value


class ASMLine:
    def __init__(self, op: ASMOP, oprands: List[ASMOperand]):
        self.op: ASMOP = op
        self.oprands: List[ASMOperand] = oprands


ASMLines: TypeAlias = List[str]


class ASMBlock:
    def __init__(self, name: str, asmlines: ASMLines):
        self.name: str = name
        self.asmlines: ASMLines = asmlines


class ASMSection:
    def __init__(self, name: str, asmblocks: List[ASMBlock]):
        self.name: str = name
        self.asmblocks: List[ASMBlock] = asmblocks

ASMFile: TypeAlias = List[ASMSection]
