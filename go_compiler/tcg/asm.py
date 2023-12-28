from enum import Enum, auto
from typing import TypeAlias


class ASMOP(Enum):
    MOV = 0
    PUSH = auto()
    POP = auto()
    CALL = auto()
    JMP = auto()
    RET = auto()


class OprandType(Enum):
    t_NULL = 0
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

    def __str__(self) -> str:
        return f"[{self.name}: {self.asmlines}]"

    def __repr__(self) -> str:
        return f"[{self.name}: {self.asmlines}]"


class ASMSection:
    def __init__(self):
        self.name: str = ""
        self.asmblocks: list[ASMBlock] = []

    def __str__(self) -> str:
        return f"[{self.name}: {self.asmblocks}]"

    def __repr__(self) -> str:
        return f"[{self.name}: {self.asmblocks}]"


ASMFile: TypeAlias = list[ASMSection]
