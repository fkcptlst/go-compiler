class UseInfo:
    def __init__(self, next_use: int = 0, active: bool = False):
        self.next_use: int = next_use  # 待用信息 (0 表示不再使用, 其他数表示下一次使用的位置)
        self.active: bool = active  # 活跃信息 (true 表示还要使用, false 表示不需要了)

    def copy(self):
        return UseInfo(self.next_use, self.active)

    def __str__(self):
        return f"({self.next_use},{'Y' if self.active else '^'})"

    def no_use(self) -> bool:
        return self.next_use == 0 and not self.active
