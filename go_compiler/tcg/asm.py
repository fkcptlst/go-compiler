from enum import Enum, auto
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
    def __init__(self, op: ASMOP, oprands: list[ASMOperand]):
        self.op: ASMOP = op
        self.oprands: list[ASMOperand] = oprands


ASMLines: TypeAlias = list[str]


class ASMBlock:
    def __init__(self):
        self.name: str = ""
        self.asmlines: ASMLines = ASMLines()


class ASMSection:
    def __init__(self):
        self.name: str = ""
        self.asmblocks: list[ASMBlock] = []


ASMFile: TypeAlias = list[ASMSection]
