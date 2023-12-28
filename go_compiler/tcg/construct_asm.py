from ..common.REG import REG, to_string
import pickle
from loguru import logger


def construct_asm(*args):
    op = args[0]

    if len(args) == 1:
        return "\t" + op
    if len(args) == 2:
        src = args[1]
        return f"\t{op}\t{format_reg_or_int(src)}"
    if len(args) == 3:
        dst = args[1]
        src = args[2]
        dst_str = format_reg_or_int(dst)
        src_str = format_reg_or_int(src)
        return f"\t{op}\t{dst_str}\t,\t{src_str}"
    if len(args) == 4:
        return f"\t{op}\t{construct_asm_mem(args[1], args[2], "dword")}\t,\t{format_reg_or_int(args[3])}"
    logger.error("Should NOT be here")
    return ""


def construct_memory_address(value):
    sign = "-" if value > 0 else "+"
    return f"[ ebp {sign} {abs(value)} ]"


def format_reg_or_int(value):
    if isinstance(value, int):
        return construct_memory_address(value)
    elif isinstance(value, REG):
        return to_string(value)
    return str(value)


def construct_asm_mem(reg: REG, offset: int, data_size: str = "") -> str:
    return (
        data_size
        + "[ "
        + f"{to_string(reg)}"
        + (" - " if offset > 0 else " + ")
        + str(abs(offset))
        + " ]"
    )
