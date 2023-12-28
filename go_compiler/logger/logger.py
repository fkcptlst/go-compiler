import logging
from colorlog import ColoredFormatter


# ANSI 转义序列用于颜色
class MyColoredFormatter(ColoredFormatter):
    def __init__(self, *args, **kwargs):
        super(MyColoredFormatter, self).__init__(*args, **kwargs)

    levelname_aliases = {
        "DEBUG": "DBG",
        "INFO": "INF",
        "WARNING": "WRN",
        "ERROR": "ERR",
        "CRITICAL": "CRI",
    }

    def format(self, record):
        # 用 colorlog 渲染颜色
        formatted_record = super(MyColoredFormatter, self).format(record)
        levelname = self.levelname_aliases.get(record.levelname, record.levelname)
        # levelname = "{:^5}".format(levelname) # 居中对齐
        return formatted_record.replace(record.levelname, levelname)


# 设置日志格式和颜色
formatter = MyColoredFormatter(
    "%(log_color)s%(asctime)s [%(levelname)s] | %(filename)s:%(lineno)d | %(message)s%(reset)s",
    datefmt="%Y-%m-%d %H:%M:%S",
    reset=True,
    log_colors={
        "DEBUG": "cyan",
        "INFO": "green",
        "WARNING": "yellow",
        "ERROR": "red",
        "CRITICAL": "red,bg_white",
    },
    secondary_log_colors={},
    style="%",
)


# 创建一个日志记录器
logger = logging.getLogger()
logger.setLevel(logging.DEBUG)

# 创建一个用于控制台输出的处理器
console_handler = logging.StreamHandler()
console_handler.setLevel(logging.INFO)
console_handler.setFormatter(formatter)

# 创建一个用于文件输出的处理器，以追加模式打开
file_handler = logging.FileHandler("log.txt", mode="a")
file_handler.setLevel(logging.DEBUG)
file_handler.setFormatter(
    logging.Formatter(
        "%(asctime)s | %(levelname)s | %(filename)s:%(lineno)d | %(message)s",
        "%Y-%m-%d %H:%M:%S",
    )
)

# 将处理器添加到日志记录器
logger.addHandler(console_handler)
logger.addHandler(file_handler)
