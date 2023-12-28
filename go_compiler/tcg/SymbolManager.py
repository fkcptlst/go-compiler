from enum import Enum, auto
import pickle
from ..common.common import *
from ..common.tac import TACOP, TACOPERANDTYPE, Operand, TACBlock
from ..common.Scope import Symbol, Scope
from ..common.REG import REG
from ..common.UseInfo import UseInfo
from loguru import logger


class POSTYPE(Enum):
    GLOBAL = 0
    REG = auto()
    MEM = auto()
    NONE = auto()


# 需要 push 的情况: {reg, val, false, -1}
# 需要更新内存的情况: {reg, val, false, 内存位置}
class RelacedEeg:
    def __init__(
        self, reg: REG = REG.NONE, val: str = "", no_use: bool = True, mem: int = -1
    ):
        self.reg: REG = reg  # 被替换的寄存器
        self.val: str = val  # 原本的变量
        self.no_use: bool = no_use  # 以后是否不再使用
        self.mem: int = (
            mem  # 如果该变量还要继续使用，且变量在内存中，则存储内存地址，否则为-1
        )

    def __str__(self) -> str:
        return f"{{{self.reg}, {self.val}, {self.no_use}, {self.mem}}}"

    def copy(self):
        return RelacedEeg(self.reg, self.val, self.no_use, self.mem)


