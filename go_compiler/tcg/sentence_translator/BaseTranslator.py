from abc import abstractmethod, ABC
from ...tcg.asm import ASMLines
from ...common.tac import TACLine
from ..SymbolManager import SymbolManager


class BaseTranslator(ABC):
    def __init__(self) -> None:
        pass

    def SentenceTranslator(self, sm: SymbolManager, tac: TACLine) -> ASMLines:
        sm.set_scope(tac.scope)
        return self.SentenceTranslate_(sm, tac)

    @abstractmethod
    def SentenceTranslate_(
        self, SymbolManager_: SymbolManager, TACLine_: TACLine
    ) -> ASMLines:
        pass
