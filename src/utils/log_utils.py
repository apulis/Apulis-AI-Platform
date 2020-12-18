
import logging
import config

class Singleton(type):
    _instances = {}
    def __call__(cls, *args, **kwargs):
        if cls not in cls._instances:
            cls._instances[cls] = super(Singleton, cls).__call__(*args, **kwargs)
        return cls._instances[cls]

class LogManager(object):
    
    __metaclass__ = Singleton

    def __init__(self, level):
        self._level= level
        return

    def get_instance(self, filename):
        logger = logging.getLogger(filename)
        logger.setLevel(self._level)
        return logger


# log level: https://docs.python.org/zh-cn/3.7/library/logging.html#handler-objects
# CRITICAL 50
# ERROR    40
# WARNING  30
# INFO     20
# DEBUG    10 
# NOSET    0 
if "log_level" in config.config:
    logger = LogManager(int(config.config["log_level"]))
else:
    logger = LogManager(logging.WARNING)

def get_log_instance(filename):
    return logger.get_instance(filename)