class SymbolManager:
    def __init__(self, global_scope: Scope, name: str):
        # 符号表
        self.name_: str = name
        self.global_scope_: Scope = global_scope
        self.local_scope_: Scope | None = None
        self.func_: Symbol | None = global_scope.resolve(name=name)

        # 变量存储信息
        self.rvalue_: list[str] = [""] * (REG.NONE.value + 4)  # 地址-寄存器 -> 变量
        self.svalue_: list[str] = []  # 地址-内存 -> 变量
        self.avalue_reg_: dict[str, REG] = {}  # 变量 -> 地址-寄存器
        self.avalue_mem_: dict[str, int] = {}  # 变量 -> 地址-内存(存与ebp的偏移)

        # 变量名 -> 符号表中的 待用信息 和 活跃信息
        self.use_info_: dict[str, UseInfo] = {}

        # 函数堆栈模拟
        self.len_: int = 0  # 当前栈的长度 (4 字节)
        self.stack_esp_: int = 0  # 模拟堆栈的栈顶 (与当前函数栈底的相对值)
        self.para_num_: int = (
            0
            if self.func_.fun_para_type_list is None
            else len(self.func_.fun_para_type_list)
        )
        self.para_now_: int = 0
        self.ret_num_: int = (
            0
            if self.func_.fun_ret_type_list is None
            else len(self.func_.fun_ret_type_list)
        )
        self.ret_now_: int = 0

        # reset local scope

    def __repr__(self) -> str:
        return f"SymbolManager[{self.name_}]"

    def set_scope(self, local_scope: Scope):
        self.local_scope_ = local_scope

    # 编码变量 = scode名 + 变量名 (如果是数字则返回空串)
    def encode_var(self, var: str) -> str:
        en_var = ""
        if var and not var[0].isdigit():
            symbol: Symbol | None = self.local_scope_.resolve(var)
            en_var = f"{pickle.dumps(symbol.scope)}:{var}"
        return en_var

    # 计算一个代码块中, 变量的待用信息和活跃信息
    def cal_use_info(self, block: TACBlock):
        # 初始化符号表中所有变量的待用信息
        self.use_info_.clear()
        # 根据基本块出口来设置变量的活跃信息
        for line in reversed(block):
            self.set_scope(line.scope)
            if line.op == TACOP.FUN_RET:
                _ = UseInfo(0, True)
                self.set_use_info(line.src1.value, _)
            else:
                break
        # 从后往前遍历基本块，将 变量的待用信息和活跃信息 绑定在 TACline 上
        for line in reversed(block):
            self.set_scope(line.scope)
            if line.op == TACOP.CALL:
                continue
            if line.dst.OperType == TACOPERANDTYPE.VAR:
                # 把符号表中 dst 的待用信息和活跃信息 附加到 中间代码上
                line.dst.use_info = self.use_info(line.dst.value)
                # 重置符号表 dst 的待用信息和活跃信息
                _ = UseInfo(0, False)
                self.set_use_info(line.dst.value, _)
            if line.src1.OperType == TACOPERANDTYPE.VAR:
                # 把符号表中 src1 的待用信息和活跃信息 附加到 中间代码上
                line.src1.use_info = self.use_info(line.src1.value)
                # 置位符号表 src1 的待用信息和活跃信息
                _ = UseInfo(line.line, True)
                self.set_use_info(line.src1.value, _)
            if line.src2.OperType == TACOPERANDTYPE.VAR:
                # 把符号表中 src2 的待用信息和活跃信息 附加到 中间代码上
                line.src2.use_info = self.use_info(line.src2.value)
                # 置位符号表 src2 的待用信息和活跃信息
                _ = UseInfo(line.line, True)
                self.set_use_info(line.src2.value, _)

    # 查看 $1 的 use_info
    def use_info(self, variable: str, encoded: bool = False) -> UseInfo:
        var_en: str = variable if encoded else self.encode_var(variable)
        try:
            return self.use_info_[var_en]
        except KeyError:
            return UseInfo(0, False)

    # set $1 的 use_info 为 $2, 在翻译每个 TACline 时调用，用于替换选择
    def set_use_info(self, variable: str, use_info: UseInfo):
        var_en: str = self.encode_var(variable)
        if var_en:
            self.use_info_[var_en] = use_info

    # 得一个空闲寄存器 (如果没有返回 REG::None)
    def get_free_reg(self) -> REG:
        for i in range(REG.NONE.value):
            if self.rvalue_[i] == "":
                return REG(i)
        return REG.NONE

    # 当寄存器满的时候，找到一个将要替换的reg，要有替换策略

    def get_reg(self, dst, src1) -> REG:
        # 找到非活跃的寄存器
        if self.avalue_reg(dst) != REG.NONE:
            return self.avalue_reg(dst)
        if dst not in self.avalue_mem_:
            return REG.EDI
        if (src1 != "") and (self.avalue_reg(src1) != REG.NONE):
            return self.avalue_reg(src1)
        return self.get_free_reg()

    # 寻找一个将要替换的寄存器
    def get_replaced_reg(self) -> RelacedEeg:
        # 寻找 不再使用的变量
        for i in range(REG.NONE.value):
            replaced_val: str = self.rvalue_[i]
            if self.use_info_[replaced_val].no_use():
                return RelacedEeg(REG(i), replaced_val)
        for i in range(REG.NONE.value):
            replaced_val: str = self.rvalue_[i]
            val_mem: int = self.avalue_mem(replaced_val)
            if val_mem != -1:
                return RelacedEeg(REG(i), self.rvalue_[i], False, val_mem)
        # 返回需要 push 备份的寄存器
        max_next_use: int = 0
        max_next_use_reg: REG = None
        for i in range(REG.NONE.value):
            en_var: str = self.rvalue(REG(i))
            next_use_i: int = self.use_info(en_var, True).next_use
            if max_next_use < next_use_i:
                max_next_use = next_use_i
                max_next_use_reg = REG(i)
        return RelacedEeg(max_next_use_reg, self.rvalue(max_next_use_reg), False, -1)

    def set_zero_len(self):
        self.len_ = 0

    def get_stack_len(self) -> int:
        return self.len_

    def get_name(self) -> str:
        return self.name_

    def rvalue(self, reg: REG) -> str:
        return self.rvalue_[reg.value]

    def avalue_reg(self, variable: str) -> REG:
        try:
            return self.avalue_reg_[variable]
        except KeyError:
            return REG.NONE

    def avalue_mem(self, variable: str) -> int:
        try:
            return self.avalue_mem_[variable]
        except KeyError:
            return -1

    def para(self) -> int:
        bias: int = -4 * (
            self.para_num_ - self.para_now_ + 1
        )  # 栈底之前还有一个ret地址
        self.para_now_ += 1
        return bias

    def ret(self) -> int:
        bias: int = -4 * (self.ret_num_ + self.para_num_ - self.ret_now_ + 1)
        self.ret_now_ += 1
        return bias

    def resolve_fun(self, name) -> Symbol | None:
        p: Symbol | None = self.local_scope_.resolve(name)
        if p.is_fun():
            return p
        else:
            return None

    # 让一个变量存在 reg 里面
    def set_avalue_reg(self, variable: str, reg: REG):
        old: str = self.rvalue_[reg.value]
        if old in self.avalue_reg_ and self.avalue_reg_[old] == reg:
            del self.avalue_reg_[old]
        self.avalue_reg_[variable] = reg
        self.rvalue_[reg.value] = variable

    # 让一个变量存在堆栈里，用跟 ebp 的偏移
    def set_avalue_mem(self, variable: str, mem: int):
        old: str = self.svalue_[mem // 4]
        if old in self.avalue_mem_ and self.avalue_mem_[old] == mem:
            del self.avalue_mem_[old]
        self.avalue_mem_[variable] = mem
        self.svalue_[mem // 4] = variable

    # 打印 当前通用寄存器中所存的变量
    def show_reg(self, reg: REG):
        if reg != REG.NONE:
            logger.info(f"{reg.name}: {self.rvalue_[reg.value]}")
        else:
            for i in range(0, REG.NONE.value):
                en_var: str = self.rvalue_[i]
                try:
                    var_next_use: int = self.use_info_[en_var].next_use
                except KeyError:
                    var_next_use: int = 0
                logger.info(f"{REG(i).name}: {en_var}, {var_next_use}")

    # 打印 当前栈中所存的变量
    def show_mem(self, mem: int):
        logger.info(f"len of mem: {self.len_}")
        for key, val in self.avalue_mem_.items():
            logger.info(f"mem {val}: {key}")

    def position(self, variable: str) -> POSTYPE:
        pos = variable.find(":")
        scope_str = variable[:pos]
        ture_name = variable[pos + 1 :]
        scope_p: Scope = pickle.loads(eval(scope_str))

        if scope_p == self.global_scope_:
            return POSTYPE.GLOBAL
        elif variable in self.avalue_reg_:
            return POSTYPE.REG
        elif variable in self.avalue_mem_:
            return POSTYPE.MEM
        else:
            return POSTYPE.NONE

    # 模拟堆栈 push指令 把reg里面的变量放到mem里 并且让reg空
    # WARN: `overwrite = 0` 这个默认值可能有问题
    #       cpp 这个地方没有默认值，但是有的调用并没有给出该值
    def push_reg(self, reg: REG, overwrite: int = 1):
        if overwrite == 1:
            var: str = self.rvalue_[reg.value]
            self.svalue_.append(var)
            self.avalue_mem_[var] = self.stack_esp_
        else:
            self.svalue_.append("Null")
        self.stack_esp_ += 4
        self.len_ += 1

    # 模拟堆栈 pop指令 把栈顶的变量pop到reg里
    def pop_reg(self, reg: REG):
        var: str = self.svalue_[-1]
        self.svalue_.pop()
        old: str = self.rvalue_[reg.value]
        if old in self.avalue_reg_ and self.avalue_reg_[old] == reg:
            del self.avalue_reg_[old]
        if var != "Null":
            self.avalue_reg_[var] = reg
            self.rvalue_[reg.value] = var
        self.stack_esp_ -= 4
        self.len_ -= 1

    # 直接操作 栈顶 esp
    def set_esp_bias(self, bias: int):
        if bias > 0:
            num = bias // 4
            for i in range(num):
                self.svalue_.append("Null")
            self.len_ += num
        else:
            num = (-bias) // 4
            for i in range(num):
                old = self.svalue_[-1]
                if (
                    old in self.avalue_mem_
                    and self.avalue_mem_[old] == self.stack_esp_ - (i + 1) * 4
                ):
                    del self.avalue_mem_[old]
                self.svalue_.pop()
            self.len_ -= num
        self.stack_esp_ += bias

    def get_esp(self) -> int:
        return self.stack_esp_
