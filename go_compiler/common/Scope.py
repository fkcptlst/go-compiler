from enum import Enum, auto
from .common import SUCCESS, FAIL


class Symbol:
    class SymbolType(Enum):
        VAR = 0
        FUN = auto()

    class Type(Enum):
        # boolean
        BOOL = 0
        # number
        INT8 = auto()
        INT16 = auto()
        INT32 = auto()
        INT64 = auto()
        INT = auto()
        UINT8 = auto()
        UINT16 = auto()
        UINT32 = auto()
        UINT64 = auto()
        UINT = auto()
        FLOAT32 = auto()
        FLOAT64 = auto()
        COMPLEX64 = auto()
        COMPLEX128 = auto()
        BYTE = auto()  # equivalent to uint8
        RUNE = auto()  # equivalent to int32
        # string
        STRING = auto()

    def __init__(
        self,
        name: str,
        scope: "Scope",
        symbol_type: SymbolType,
        *,
        type_: Type = None,
        fun_ret_type_list: list[Type] = None,
        fun_para_type_list: list[Type] = None,
        is_array: bool = False,
        array_length: int = 0,
    ):
        self.name: str = name
        self.scope: Scope = scope
        self.symbol_type: Symbol.SymbolType = symbol_type

        self.type: Symbol.Type = type_

        self.fun_ret_type_list: list[Symbol.Type] = fun_ret_type_list
        self.fun_para_type_list: list[Symbol.Type] = fun_para_type_list

        self.is_array: bool = is_array
        self.array_length: int = array_length

    def is_var(self) -> bool:
        return self.symbol_type == Symbol.SymbolType.VAR

    def is_fun(self) -> bool:
        return self.symbol_type == Symbol.SymbolType.FUN

    @staticmethod
    def to_type(s: str) -> Type:
        if s == "int":
            return Symbol.Type.INT


class Scope:
    def __init__(self, enclosing_scope: "Scope" = None):
        self.enclosing_scope: Scope = enclosing_scope
        self.fun_symbols: dict[str, Symbol] = {}
        self.para_symbols: dict[str, Symbol] = {}

    def fun_define(self, sym: Symbol):
        self.fun_symbols[sym.name] = sym

    def para_define(self, sym: Symbol):
        self.para_symbols[sym.name] = sym

    def para_delete(self, name: str):
        del self.para_symbols[name]

    def resolve(self, name: str) -> Symbol | None:
        if name in self.fun_symbols:
            return self.fun_symbols[name]
        elif name in self.para_symbols:
            return self.para_symbols[name]
        elif self.enclosing_scope:  # check enclosing scope
            return self.enclosing_scope.resolve(name)
        else:
            return None

    def cur_resolve(self, name: str) -> int:
        if name in self.fun_symbols:
            return SUCCESS
        elif name in self.para_symbols:
            return SUCCESS
        else:
            return FAIL
