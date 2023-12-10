from ..common.common import DELIMITER


def ctx_encoder(values: list[str]) -> str:
    ret = ""
    for s in values:
        ret += s
        ret += DELIMITER
    return ret


def ctx_decoder(s: str) -> list[str]:
    return s.split(DELIMITER)[:-1]
