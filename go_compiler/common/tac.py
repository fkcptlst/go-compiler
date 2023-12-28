from enum import Enum, auto
from typing import TypeAlias
from .UseInfo import UseInfo
from .Scope import Scope


class TACOP(Enum):
    ADD = auto()
    SUB = auto()
    MUL = auto()
    DIV = auto()
    ASSIGN = auto()
    CALL = auto()
    PARA = auto()
    RET = auto()
    ENDCALL = auto()
    FUN_RET = auto()
    FUN_PARA = auto()
    GOTO = auto()
    IFGT = auto()
    IFGE = auto()
    IFLT = auto()
    IFLE = auto()
    IFEQ = auto()
    IFNEQ = auto()
    LABEL = auto()
    CREATLIST = auto()


tacopmap = {
    TACOP.IFEQ: "je",
    TACOP.IFGE: "jge",
    TACOP.IFGT: "jg",
    TACOP.IFLE: "jle",
    TACOP.IFLT: "jl",
    TACOP.IFNEQ: "jne",
}


def to_string(op: TACOP) -> str:
    return op.name.lower()


class TACOPERANDTYPE(Enum):
    IMM = auto()
    VAR = auto()
    NULL_ = auto()
    PTR = auto()
    LABEL = auto()  # eg: ELSE0


class Operand:
    def __init__(self, value: str, type_: TACOPERANDTYPE):
        self.value: str = value
        self.use_info: UseInfo = UseInfo()
        self.OperType: TACOPERANDTYPE = type_

    def __str__(self) -> str:
        return f"(v:{self.value}, ui:{self.use_info}, ot:{self.OperType})"


class TACLine:
    def __init__(
        self,
        line: int,
        op: TACOP,
        src1: Operand,
        src2: Operand,
        dst: Operand,
        scope: Scope,
    ):
        self.line: int = line
        self.op: TACOP = op
        self.src1: Operand = src1
        self.src2: Operand = src2
        self.dst: Operand = dst
        self.scope: Scope = scope

    def __str__(self):
        return f"{self.line}:(op:{to_string(self.op)}, src1:{self.src1}, src2:{self.src2}, dst:{self.dst})"

    def __repr__(self):
        return self.__str__()


TACBlock: TypeAlias = list[TACLine]
TACFile: TypeAlias = dict[str, TACBlock]
