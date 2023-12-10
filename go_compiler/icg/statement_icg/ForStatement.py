from ...common.tac import TACLine


class ForStatement:
    def __init__(self, cur_index: str, *,
                 loop_con: TACLine = None,
                 update_con: TACLine = None,
                 new_paras: list[str] = None):
        self.cur_index: str = cur_index
        self.loop_con: TACLine = loop_con
        self.update_con: TACLine = update_con
        self.new_paras: list[str] = new_paras
