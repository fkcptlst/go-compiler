from abc import abstractmethod
from ...tcg.asm import ASMLines
from ...common.tac import TACLine
from ..SymbolManager import SymbolManager


class BaseTranslator:
    def SentenceTranslator(self, sm: SymbolManager, tac: TACLine):
        sm.set_scope(tac.scope)
        return self.SentenceTranslate_(sm, tac)

    @abstractmethod
    def SentenceTranslate_(
        self, SymbolManager_: SymbolManager, TACLine_: TACLine
    ) -> ASMLines:
        pass
