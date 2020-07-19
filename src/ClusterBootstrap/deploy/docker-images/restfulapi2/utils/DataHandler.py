from config import config
from cache import fcache

if "datasource" in config and config["datasource"] == "MySQL":
    from MySQLNewPoolDataHandler import DataHandler
elif "datasource" in config and config["datasource"] == "MySQLPool":
    from MySQLPoolDataHandler import DataHandler
elif "datasource" in config and config["datasource"] == "MySQLDBUtilsPool":
    from MySQLDBUtilsPoolDataHandler import DataHandler
else:
    from SQLDataHandler import DataHandler


def exceptionWrapper(func):
    def inner(*args, **kwargs):
        dataHandler = DataHandler()
        ret = None
        try:
            fun = getattr(dataHandler,func.__name__)
            ret = fun(*args, **kwargs)
        finally:
            dataHandler.Close()
        return ret
    return inner

class DataManager:

    @staticmethod
    @exceptionWrapper
    def GetClusterStatus():
        pass


    @staticmethod
    @exceptionWrapper
    def ListVCs():
        pass

    
    @staticmethod
    @exceptionWrapper
    def GetResourceAcl(resourceAclPath):
        pass


    @staticmethod
    @exceptionWrapper
    def GetIdentityInfo(identityName):
        pass


    @staticmethod
    def GetAllPendingJobs(vcName):
        dataHandler = DataHandler()
        ret = None
        try:
            ret = dataHandler.GetJobList("all",vcName,None, "running,queued,scheduling,unapproved,pausing,paused", ("=","or"))
        finally:
            dataHandler.Close()
        return ret
    

    @staticmethod
    def GetTemplates(scope):
        dataHandler = DataHandler()
        ret = []
        try:
            ret = dataHandler.GetTemplates(scope)
        finally:
            dataHandler.Close()
        return ret

    @staticmethod
    @exceptionWrapper
    def UpdateTemplate(name, scope, json):
        pass

    @staticmethod
    @exceptionWrapper
    def DeleteTemplate(name, scope):
        pass
