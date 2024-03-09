# 输出调试日志
import logging

logger = logging.getLogger()
# 文件保存
file_handler = logging.FileHandler("hello.log", "w", encoding="utf-8")
logger.addHandler(file_handler)
# 格式
formatter = logging.Formatter('%(asctime)s - %(levelname)s - %(message)s')
file_handler.setFormatter(formatter)
# 日志级别
logger.setLevel(logging.DEBUG)
logger.info("你好")