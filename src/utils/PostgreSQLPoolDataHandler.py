import json
import base64
import os
import logging
import functools
import timeit
import time
from itertools import chain
from Queue import Queue
from DBUtils.PooledDB import PooledDB
from config import config
from config import global_vars
import requests
import psycopg2
from datetime import datetime
from prometheus_client import Histogram
import threading
from postgresql_conn_pool import PostgresqlConn,db_connect_histogram
import EndpointUtils
from psycopg2.extensions import ISOLATION_LEVEL_AUTOCOMMIT

PostgresqlConn.config_pool(risk_config={"max_connections":5})

logger = logging.getLogger(__name__)

data_handler_fn_histogram = Histogram("datahandler_fn_latency_seconds",
                                      "latency for executing data handler function (seconds)",
                                      buckets=(.05, .075, .1, .25, .5, .75, 1.0, 2.5, 5.0,
                                               7.5, 10.0, 12.5, 15.0, 17.5, 20.0, float("inf")),
                                      labelnames=("fn_name",))

# db_connect_histogram = Histogram("db_connect_latency_seconds",
#                                  "latency for connecting to db (seconds)",
#                                  buckets=(.05, .075, .1, .25, .5, .75, 1.0, 2.5, 5.0, 7.5, float("inf")))


def parse_endpoints(endpoints):
    if len(endpoints) == 1:
        endpoint = endpoints[0]
        ep = EndpointUtils.parse_endpoint(endpoint)
        if ep["status"]=="running":
            return "http://%s.%s/endpoints/v2/%s/v1/models/%s:predict" % (ep["nodeName"],ep["domain"],ep["port"],ep["modelname"])

def sql_injection_parse(name):
    if name.strip() in ["%","_"]:
        return " "
    return name.strip()



class SingletonDBPool(object):
    __instance_lock = threading.Lock()

    def __init__(self, *args, **kwargs):
        logger.info("init postgreSQL DBUtils pool start")
        database = "DLWSCluster-%s" % config["clusterId"]
        server = config["postgresql"]["hostname"]
        username = config["postgresql"]["username"]
        password = config["postgresql"]["password"]
        with db_connect_histogram.time():
            self.pool = PooledDB(creator=psycopg2, maxconnections=150, blocking=False,
                                 host=server, database=database, user=username, password=password)

            logger.info("init postgreSQL DBUtils pool succeed")

    @classmethod
    def instance(cls, *args, **kwargs):
        if not hasattr(SingletonDBPool, "_instance"):
            with SingletonDBPool.__instance_lock:
                if not hasattr(SingletonDBPool, "_instance"):
                    SingletonDBPool._instance = SingletonDBPool(*args, **kwargs)
        return SingletonDBPool._instance

    def get_connection(self):
        return self.pool.connection()


def record(fn):
    @functools.wraps(fn)
    def wrapped(*args, **kwargs):
        start = timeit.default_timer()
        try:
            return fn(*args, **kwargs)
        except Exception as e:
            logger.exception('PostgreSQL Exception: %s', str(e))
        finally:
            elapsed = timeit.default_timer() - start
            ##logger.info("DataHandler: %s, time elapsed %.2fs", fn.__name__, elapsed)
            data_handler_fn_histogram.labels(fn.__name__).observe(elapsed)

    return wrapped


class DataHandler(object):
    def __init__(self):
        start_time = timeit.default_timer()
        self.database = "DLWSCluster-%s" % config["clusterId"]
        self.accounttablename = "account"
        self.jobtablename = "jobs"
        self.inferencejobtablename = "inferencejobs"
        self.modelconversionjobtablename = "modelconversionjobs"
        self.dataconvert = "dataconvert"
        self.fdserverinfotablename = "fdserverinfo"
        self.identitytablename = "identity"
        self.acltablename = "acl"
        self.vctablename = "vc"
        self.storagetablename = "storage"
        self.clusterstatustablename = "clusterstatus"
        self.commandtablename = "commands"
        self.templatetablename = "templates"
        self.jobprioritytablename = "job_priorities"
        self.deviceStatusTableName = "devicestatus"
        self.monitorConfigTableName = "monitorconfig"
        self.monitormetricsTableName = "monitormetrics"
        self.monitorchannelTableName = "monitorchannel"
        self.pool = SingletonDBPool.instance()

        elapsed = timeit.default_timer() - start_time
        ##logger.info("DB Utils DataHandler initialization, time elapsed %f s", elapsed)
        self.CreateDatabase()
        self.CreateTable()


    def CreateDatabase(self):
        if "initSQLDB" not in global_vars or not global_vars["initSQLDB"]:
            logger.info("===========init SQL database===============")
            global_vars["initSQLDB"] = True

            server = config["postgresql"]["hostname"]
            username = config["postgresql"]["username"]
            password = config["postgresql"]["password"]

            conn = psycopg2.connect(user=username, password=password,
                                          host=server, sslmode='disable')

            cursor = conn.cursor()
            cursor.execute("SELECT 1 FROM pg_catalog.pg_database WHERE datname = '%s'" % self.database)
            exists = cursor.fetchone()
            if not exists:
                conn.set_isolation_level(ISOLATION_LEVEL_AUTOCOMMIT)
                cursor.execute("""CREATE DATABASE "{}" """.format(self.database))

                conn.commit()
            cursor.close()
            conn.close()

    def CreateTable(self):
        if "initSQLTable" not in global_vars or not global_vars["initSQLTable"]:
            logger.info("===========init SQL Tables ===============")
            global_vars["initSQLTable"] = True

            database = "DLWSCluster-%s" % config["clusterId"]
            server = config["postgresql"]["hostname"]
            username = config["postgresql"]["username"]
            password = config["postgresql"]["password"]
            conn = psycopg2.connect(host=server, database=database, user=username, password=password)
            cursor = conn.cursor()

            sql = """
                CREATE TABLE  "%s"
                (
                    "uid" serial primary key,
                    "openId" varchar(64) NOT NULL,
                    "group" varchar(64) NOT NULL,
                    "nickName" varchar(64) NOT NULL,
                    "userName" varchar(64) UNIQUE NOT NULL,
                    "password" varchar(64) NOT NULL,
                    "phoneNumber" varchar(64) DEFAULT NULL,
                    "email" varchar(64) DEFAULT NULL,
                    "isAdmin" int NOT NULL,
                    "isAuthorized" int NOT NULL,
                    "time" timestamp NOT NULL DEFAULT now()
                );
                alter table "%s" add constraint "unique-%s-openId-group" unique("openId","group");
                """ % (self.accounttablename,self.accounttablename,self.accounttablename)

            try:
                cursor.execute(sql)
                conn.commit()
            except:
                logger.info("Table %s already exists" %self.accounttablename)


            sql = """
                CREATE TABLE  "%s"
                (
                    "id"                    serial primary key,
                    "jobId"                 varchar(50) UNIQUE  NOT NULL,
                    "familyToken"           varchar(50)   NOT NULL,
                    "isParent"              INT  NOT NULL,
                    "jobName"               varchar(1024) NOT NULL,
                    "userName"              varchar(255) NOT NULL,
                    "vcName"                varchar(255) NOT NULL,
                    "jobStatus"             varchar(255) NOT NULL DEFAULT 'unapproved',
                    "jobStatusDetail"       TEXT  NULL,
                    "jobType"               varchar(255) NOT NULL,
                    "jobDescriptionPath"    TEXT NULL,
                    "jobDescription"        TEXT  NULL,
                    "jobTime"               timestamp  without time zone DEFAULT now() NOT NULL,
                    "endpoints"             TEXT  NULL,
                    "errorMsg"              TEXT  NULL,
                    "jobParams"             TEXT  NOT NULL,
                    "jobMeta"               TEXT  NULL,
                    "jobLog"                TEXT  NULL,
                    "retries"               int   NULL DEFAULT 0,
                    "isDeleted"             int    NULL DEFAULT 0,
                    "lastUpdated"           timestamp  without time zone DEFAULT now() NOT NULL,
                    "jobGroup"              varchar(255) NULL
                );
                CREATE INDEX "index-%s-userName" ON "%s" USING btree ("userName");
                CREATE INDEX "index-%s-jobTime" ON "%s" USING btree ("jobTime");
                CREATE INDEX "index-%s-jobId" ON "%s" USING btree ("jobId");
                CREATE INDEX "index-%s-vcName" ON "%s" USING btree ("vcName");
                CREATE INDEX "index-%s-jobStatus" ON "%s" USING btree ("jobStatus");
                CREATE INDEX "index-%s-jobType" ON "%s" USING btree ("jobType");
                CREATE INDEX "index-%s-jobGroup" ON "%s" USING btree ("jobGroup");
                """ % (self.jobtablename,self.jobtablename,self.jobtablename,
                self.jobtablename,self.jobtablename,
                self.jobtablename,self.jobtablename,
                self.jobtablename,self.jobtablename,
                self.jobtablename,self.jobtablename,
                self.jobtablename,self.jobtablename,
                self.jobtablename,self.jobtablename
                )

            try:
                cursor.execute(sql)
                conn.commit()
            except:
                logger.info("Table %s already exists" %self.jobtablename)


            sql = """
                CREATE TABLE   "%s"
                (
                    "id"      serial primary key,
                    "jobId"   varchar(50) UNIQUE  NOT NULL,
                    "time"    timestamp DEFAULT now() NOT NULL
                )
                """ % (self.inferencejobtablename)
            try:
                cursor.execute(sql)
                conn.commit()
            except:
                logger.info("Table %s already exists" %self.inferencejobtablename)


            sql = """
                CREATE TABLE   "%s"
                (
                    "id"         serial primary key,
                    "jobId"      varchar(50) UNIQUE NOT NULL,
                    "inputPath"  TEXT NOT NULL,
                    "outputPath" TEXT NOT NULL,
                    "type"       varchar(255) NOT NULL,
                    "status"     varchar(255) NOT NULL,
                    "fileId"     varchar(255)
                );
                CREATE INDEX "index-%s-jobId" ON "%s" USING btree ("jobId");
                CREATE INDEX "index-%s-type" ON "%s" USING btree (type);
                CREATE INDEX "index-%s-status" ON "%s" USING btree (status);
                """ % (self.modelconversionjobtablename,self.modelconversionjobtablename
                ,self.modelconversionjobtablename,self.modelconversionjobtablename
                ,self.modelconversionjobtablename,self.modelconversionjobtablename,
                self.modelconversionjobtablename)
            try:

                cursor.execute(sql)
                conn.commit()
            except:
                logger.info("Table %s already exists" %self.modelconversionjobtablename)


            sql = """
                CREATE TABLE   "%s"
                (
                    "id"         serial primary key,
                    "name"       varchar(50)   NOT NULL UNIQUE,
                    "username"   varchar(50)   NOT NULL,
                    "password"   varchar(50)   NOT NULL,
                    "url"        varchar(255)  NOT NULL
                )
                """ % (self.fdserverinfotablename)
            try:
                cursor.execute(sql)
                conn.commit()
            except:
                logger.info("Table %s already exists" %self.fdserverinfotablename)

            sql = """
                CREATE TABLE  "%s"
                (
                    "id"        serial primary key,
                    "status"    TEXT NOT NULL,
                    "time"      timestamp DEFAULT now() NOT NULL
                );

                CREATE INDEX "index-%s-time" ON "%s" USING btree (time);
                """ % (self.clusterstatustablename,self.clusterstatustablename,self.clusterstatustablename)
            try:
                cursor.execute("ROLLBACK")
                conn.commit()
                cursor.execute(sql)
                conn.commit()
            except:
                logger.info("Table %s already exists" %self.clusterstatustablename)

            sql = """
                CREATE TABLE  "%s"
                (
                    "id"        serial primary key,   
                    "jobId"     varchar(50)   NOT NULL,
                    "status"    varchar(255) NOT NULL DEFAULT 'pending',
                    "time"      timestamp without time zone DEFAULT now() NOT NULL,
                    "command"   TEXT NOT NULL,
                    "output"    TEXT NULL
                );
                """ % (self.commandtablename)

            try:
                cursor.execute(sql)
                conn.commit()
            except:
                logger.info("Table %s already exists" %self.commandtablename)

            sql = """
                CREATE TABLE   "%s"
                (
                    "id"               serial primary key, 
                    "storageType"      varchar(255) NOT NULL,
                    "url"              varchar(255) NOT NULL,
                    "metadata"         TEXT NOT NULL,
                    "vcName"           varchar(255) NOT NULL,
                    "defaultMountPath" varchar(255) NOT NULL,
                    "time"             timestamp without time zone DEFAULT now() NOT NULL
                )
                ;
                alter table "%s" add constraint "unique-%s-vcName-url" unique("vcName","url");
                alter table "%s" add constraint "unique-%s-vcName-defaultMountPath" unique("vcName","defaultMountPath");
                """ % (self.storagetablename,self.storagetablename,self.storagetablename,
                self.storagetablename,self.storagetablename)

            try:
                cursor.execute(sql)
                conn.commit()
            except:
                logger.info("Table %s already exists" %self.storagetablename)


            # when the VC has vm of same GPU type but different VMsizes, e.g., when VC has Standard_NC6s_v3 and Standard_NC12s_v3 both?
            # impossible since there's no way to do it with current config mechanism

            gpu_count_per_node = config["gpu_count_per_node"]
            worker_node_num = config["worker_node_num"]
            gpu_type = config["gpu_type"]
            default_type = json.dumps({one:0 for one in config["defalt_virtual_cluster_device_type_list"] }  if config["defalt_virtual_cluster_device_type_list"] else {})
            logging.info([config['defalt_virtual_cluster_name'],default_type])
            sql = """
                CREATE TABLE   "%s"
                (
                    "id"        serial primary key,
                    "vcName"    varchar(255) UNIQUE NOT NULL,
                    "parent"    varchar(255) DEFAULT NULL,
                    "quota"     varchar(255) NOT NULL,
                    "metadata"  TEXT NOT NULL,
                    "time"      timestamp without time zone DEFAULT now() NOT NULL
                )
                ;
                INSERT into vc ("vcName","parent","quota","metadata") VALUES ('%s',NULL,'%s','{}')
                """ % (self.vctablename, config['defalt_virtual_cluster_name'],default_type)
            try:
                cursor.execute(sql)
                conn.commit()
            except:
                logger.info("Table %s already exists" %self.vctablename)

            sql = """
                CREATE TABLE   "%s"
                (
                    "id"            serial primary key,
                    "identityName"  varchar(255) NOT NULL UNIQUE,
                    "uid"           INT NOT NULL,
                    "gid"           INT NOT NULL,
                    "groups"        TEXT NOT NULL,
                    "time"          timestamp  without time zone DEFAULT now() NOT NULL
                )
                """ % (self.identitytablename)

            try:
                cursor.execute(sql)
                conn.commit()
            except:
                logger.info("Table %s already exists" %self.identitytablename)


            sql = """
                CREATE TABLE   "%s"
                (
                    "id"             serial primary key,
                    "identityName"   varchar(255) NOT NULL,
                    "identityId"     INT NOT NULL,
                    "resource"       varchar(255) NOT NULL,
                    "permissions"    INT NOT NULL,
                    "isDeny"         INT NOT NULL,
                    "time"           timestamp without time zone  DEFAULT now() NOT NULL
                );
                alter table "%s" add constraint "identityName_resource" unique("identityName","resource");
                """ % (self.acltablename,self.acltablename)
            try:
                cursor.execute(sql)
                conn.commit()
            except:
                logger.info("Table %s already exists" %self.acltablename)

            sql = """
                CREATE TABLE  "%s"
                (
                    "id"    serial primary key,
                    "name"  VARCHAR(255) NOT NULL,
                    "scope" VARCHAR(255) NOT NULL ,
                    "json"  TEXT         NOT NULL,
                    "isDefault"  smallint  DEFAULT 0,
                    "time"  timestamp  without time zone DEFAULT now() NOT NULL
                );
                alter table "%s" add constraint "name_scope" unique("name", "scope");
                """ % (self.templatetablename,self.templatetablename)

            try:
                cursor.execute(sql)
                conn.commit()

                default_job_json={
                    "workPath": "",
                    "interactivePorts": "",
                    "name": "Default Job",
                    "gpuType": "",
                    "workers": 0,
                    "jobPath": "",
                    "dataPath": "",
                    "gpuNumPerDevice": 0,
                    "enableWorkPath": True,
                    "preemptible": False,
                    "enableJobPath": True,
                    "command": " while true; do echo \"job running\"; sleep 1; done",
                    "environmentVariables": [],
                    "tensorboard": False,
                    "plugins": {
                        "blobfuse": [
                            {
                                "accountKey": "",
                                "containerName": "",
                                "mountPath": "",
                                "mountOptions": "",
                                "accountName": ""
                            }
                        ],
                        "imagePull": [
                            {
                                "username": "",
                                "password": "",
                                "registry": ""
                            }
                        ]
                    },
                    "ipython": True,
                    "gpus": 1,
                    "type": "RegularJob",
                    "image": "ubuntu:18.04",
                    "enableDataPath": True,
                    "ssh": True
                }

                sql = """
                    insert into %s (
                        id, name, scope, json, "isDefault"
                    )
                    values(
                        0, 'default_template', 'vc:platform', '%s', 1
                    ) ON CONFLICT (id) DO UPDATE SET  id=0
                """ %(self.templatetablename, json.dumps(default_job_json))

                try:
                    cursor.execute(sql)
                    conn.commit()
                except:
                    logger.error("Table %s insert fall" %self.templatetablename)


            except:
                logger.info("Table %s already exists" %self.templatetablename)


            sql = """
                CREATE TABLE   "%s"
                (
                    "id"        serial primary key,
                    "jobId"     varchar(50)   NOT NULL,
                    "priority"  INT NOT NULL,
                    "time"      timestamp  without time zone DEFAULT now() NOT NULL
                );
                alter table "%s" add constraint "identityName_jobId" unique("jobId");
                """ % (self.jobprioritytablename,self.jobprioritytablename)

            try:
                cursor.execute(sql)
                conn.commit()
            except:
                logger.info("Table %s already exists" %self.jobprioritytablename)

            sql = """
                    CREATE TABLE   "%s"
                    (
                        "id"            serial primary key,
                        "deviceType"    varchar(50) UNIQUE  NOT NULL,
                        "deviceStr"     varchar(50)   NOT NULL,
                        "capacity"      INT NOT NULL,
                        "detail"        TEXT NOT NULL,
                        "time"          timestamp  without time zone DEFAULT now() NOT NULL
                    )
                    """ % (self.deviceStatusTableName)
            try:
                cursor.execute(sql)
                conn.commit()
            except:
                logger.info("Table %s already exists" %self.deviceStatusTableName)
            sql = """
                    CREATE TABLE   "%s"
                    (
                        "id"                serial primary key,
                        "configuration"     TEXT   NOT NULL,
                        "time"              timestamp without time zone  DEFAULT now() NOT NULL
                    )
                    """ % (self.monitorConfigTableName)
            try:
                cursor.execute(sql)
                conn.commit()
            except:
                logger.info("Table %s already exists" %self.monitorConfigTableName)

            sql = """
                    CREATE TABLE   "%s"
                    (
                        "id"            serial primary key,
                        "name"          varchar(50)   NOT NULL,
                        "query"         varchar(255)   NOT NULL,
                        "time"          timestamp  without time zone DEFAULT now() NOT NULL
                    )
                    """ % (self.monitormetricsTableName)
            try:
                cursor.execute(sql)
                conn.commit()
            except:
                logger.info("Table %s already exists" %self.monitormetricsTableName)
            sql = """
                    CREATE TABLE   "%s"
                    (
                        "id"            serial primary key,
                        "name"          varchar(50)   NOT NULL,
                        "fields"        TEXT      NOT NULL,
                        "time"          timestamp  without time zone DEFAULT now() NOT NULL
                    )
                    """ % (self.monitorchannelTableName)
            try:
                cursor.execute(sql)
                conn.commit()
            except:
                logger.info("Table %s already exists" %self.monitorchannelTableName)

            sql = """
                CREATE TABLE   "%s"
                (
                    "id"            serial primary key,
                    "projectId"     varchar(50)   NOT NULL,
                    "datasetId"     varchar(50)   NOT NULL,
                    "targetFormat"  varchar(50)   NOT NULL,
                    "type"          varchar(255) NOT NULL,
                    "outPath"       varchar(255) NULL,
                    "status"        varchar(255) NOT NULL DEFAULT 'queued',
                    "errorMsg"      TEXT  NULL,
                    "time"          timestamp  without time zone DEFAULT now() NOT NULL
                );
                CREATE INDEX "index-%s-projectId" ON "%s" USING btree ("projectId");
                CREATE INDEX "index-%s-datasetId" ON "%s" USING btree ("datasetId");
                CREATE INDEX "index-%s-status" ON "%s" USING btree ("status");
                """ % (self.dataconvert,self.dataconvert,self.dataconvert,self.dataconvert,self.dataconvert,self.dataconvert,self.dataconvert)

            try:
                cursor.execute(sql)
                conn.commit()
            except:
                logger.info("Table %s already exists" %self.dataconvert)


    @record
    def AddDevice(self,deviceType, deviceStr, capacity,detail):
        #TODO
        ret = False
        try:
            sql = """
            INSERT INTO "%s" 
            ("deviceType", "deviceStr", capacity,detail) 
            VALUES ('%s','%s','%s','%s')
            ON CONFLICT ("deviceType") DO UPDATE SET "deviceStr"='%s',capacity='%s',detail='%s'
            """%(self.deviceStatusTableName,deviceType,deviceStr, capacity,json.dumps(detail), deviceStr, capacity,json.dumps(detail))

            with PostgresqlConn() as conn:
                conn.insert_one(sql)
                conn.commit()
            ret = True
        except Exception as e:
            logger.exception('AddStorage Exception: %s', str(e))
        return ret


    @record
    def GetAllDevice(self):
        ret = {}
        try:
            sql = """select "deviceType", "deviceStr", capacity,detail from %s  """ %(self.deviceStatusTableName)
            with PostgresqlConn() as conn:
                logger.info(sql)
                rets = conn.select_many(sql)
            for one in rets:
                ret[one["deviceType"]] = {"deviceStr":one["deviceStr"],"capacity":one["capacity"],"detail":json.loads(one["detail"])}
        except Exception as e:
            logger.exception('AddStorage Exception: %s', str(e))
        return ret
    @record
    def DeleteDeviceType(self,deviceType):
        ret = True
        try:
            sql = """DELETE FROM "%s" where "deviceType"= %s """ % (self.deviceStatusTableName,"%s")
            with PostgresqlConn() as conn:
                logger.info(sql,[deviceType])
                conn.insert_one(sql,[deviceType])
                conn.commit()
        except Exception as e:
            logger.exception('AddStorage Exception: %s', str(e))
            ret = False
        return ret

    @record
    def CountJobByStatus(self,vcName,status=None):
        sql  = """select count(1) from "%s" where "vcName"=%s""" % (self.jobtablename,"%s")
        params = [vcName]
        if status is not None:
            if "," not in status:
                sql += """ and "jobStatus" = %s"""
                params.append(status)
            else:
                sql += """ and "jobStatus" in %s"""
                params.append([s for s in status.split(",")])
        with PostgresqlConn() as conn:
            logger.info(sql,params)
            ret = conn.select_one_value(sql, params)
        return ret

    @record
    def AddStorage(self, vcName, url, storageType, metadata, defaultMountPath):
        ret = False
        try:
            sql = "INSERT INTO " + self.storagetablename + " (storageType, url, metadata, vcName, defaultMountPath) VALUES (%s,%s,%s,%s,%s)"
            with PostgresqlConn() as conn:
                logger.info(sql,(storageType, url, metadata, vcName, defaultMountPath))
                conn.insert_one(sql,(storageType, url, metadata, vcName, defaultMountPath))
                conn.commit()
            ret = True
        except Exception as e:
            logger.exception('AddStorage Exception: %s', str(e))
        return ret

    @record
    def DeleteStorage(self, vcName, url):
        ret = False
        try:
            sql = "DELETE FROM %s WHERE url = %s and vcName = %s" % (self.storagetablename,"%s","%s")
            with PostgresqlConn() as conn:
                logger.info(sql,[url, vcName])
                conn.insert_one(sql,[ url, vcName])
                conn.commit()
            ret = True
        except Exception as e:
            logger.exception('DeleteStorage Exception: %s', str(e))
        return ret

    @record
    def ListStorages(self, vcName):
        ret = []
        try:
            query = "SELECT storageType,url,metadata,vcName,defaultMountPath FROM %s WHERE vcName = %s " % (
            self.storagetablename,"%s")
            with PostgresqlConn() as conn:
                logger.info(query,[vcName])
                rets = conn.select_many(query,[vcName])
            for one in rets:
                ret.append(one)
        except Exception as e:
            logger.exception('ListStorages Exception: %s', str(e))
        return ret

    @record
    def UpdateStorage(self, vcName, url, storageType, metadata, defaultMountPath):
        ret = False
        try:
            sql = """update %s set storageType = %s, metadata = %s, defaultMountPath = %s where vcName = %s and url = %s """ % (
            self.storagetablename,"%s","%s","%s","%s","%s")
            with PostgresqlConn() as conn:
                logger.info(sql,[storageType, metadata, defaultMountPath, vcName, url])
                conn.update(sql,[storageType, metadata, defaultMountPath, vcName, url])
                conn.commit()
            ret = True
        except Exception as e:
            logger.exception('UpdateStorage Exception: %s', str(e))
        return ret

    @record
    def AddVC(self, vcName, quota, metadata):
        ret = False
        try:
            sql = "INSERT INTO " + self.vctablename + " (vcName, quota, metadata) VALUES (%s,%s,%s)"
            with PostgresqlConn() as conn:
                logger.info(sql, (vcName, quota, metadata))
                conn.insert_one(sql, (vcName, quota, metadata))
                conn.commit()
            ret = True
        except Exception as e:
            logger.exception('AddVC Exception: %s', str(e))
        return ret

    @record
    def GetVC(self, vcName):

        try:
            query = """SELECT "vcName",quota,metadata FROM %s""" % (self.vctablename)

            if vcName:
                query += """ WHERE "vcName" = '%s' """ %(vcName)
            else:
                pass

            with PostgresqlConn() as conn:
                logger.info(query)
                rets = conn.select_many(query)

            for one in rets:
                return one

        except Exception as e:
            logger.exception('GetVC Exception: %s', str(e))

        return None
    @record
    def ListVCs(self,page=None,size=None,name=None):
        ret = []
        try:
            query = """SELECT "vcName",quota,metadata FROM %s """% (self.vctablename)
            if name:
                query += """ WHERE "vcName" like '%%%s%%' """ %(name)
            if page and size:
                query += " limit %d offset %d" % (int(size),(int(page)-1)*int(size))
            with PostgresqlConn() as conn:
                logger.info(query)
                rets = conn.select_many(query)
            for one in rets:
                ret.append(one)
        except Exception as e:
            logger.exception('ListVCs Exception: %s', str(e))
        return ret
    @record
    def CountVCs(self,name=None):
        ret = None
        try:
            query = "SELECT count(1) FROM %s" % (self.vctablename)
            if name:
                query += """ WHERE "vcName" like '%%%s%%' """ %(name)
            with PostgresqlConn() as conn:
                logger.info(query)
                ret = conn.select_one_value(query)
        except Exception as e:
            logger.exception('ListVCs Exception: %s', str(e))
        return ret

    @record
    def DeleteVC(self, vcName):
        ret = False
        try:
            sql = """DELETE FROM %s WHERE "vcName" = %s""" % (self.vctablename, "%s")
            with PostgresqlConn() as conn:
                logger.info(sql,[vcName])
                conn.insert_one(sql,[vcName])
                conn.commit()
            ret = True
        except Exception as e:
            logger.exception('DeleteVC Exception: %s', str(e))
        return ret

    @record
    def UpdateVC(self, vcName, quota, metadata):
        ret = False
        try:
            sql = """update %s set quota = %s, metadata = %s where "vcName" = %s """ % (
            self.vctablename,"%s","%s","%s")
            with PostgresqlConn() as conn:
                logger.info(sql,[ quota, metadata, vcName])
                conn.update(sql,[ quota, metadata, vcName])
                conn.commit()
            ret = True
        except Exception as e:
            logger.exception('UpdateVC Exception: %s', str(e))
        return ret

    @record
    def ListUser(self):
        query = """SELECT uid,"openId", "group" ,"nickName","userName",password,"isAdmin","isAuthorized" FROM %s """ % (self.accounttablename)
        ret = []
        try:
            with PostgresqlConn() as conn:
                logger.info(query)
                rets = conn.select_many(query)
            for one in rets:
                ret.append(one)
        except Exception as e:
            logger.exception('ListUser Exception: %s', str(e))
        return ret

    @record
    def DeleteUser(self,userName):
        query = """Delete FROM %s where "userName"=%s""" % (self.accounttablename,"%s")
        ret = False
        try:
            with PostgresqlConn() as conn:
                logger.info(query,[userName])
                conn.insert_one(query,[userName])
                conn.commit()
                ret = True
        except Exception as e:
            logger.exception('DeleteUser Exception: %s', str(e))
        return ret

    @record
    def GetAccountByopenId(self, openId, group):
        query = """SELECT uid,"openId", "group","nickName","userName",password,"isAdmin","isAuthorized",email,"phoneNumber" FROM %s where "openId" = %s and "group" = %s""" % (self.accounttablename,"%s","%s" )
        ret = []
        try:
            with PostgresqlConn() as conn:
                logger.info(query,[openId, group])
                rets = conn.select_many(query,[openId, group])
            for one in rets:
                ret.append(one)

        except Exception as e:
            logger.exception('GetAccountByopenId Exception: %s', str(e))
        return ret

    @record
    def GetAccountByopenIdAndPassword(self, openId,group,password):
        query = """SELECT uid,"openId","group","nickName","userName",password,"isAdmin","isAuthorized",email,"phoneNumber" FROM %s where "openId" = %s and group = %s and password=%s""" % (self.accounttablename,"%s","%s","%s")
        ret = []
        try:
            with PostgresqlConn() as conn:
                logger.info(query,[openId, group,password])
                rets = conn.select_many(query,[openId, group,password])
            for one in rets:
                ret.append(one)

        except Exception as e:
            logger.exception('GetAccountByopenId Exception: %s', str(e))
        return ret

    @record
    def GetAccountByUserName(self, userName):
        query = """SELECT uid,"openId","group","nickName","userName",password,"isAdmin","isAuthorized" FROM %s where "userName" = %s """ % (self.accounttablename,"%s")
        ret = []
        try:
            with PostgresqlConn() as conn:
                logger.info(query,[userName])
                rets = conn.select_many(query,[userName])
            for one in rets:
                ret.append(one)
        except Exception as e:
            logger.exception('GetAccountByUserName Exception: %s', str(e))
        return ret

    @record
    def UpdateAccountInfo(self, openId, group, nickName, userName, password, isAdmin, isAuthorized):
        try:
            if len(self.GetAccountByopenId(openId, group)) == 0:
                sql = """INSERT INTO "+self.accounttablename+" ("openId", "group", "nickName", "userName", password, "isAdmin", "isAuthorized") VALUES (%s,%s,%s,%s,%s,%s,%s)"""
                with PostgresqlConn() as conn:
                    logger.info(sql, (openId, group, nickName, userName, password, isAdmin, isAuthorized))
                    conn.insert_one(sql, (openId, group, nickName, userName, password, isAdmin, isAuthorized))
                    conn.commit()
            else:
                sql = """update %s set "nickName" = %s, "userName" = %s, "password" = %s, "isAdmin" = %s, "isAuthorized" = %s where "openId" = %s and "group" = %s"""
                with PostgresqlConn() as conn:
                    logger.info(sql % (self.accounttablename,"%s","%s","%s","%s","%s","%s","%s"),[nickName, userName, password, isAdmin, isAuthorized, openId, group])
                    conn.insert_one(sql % (self.accounttablename,"%s","%s","%s","%s","%s","%s","%s"),[nickName, userName, password, isAdmin, isAuthorized, openId, group])
                    conn.commit()
            return True
        except Exception as e:
            logger.exception('UpdateIdentityInfo Exception: %s', str(e))
            return False

    @record
    def UpdateEmailAndPhone(self,openId, group,email,phone):
        try:
            if len(self.GetAccountByopenId(openId, group)) == 0:
                return False
            else:
                sql = """update %s set "email" = %s, "phoneNumber" = %s where "openId" = %s and "group" = %s""" % (self.accounttablename,"%s","%s","%s","%s")
                with PostgresqlConn() as conn:
                    logger.info(sql,[email,phone,openId, group])
                    conn.insert_one(sql,[email,phone,openId, group])
                    conn.commit()
            return True
        except Exception as e:
            logger.exception('UpdateIdentityInfo Exception: %s', str(e))
            return False

    @record
    def UpdateAccountPermission(self,userName, isAdmin,isAuthorized):
        try:
            sql = """update %s set "isAdmin" = %s, "isAuthorized" = %s where "userName" = %s""" % (self.accounttablename,"%s","%s","%s")
            with PostgresqlConn() as conn:
                logger.info(sql,[int(isAdmin),int(isAuthorized),userName])
                conn.insert_one(sql,[int(isAdmin),int(isAuthorized),userName])
                conn.commit()
            return True
        except Exception as e:
            logger.exception('UpdateIdentityInfo Exception: %s', str(e))
            return False

    @record
    def GetIdentityInfo(self, identityName):
        ret = []
        try:
            # query = "SELECT identityName,uid,gid,groups FROM %s where identityName = %s" % (
            # self.identitytablename, "%s")
            # with PostgresqlConn() as conn:
            #     rets = conn.select_many(query,[identityName])
            # for one in rets:
            #     one["groups"] = json.loads(one["groups"])
            #     ret.append(one)
            res = requests.get(url=config["usermanagerapi"] + "/open/getUserIdByUserName/"+identityName,headers={"Authorization":"Bearer "+config["usermanagerapitoken"]})
            if res.status_code==200:
                ret.append({"identityName":identityName,"uid":json.loads(res.content.decode("utf-8"))["uid"],"gid":30001,"groups":[30001]})
        except Exception as e:
            logger.exception('GetIdentityInfo Exception: %s', str(e))
        return ret

    @record
    def UpdateIdentityInfo(self, identityName, uid, gid, groups):
        ret = False
        try:
            if (isinstance(groups, list)):
                groups = json.dumps(groups)
            if len(self.GetIdentityInfo(identityName)) == 0:
                sql = """insert into {0} ("identityName", uid, gid, groups) values ('{1}', '{2}', '{3}', '{4}') on CONFLICT (id)  DO UPDATE SET uid='{2}', gid='{3}', groups='{4}'""".format(
                    self.identitytablename, identityName, uid, gid, groups)
                with PostgresqlConn() as conn:
                    logger.info(sql)
                    conn.insert_one(sql)
                    conn.commit()
            else:
                sql = """update %s set uid = %s, gid = %s, groups = %s where "identityName" = %s """ % (
                self.identitytablename, "%s", "%s", "%s", "%s")
                with PostgresqlConn() as conn:
                    logger.info(sql,[uid, gid, groups, identityName])
                    conn.insert_one(sql,[uid, gid, groups, identityName])
                    conn.commit()
            ret = True
        except Exception as e:
            logger.exception('UpdateIdentityInfo Exception: %s', str(e))
        return ret

    @record
    def UpdateIdentityInfoPerm(self, identityName, isAdmin, isAuthorized):
        try:
            sql = """update %s set "isAdmin" = %s, "isAuthorized" = %s where "userName" = %s """ % (self.accounttablename, "%s", "%s", "%s")
            with PostgresqlConn() as conn:
                logger.info(sql,[isAdmin, isAuthorized, identityName])
                conn.update(sql,[isAdmin, isAuthorized, identityName])
                conn.commit()
            return True
        except Exception as e:
            logger.exception('UpdateIdentityInfo Exception: %s', str(e))
            return False


    @record
    def GetAceCount(self, identityName, resource):
        query = """SELECT count(ALL id) as c FROM %s where "identityName" = %s and resource = %s""" % (self.acltablename,"%s", "%s")
        with PostgresqlConn() as conn:
            logger.info(query,[identityName, resource])
            rets = conn.select_many(query,[identityName, resource])
        ret = 0
        for c in rets:
            ret = c["c"]
        return ret

    @record
    def UpdateAce(self, identityName, identityId, resource, permissions, isDeny):
        ret = False
        try:
            existingAceCount = self.GetAceCount(identityName, resource)
            if existingAceCount == 0:
                sql = """insert into {0} ("identityName", "identityId", resource, permissions, "isDeny") values ('{1}', '{2}', '{3}', '{4}', '{5}') on duplicate key update permissions='{4}'""".format(
                    self.acltablename, identityName, identityId, resource, permissions, isDeny)
                with PostgresqlConn() as conn:
                    logger.info(sql)
                    conn.insert_one(sql)
                    conn.commit()
            else:
                sql = """update %s set permissions = %s where "identityName" = %s and resource = %s """ % (
                self.acltablename, "%s", "%s", "%s")
                with PostgresqlConn() as conn:
                    logger.info(sql,[permissions, identityName, resource])
                    conn.insert_one(sql,[permissions, identityName, resource])
                    conn.commit()
            ret = True
        except Exception as e:
            logger.exception('UpdateAce Exception: %s', str(e))
        return ret

    @record
    def UpdateAclIdentityId(self, identityName, identityId):
        ret = False
        try:
            sql = """update %s set "identityId" = %s where "identityName" = %s """ % (
            self.acltablename, "%s", "%s")
            with PostgresqlConn() as conn:
                logger.info(sql,[identityId, identityName])
                conn.update(sql,[identityId, identityName])
                conn.commit()
            ret = True
        except Exception as e:
            logger.exception('UpdateAclIdentityId Exception: %s', str(e))
        return ret

    @record
    def DeleteResourceAcl(self, resource):
        ret = False
        try:
            sql = "DELETE FROM %s WHERE resource = %s" % (self.acltablename, "%s")
            with PostgresqlConn() as conn:
                logger.info(sql,[resource])
                conn.insert_one(sql,[resource])
                conn.commit()
            ret = True
        except Exception as e:
            logger.exception('DeleteResourceAcl Exception: %s', str(e))
        return ret

    @record
    def DeleteAce(self, identityName, resource):
        ret = False
        try:
            sql = """DELETE FROM %s WHERE "identityName" = %s and resource = %s""" % (
            self.acltablename, "%s", "%s")
            with PostgresqlConn() as conn:
                logger.info(sql,[identityName, resource])
                conn.insert_one(sql,[identityName, resource])
                conn.commit()
            ret = True
        except Exception as e:
            logger.exception('DeleteAce Exception: %s', str(e))
        return ret

    @record
    def GetAcl(self):
        ret = []
        try:
            query = """SELECT "identityName","identityId",resource,permissions,"isDeny" FROM %s""" % (
                self.acltablename)
            with PostgresqlConn() as conn:
                logger.info(query)
                rets = conn.select_many(query)
            for one in rets:
                ret.append(one)
        except Exception as e:
            logger.exception('GetAcl Exception: %s', str(e))
        return ret

    @record
    def GetResourceAcl(self, resource):
        ret = []
        try:
            query = """SELECT "identityName","identityId",resource,permissions,"isDeny" FROM %s where resource = %s""" % (
            self.acltablename, "%s")
            with PostgresqlConn() as conn:
                logger.info(query,[resource])
                rets = conn.select_many(query,[resource])
            for one in rets:
                ret.append(one)
        except Exception as e:
            logger.exception('GetResourceAcl Exception: %s', str(e))
        return ret

    @record
    def AddJob(self, jobParams):
        ret = False
        try:
            sql = """INSERT INTO %s ("jobId", "familyToken", "isParent", "jobName", "userName", "vcName", "jobType","jobParams","jobGroup") VALUES (%s,%s,%s,%s,%s,%s,%s,%s,%s)""" % (self.jobtablename,"%s","%s","%s","%s","%s","%s","%s","%s","%s")
            jobParam = base64.b64encode(json.dumps(jobParams))
            with PostgresqlConn() as conn:
                conn.insert_one(sql, (
                    jobParams["jobId"], jobParams["familyToken"], jobParams["isParent"], jobParams["jobName"],
                    jobParams["userName"], jobParams["vcName"], jobParams["jobType"], jobParam,jobParams.get("jobGroup",None)))
                conn.commit()
            ret = True
        except Exception as e:
            logger.exception('AddJob Exception: %s', str(e))
        return ret

    @record
    def AddInferenceJob(self, jobParams):
        ret = False
        try:
            sql = """INSERT INTO %s ("jobId") VALUES (%s)""" % (self.inferencejobtablename,"%s")
            jobParam = base64.b64encode(json.dumps(jobParams))
            with PostgresqlConn() as conn:
                logger.info(sql, (
                    jobParams["jobId"],))
                conn.insert_one(sql, (
                    jobParams["jobId"],))
                conn.commit()
            ret = True
        except Exception as e:
            logger.exception('AddJob Exception: %s', str(e))
        return ret

    ### Model Convert && FD Push functions
    @record
    def AddModelConversionJob(self, jobParams):
        ret = False
        try:
            sql = """INSERT INTO %s (jobId, inputPath, outputPath, type, status) VALUES (%s, %s, %s, %s, %s)""" % (self.modelconversionjobtablename,"%s", "%s", "%s", "%s", "%s")
            jobParam = base64.b64encode(json.dumps(jobParams))
            with PostgresqlConn() as conn:
                logger.info(sql, (
                    jobParams["jobId"], jobParams["inputPath"], jobParams["outputPath"], jobParams["conversionType"], "converting"))
                conn.insert_one(sql, (
                    jobParams["jobId"], jobParams["inputPath"], jobParams["outputPath"], jobParams["conversionType"], "converting"))
                conn.commit()
            ret = True
        except Exception as e:
            logger.exception('AddJob Exception: %s', str(e))
        return ret

    @record
    def UpdateModelConversionFileId(self, jobId, fileId):
        ret = False
        try:
            sql = """UPDATE %s SET "fileId"='%s' WHERE "jobID"='%s'""" % (self.modelconversionjobtablename, fileId, jobId)
            with PostgresqlConn() as conn:
                logger.info(sql)
                conn.update(sql)
                conn.commit()
            ret = True
        except Exception as e:
            logger.exception('Update modelconvert fileId failed: %s', str(e))
        return ret

    @record
    def UpdateModelConversionStatus(self, jobId, status):
        ret = False
        try:
            sql = """UPDATE %s SET status='%s' WHERE "jobID"='%s'""" % (self.modelconversionjobtablename, status, jobId)
            with PostgresqlConn() as conn:
                logger.info(sql)
                conn.update(sql)
                conn.commit()
            ret = True
        except Exception as e:
            logger.exception('Update modelconvert status failed: %s', str(e))
        return ret

    @record
    def SetFDInfo(self, params):
        ret = False
        try:
            name = "default"
            sql = ""
            if self.GetFDInfo() is None:
                sql = """INSERT INTO %s (userName, password, url, name) VALUES (%s, %s, %s, %s)"""  %(self.fdserverinfotablename,"%s", "%s", "%s", "%s")
            else:
                sql = """UPDATE %s SET userName=%s, password=%s, url=%s WHERE name=%s""" % (self.fdserverinfotablename,"%s", "%s", "%s", "%s")
            with PostgresqlConn() as conn:
                logger.info(sql, (
                    params["userName"], params["password"], params["url"], name
                ))

                conn.insert_one(sql, (
                    params["userName"], params["password"], params["url"], name
                ))
                conn.commit()
            ret = True
        except Exception as e:
            logger.exception('SetFDInfo Exception: %s', str(e))
        return ret

    @record
    def GetFDInfo(self):
        ret = None
        try:
            name = "default"
            query = """SELECT name, "userName", password, url FROM %s where name='%s'""" % (
                self.fdserverinfotablename, name
            )
            with PostgresqlConn() as conn:
                logger.info(query)
                ret = conn.select_one(query)
        except Exception as e:
            logger.exception('GetFDInfo Exception: %s', str(e))
        return ret

    @record
    def GetModelConvertInfo(self, jobId):
        ret = None
        try:
            query = """SELECT "jobId", "inputPath", "outputPath", type, status, "fileId" FROM %s where "jobId"='%s'""" % (
                self.modelconversionjobtablename, jobId
            )
            with PostgresqlConn() as conn:
                logger.info(query)
                ret = conn.select_one(query)
        except Exception as e:
            logger.exception("Get ModelConvert info exception: %s", str(e))
        return ret

    @record
    def GetModelConvertInfoByOutputpath(self, outputpath):
        ret = None
        try:
            query = """SELECT "jobId", inputPath", "outputPath", type, status, "fileId" FROM %s where "outputPath"='%s' ORDER BY id DESC""" % (
                self.modelconversionjobtablename, outputpath
            )
            with PostgresqlConn() as conn:
                logger.info(query)
                rets = conn.select_many(query)
                for ret in rets:
                    fileId = ret["fileId"]
                    if fileId is not None and fileId not in ["", "NULL", "None", "null"]:
                        return ret
                return None
        except Exception as e:
            logger.exception("Get ModelConvert info exception: %s", str(e))
        return ret

    @record
    def GetJobList(self, userName, vcName, num=None, pageSize=None, pageNum=None, jobName=None, status=None, op=("=", "or")):
        ret = []
        try:
            query = """SELECT "jobId","jobName","userName", "vcName", "jobStatus", "jobStatusDetail", "jobType", "jobDescriptionPath", "jobDescription", "jobTime", endpoints, "jobParams","errorMsg" ,"jobMeta", "lastUpdated" ,"jobGroup" FROM %s where "isDeleted"=0  """ % (
                self.jobtablename)
            params = []
            if jobName != None:
                query += """ and "jobName" = %s"""
                params.append(jobName)
            if userName != "all":
                query += """ and "userName" = %s"""
                params.append(userName)
            if vcName != "all":
                query += """ and "vcName" = %s"""
                params.append(vcName)
            if status is not None:
                if "," not in status:
                    query += """ and "jobStatus" %s %s""" % (op[0], "%s")
                    params.append(status)
                else:
                    status_list = [""" "jobStatus" %s %s """ % (op[0], "%s") for _ in status.split(',')]
                    for s in status.split(','):
                        params.append(s)
                    status_statement = (" " + op[1] + " ").join(status_list)
                    query += " and ( %s ) " % status_statement

            query += """ order by "jobTime" Desc"""

            if num is not None:
                query += " limit %s " % str(num)
            elif pageNum != None and pageSize != None:
                offset = (pageNum - 1)*pageSize
                query += " limit %s offset %s " % (str(pageSize), str(offset))
            with PostgresqlConn() as conn:
                logger.info(query,params)
                rets = conn.select_many(query,params)
            fetch_start_time = timeit.default_timer()
            fetch_elapsed = timeit.default_timer() - fetch_start_time
            ##logger.info("(fetchall time: %f)", fetch_elapsed)
            for one in rets:
                ret.append(one)
        except Exception as e:
            logger.exception('GetJobList Exception: %s', str(e))
        return ret

    @record
    def GetInferenceJobList(self, userName, vcName, num=None, status=None, op=("=", "or")):
        ret = []
        try:
            query = """SELECT "jobId","jobName","userName", "vcName", "jobStatus", "jobStatusDetail", "jobType", "jobDescriptionPath", "jobDescription", "jobTime", endpoints, "jobParams","errorMsg" ,"jobMeta" FROM %s where true""" % (
                self.inferencejobtablename)
            params = []
            if userName != "all":
                query += """ and "userName" = %s"""
                params.append(userName)
            if vcName != "all":
                query += """ and "vcName" = %s"""
                params.append(vcName)
            if status is not None:
                if "," not in status:
                    query += """ and "jobStatus" %s %s"""% (op[0], "%s")
                    params.append(status)
                else:
                    status_list = [""" "jobStatus" %s %s """ % (op[0], "%s") for _ in status.split(',')]
                    for s in status.split(','):
                        params.append(s)
                    status_statement = (" " + op[1] + " ").join(status_list)
                    query += " and ( %s ) " % status_statement

            query += """ order by "jobTime" Desc"""

            if num is not None:
                query += " limit %s " % str(num)
            with PostgresqlConn() as conn:
                logger.info(query,params)
                rets = conn.select_many(query, params)
            fetch_start_time = timeit.default_timer()
            fetch_elapsed = timeit.default_timer() - fetch_start_time
            logger.info("(fetchall time: %f)", fetch_elapsed)
            for one in rets:
                ret.append(one)
        except Exception as e:
            logger.exception('GetJobList Exception: %s', str(e))
        return ret

    @record
    def GetJobListV2(self, userName, vcName, num=None, status=None, op=("=", "or")):
        ret = {}
        ret["queuedjobs"] = []
        ret["runningJobs"] = []
        ret["finishedJobs"] = []
        ret["visualizationJobs"] = []

        conn = None
        cursor = None
        try:
            conn = self.pool.get_connection()
            cursor = conn.cursor()

            query = """SELECT {}."jobId", "jobName", "userName", "vcName", "jobStatus", "jobStatusDetail", "jobType", "jobTime", "jobParams", "priority" FROM {} left join {} on {}."jobId" = {}."jobId" where "isDeleted"=0""".format(
                self.jobtablename, self.jobtablename, self.jobprioritytablename, self.jobtablename,self.jobprioritytablename)
            if userName != "all":
                query += """ and "userName" = '%s'""" % userName

            if vcName != "all":
                query += """ and "vcName" = '%s'""" % vcName

            if status is not None:
                if "," not in status:
                    query += """ and "jobStatus" %s '%s'""" % (op[0], status)
                else:
                    status_list = [""" "jobStatus" %s '%s' """ % (op[0], s) for s in status.split(',')]
                    status_statement = (" " + op[1] + " ").join(status_list)
                    query += " and ( %s ) " % status_statement

            query += """ order by "jobTime" Desc"""

            if num is not None:
                query += " limit %s " % str(num)
            logger.info(query)
            cursor.execute(query)

            columns = [column[0] for column in cursor.description]
            data = cursor.fetchall()
            for item in data:
                record = dict(zip(columns, item))
                if record["jobStatusDetail"] is not None:
                    record["jobStatusDetail"] = self.load_json(base64.b64decode(record["jobStatusDetail"]))
                if record["jobParams"] is not None:
                    record["jobParams"] = self.load_json(base64.b64decode(record["jobParams"]))

                if record["jobStatus"] == "running":
                    if record["jobType"] == "training" or record["jobType"] == "codeEnv":
                        ret["runningJobs"].append(record)
                    elif record["jobType"] == "visualization":
                        ret["visualizationJobs"].append(record)
                elif record["jobStatus"] == "queued" or record["jobStatus"] == "scheduling" or record[
                    "jobStatus"] == "unapproved":
                    ret["queuedjobs"].append(record)
                else:
                    ret["finishedJobs"].append(record)
            conn.commit()
        except Exception as e:
            logger.exception('GetJobListV2 Exception: %s', str(e))
        finally:
            if cursor is not None:
                cursor.close()
            if conn is not None:
                conn.close()

        ret["meta"] = {"queuedjobs": len(ret["queuedjobs"]), "runningJobs": len(ret["runningJobs"]),
                       "finishedJobs": len(ret["finishedJobs"]), "visualizationJobs": len(ret["visualizationJobs"])}
        return ret

    @record
    def GetJobListV3(self, userName, vcName, jobType, jobStatus,jobGroup,pageNum,
            pageSize, searchWord, orderBy, order, status=None, op=("=", "or")):

        ret = {}
        ret["queuedjobs"] = []
        ret["runningJobs"] = []
        ret["finishedJobs"] = []
        ret["visualizationJobs"] = []
        ret["allJobs"] = []

        conn = None
        cursor = None

        try:
            conn = self.pool.get_connection()
            cursor = conn.cursor()

            query = """SELECT count(*) OVER() AS total, "jobId", "jobName", "userName", "vcName", "jobStatus", "jobStatusDetail",
                        "jobType", "jobTime", "jobParams", "errorMsg" FROM {}  
                        where "jobType"='{}' and "isDeleted"=0""".format(self.jobtablename, jobType)
            ## add support for jobGroup filter :
            if jobGroup and int(jobGroup) > 0:
                query += """ and "jobGroup"='{}'""".format(jobGroup)


            ## all jobs
            if jobGroup and jobStatus.lower() != "all":
                query += """ and "jobStatus" = '%s'""" % jobStatus
            else:
                pass

            query += """ and "userName" = '%s'""" % userName

            if vcName is not None and vcName != "":
                query += """ and "vcName" = '%s'""" % vcName

            if searchWord is not None and len(searchWord) > 0:
                query += """ and "jobName" like '%"""
                query += "%s" % (sql_injection_parse(searchWord))
                query += "%'"
            else:
                pass

            if status is not None:

                if "," not in status:
                    query += """ and "jobStatus" %s '%s'""" % (op[0], status)
                else:
                    status_list = [""" "jobStatus" %s '%s' """ % (op[0], s) for s in status.split(',')]
                    status_statement = (" " + op[1] + " ").join(status_list)
                    query += " and ( %s ) " % status_statement

            else:
                pass

            if order != "asc":
                order = "desc"

            if orderBy is None or orderBy == "":
                query += """ order by "jobTime" Desc"""
            else:
                query += """order by "%s" %s""" % (orderBy, order)

            #query += " order by jobTime Desc"
            if pageNum is not None and pageSize is not None:
                query += " limit %d OFFSET %d " % (int(pageSize), (int(pageNum) - 1) * int(pageSize))
            else:
                pass

            logger.info(query)
            cursor.execute(query)

            columns = [column[0] for column in cursor.description]
            data = cursor.fetchall()
            result = [[str(item) for item in row] for row in data]

            total = 0


            for item in result:

                record = dict(zip(columns, item))

                if record["jobStatusDetail"] is not None:
                    record["jobStatusDetail"] = self.load_json(base64.b64decode(record["jobStatusDetail"]))

                else:
                    pass

                if record["jobParams"] is not None:
                    record["jobParams"] = self.load_json(base64.b64decode(record["jobParams"]))

                if record["jobStatus"] == "running":

                    if record["jobType"] == "visualization":
                        ret["visualizationJobs"].append(record)
                    else:
                        pass

                    ret["allJobs"].append(record)

                else:
                    ret["finishedJobs"].append(record)
                    ret["allJobs"].append(record)

                ## get total count
                if total == 0 and record["total"] is not None:
                    total = record["total"]
                else:
                    pass

            conn.commit()

        except Exception as e:
            logger.exception('GetJobListV3 Exception: %s', str(e))

        finally:
            if cursor is not None:
                cursor.close()
            if conn is not None:
                conn.close()

        ret["meta"] = {"queuedjobs": len(ret["queuedjobs"]),
                       "runningJobs": len(ret["runningJobs"]),
                       "finishedJobs": len(ret["finishedJobs"]),
                       "visualizationJobs": len(ret["visualizationJobs"]),
                       "totalJobs": int(total)}

        return ret


    @record
    def GetJobCount(self, vcName, jobType, jobStatus, searchWord):
        total = 0
        try:
            conn = self.pool.get_connection()
            cursor = conn.cursor()

            query = """SELECT count(*) AS total FROM {} where "isDeleted"=0""".format(self.jobtablename)

            if jobType.lower() != "all" and len(jobType) > 0:
                query += """ and "jobType" = '%s'""" % jobType

            ## all jobs
            if jobStatus.lower() != "all" and len(jobStatus) > 0:
                query += """ and "jobStatus" = '%s'""" % jobStatus
            else:
                pass

            if vcName is not None and vcName != "":
                query += """ and "vcName" = '%s' """ % vcName
            else:
                pass

            if searchWord is not None and len(searchWord) > 0:
                query += """ and ("jobName" like '%"""
                query += "%s" % (sql_injection_parse(searchWord))
                query += "%'"
                query += """ or "userName" like '%"""
                query += "%s" % (sql_injection_parse(searchWord))
                query += "%'"
                query += ") "
            else:
                pass

            logger.info(query)
            cursor.execute(query)
            total = cursor.fetchone()[0]

            conn.commit()

        except Exception as e:
            logger.exception('GetJobCount Exception: %s', str(e))

        finally:
            if cursor is not None:
                cursor.close()
            if conn is not None:
                conn.close()

        return total

    @record
    def GetAllJobList(self, vcName, jobType, jobStatus, pageNum,
            pageSize, searchWord, orderBy, order, status=None, op=("=", "or")):
        jobs = []

        conn = None
        cursor = None

        logger.info("test")
        logger.info(vcName)
        logger.info(jobType)
        logger.info(jobStatus)
        try:
            conn = self.pool.get_connection()
            cursor = conn.cursor()

            query = """SELECT {}."jobId", "jobName", "userName", "vcName", "jobStatus", "jobStatusDetail",
                        "jobType", "jobTime", "jobParams", priority FROM {} left join {}
                        on {}."jobId" =  {}."jobId" where "isDeleted"=0""".format(self.jobtablename,
                        self.jobtablename, self.jobprioritytablename,
                        self.jobtablename, self.jobprioritytablename)

            if jobType.lower() != "all" and len(jobType) > 0:
                query += """ and "jobType" = '%s'""" % jobType

            ## all jobs
            if jobStatus.lower() != "all" and len(jobStatus) > 0:
                query += """ and "jobStatus" = '%s'""" % jobStatus
            else:
                pass

            if vcName is not None and len(vcName) > 0:
                query += """ and "vcName" = '%s'  """ % vcName

            if searchWord is not None and len(searchWord) > 0:
                query += """ and ("jobName" like '%"""
                query += "%s" % (sql_injection_parse(searchWord))
                query += "%'"
                query += """ or "userName" like '%"""
                query += "%s" % (sql_injection_parse(searchWord))
                query += "%'"
                query += ") "
            else:
                pass

            if order != "asc":
                order = "desc"

            if orderBy is None or orderBy == "":
                query += """ order by "jobTime" Desc"""
            else:
                query += """ order by %s %s""" % (orderBy, order)

            if pageNum is not None and pageSize is not None:
                query += " limit %d offset %d " % (int(pageSize),(int(pageNum)-1)*int(pageSize))
            else:
                pass
            logger.info(query)
            cursor.execute(query)
            data = cursor.fetchall()

            columns = [column[0] for column in cursor.description]
            for item in data:
                record = dict(zip(columns, item))

                if record["jobStatusDetail"] is not None:
                    record["jobStatusDetail"] = self.load_json(base64.b64decode(record["jobStatusDetail"]))
                else:
                    pass

                if record["jobParams"] is not None:
                    record["jobParams"] = self.load_json(base64.b64decode(record["jobParams"]))

                jobs.append(record)

            conn.commit()

        except Exception as e:
            logger.exception('GetAllJobList Exception: %s', str(e))

        finally:
            if cursor is not None:
                cursor.close()
            if conn is not None:
                conn.close()

        return jobs


    @record
    def GetUserJobs(self, userName, vcName, jobStatus):

        # vcName: multiple vcs are separated by comman
        # jobStatus: multiple jobTypes are separated by comman
        ret = []
        conn = None
        cursor = None

        try:
            conn = self.pool.get_connection()
            cursor = conn.cursor()

            query = """SELECT "jobName", "jobId" FROM {} where """.format(self.jobtablename)
            if "," not in vcName:
                query += """ "vcName"="%s" """ % (vcName)
            else:
                query += """ "vcName" in (%s) """ % (','.join(['"'+s+'"' for s in vcName.split(",")]))

            if "," not in jobStatus:
                query += """and "jobStatus"="%s" """ % (jobStatus)
            else:
                query += """and "jobStatus" in (%s) """ % (','.join(['"'+s+'"' for s in jobStatus.split(",")]))

            if "," not in userName:
                query += """and "userName"="%s" """ % (userName)
            else:
                query += """and "userName" in (%s) """ % (','.join(['"'+s+'"' for s in userName.split(",")]))

            query += """ and "isDeleted"=0 """
            logger.info("GetUserJobs, sql: %s" %(query))
            cursor.execute(query)

            columns = [column[0] for column in cursor.description]
            data = cursor.fetchall()

            for item in data:
                logger.info("GetUserJobs, item: %s" %(str(item)))
                ret.append({"jobName": item[0], "jobId": item[1]})

            conn.commit()

        except Exception as e:
            logger.exception('GetUserJobs Exception: %s', str(e))

        finally:

            if cursor is not None:
                cursor.close()
            if conn is not None:
                conn.close()

        return ret


    @record
    def ListInferenceJob(self, userName, vcName, num=None, status=None, op=("=", "or"),jobName=None,order=None,orderBy=None):
        ret = {}
        ret["queuedjobs"] = []
        ret["runningJobs"] = []
        ret["finishedJobs"] = []
        ret["visualizationJobs"] = []

        conn = None
        cursor = None

        try:
            conn = self.pool.get_connection()
            cursor = conn.cursor()

            query = """SELECT {}."jobId", "jobName", "userName", "vcName", "jobStatus", "jobStatusDetail", "jobType", "jobTime", "jobParams", priority,endpoints FROM {} left join {} on {}."jobId" = {}."jobId" left join {} on {}."jobId" = {}."jobId" where "isDeleted"=0""".format(
                self.jobtablename,self.inferencejobtablename,self.jobtablename,self.inferencejobtablename, self.jobtablename, self.jobprioritytablename, self.jobtablename,
                self.jobprioritytablename)
            if userName != "all":
                query += """ and "userName" = '%s'""" % userName

            if vcName != "all":
                query += """ and "vcName" = '%s'""" % vcName

            if jobName:
                query += """ and "jobName" like '%%%s%%'""" % sql_injection_parse(jobName)

            if status:
                if "," not in status:
                    query += """ and "jobStatus" %s '%s'""" % (op[0], status)
                else:
                    status_list = [""" "jobStatus" %s '%s' """ % (op[0], s) for s in status.split(',')]
                    status_statement = (" " + op[1] + " ").join(status_list)
                    query += " and ( %s ) " % status_statement

            if not orderBy:
                orderBy = "jobTime"
            if order:
                if order not in ["desc", "asc"]:
                    order = "desc"
            else:
                order = "desc"
            if orderBy:
                query += """ order by "%s" %s""" %(orderBy,order)

            if num is not None:
                query += " limit %s " % str(num)
            cursor.execute(query)
            logger.info(query)

            columns = [column[0] for column in cursor.description]
            data = cursor.fetchall()
            for item in data:
                record = dict(zip(columns, item))
                if record["jobStatusDetail"] is not None:
                    record["jobStatusDetail"] = self.load_json(base64.b64decode(record["jobStatusDetail"]))
                if record["jobParams"] is not None:
                    record["jobParams"] = self.load_json(base64.b64decode(record["jobParams"]))

                endpoints = record["endpoints"]
                if endpoints:
                    endpoints = json.loads(record["endpoints"]).values()
                    record["inference-url"] = parse_endpoints(endpoints)

                if record["jobStatus"] == "running":
                    if record["jobType"] == "InferenceJob":
                        ret["runningJobs"].append(record)
                    elif record["jobType"] == "visualization":
                        ret["visualizationJobs"].append(record)
                elif record["jobStatus"] == "queued" or record["jobStatus"] == "scheduling" or record[
                    "jobStatus"] == "unapproved":
                    ret["queuedjobs"].append(record)
                else:
                    ret["finishedJobs"].append(record)
            conn.commit()
        except Exception as e:
            logger.exception('ListInferenceJob Exception: %s', str(e))
        finally:
            if cursor is not None:
                cursor.close()
            if conn is not None:
                conn.close()

        ret["meta"] = {"queuedjobs": len(ret["queuedjobs"]), "runningJobs": len(ret["runningJobs"]),
                       "finishedJobs": len(ret["finishedJobs"]), "visualizationJobs": len(ret["visualizationJobs"])}
        return ret

    @record
    def ListInferenceJobV2(self, userName, vcName, num=None, status=None, op=("=", "or"),jobName=None,order=None,orderBy=None):
        ret = []
        try:

            query = """SELECT {}."jobId", "jobName", "userName", "vcName", "jobStatus", "jobStatusDetail", "jobType", "jobTime", "jobParams", priority,endpoints FROM {} left join {} on {}."jobId" = {}."jobId" left join {} on {}."jobId" = {}."jobId" where "isDeleted"=0""".format(
                self.jobtablename,self.inferencejobtablename,self.jobtablename,self.inferencejobtablename, self.jobtablename, self.jobprioritytablename, self.jobtablename,
                self.jobprioritytablename)
            params = []
            if userName != "all":
                query += """ and "userName" = %s""" % "%s"
                params.append(userName)

            if vcName != "all":
                query += """ and "vcName" = %s""" % "%s"
                params.append(vcName)

            if jobName:
                query += """ and "jobName" like %s""" % "%s"
                params.append("%%%s%%" % sql_injection_parse(jobName))

            if status:
                if "," not in status:
                    query += """ and "jobStatus" %s %s""" % (op[0], "%s")
                    params.append(status)
                else:
                    status_list = [""" "jobStatus" %s %s""" % (op[0], s) for s in status.split(',')]
                    status_statement = (" " + op[1] + " ").join(status_list)
                    query += " and ( %s ) " % "%s"
                    params.append(status_statement)

            if not orderBy:
                orderBy = "jobTime"
            if order:
                if order not in ["desc", "asc"]:
                    order = "desc"
            else:
                order = "desc"
            if orderBy != "":
                query += """ order by "%s" %s""" %(orderBy,order)
            if num is not None:
                query += " limit %s " % "%s"
                params.append(str(num))

            with PostgresqlConn() as conn:
                rets = conn.select_many(query,params)

            for record in rets:
                if record["jobStatusDetail"] is not None:
                    record["jobStatusDetail"] = self.load_json(base64.b64decode(record["jobStatusDetail"]))
                if record["jobParams"] is not None:
                    record["jobParams"] = self.load_json(base64.b64decode(record["jobParams"]))

                if record["jobStatus"]=="running":
                    nodeName,domain = EndpointUtils.getNodename()
                    record["inference-url"] = "http://%s.%s/endpoints/v3/v1/models/%s:predict" % (nodeName,domain, "ifs-"+record["jobId"])

                ret.append(record)
        except Exception as e:
            logger.exception('ListInferenceJobV2 Exception: %s', str(e))
        return ret

    @record
    def ListModelConversionJob(self, userName, vcName, status=None, op=("=", "or"), pageNum=None, pageSize=None, name=None, type=None, order=None, orderBy=None, convStatus=None):
        ret = {}
        ret["queuedjobs"] = []
        ret["runningJobs"] = []
        ret["finishedJobs"] = []
        ret["visualizationJobs"] = []
        total = None

        conn = None
        cursor = None
        try:
            conn = self.pool.get_connection()
            cursor = conn.cursor()

            query = """SELECT count(*) OVER() as total, j."jobId" as "jobId", "jobName", "userName", "vcName", "jobStatus", "jobStatusDetail", "jobType", "jobTime", "jobParams", "inputPath", "outputPath", m.type as "modelconversionType", m.status as "modelconversionStatus", priority FROM {} as m left join {} as j on m."jobId" = j."jobId" left join {} as p on m."jobId" = p."jobId" where "isDeleted"=0""".format(
                self.modelconversionjobtablename, self.jobtablename, self.jobprioritytablename)
            if userName != "all":
                query += """ and "userName" = '%s'""" % userName

            if vcName != "all":
                query += """ and "vcName" = '%s'""" % vcName

            if name is not None:
                query += """ and "jobName" like '%%%s%%'""" % sql_injection_parse(name)

            if type is not None:
                query += " and m.type = '%s'" % type

            if status is not None:
                if "," not in status:
                    query += """ and "jobStatus" %s '%s'""" % (op[0], status)
                else:
                    status_list = [""" "jobStatus" %s '%s' """ % (op[0], s) for s in status.split(',')]
                    status_statement = (" " + op[1] + " ").join(status_list)
                    query += " and ( %s ) " % status_statement

            if convStatus is not None:
                query += " and m.status = '%s'" % convStatus

            if order != "asc":
                order = "desc"

            if orderBy is None or orderBy == "":
                query += """ order by "jobTime" Desc"""
            else:
                query += """order by "%s" %s""" % (orderBy, order)

            if pageNum is not None and pageSize is not None:
                query += " limit %d offset %d " % (int(pageSize),(int(pageNum)-1)*int(pageSize)) 
            if pageNum is not None and pageSize is None:
                query += " limit %s " % pageNum
            cursor.execute(query)
            logger.info(query)
            columns = [column[0] for column in cursor.description]
            data = cursor.fetchall()

            for item in data:
                record = dict(zip(columns, item))
                if record["jobStatusDetail"] is not None:
                    record["jobStatusDetail"] = self.load_json(base64.b64decode(record["jobStatusDetail"]))
                if record["jobParams"] is not None:
                    record["jobParams"] = self.load_json(base64.b64decode(record["jobParams"]))

                if record["jobStatus"] == "running":
                    if record["jobType"] == "ModelConversionJob":
                        ret["runningJobs"].append(record)
                    elif record["jobType"] == "visualization":
                        ret["visualizationJobs"].append(record)
                elif record["jobStatus"] == "queued" or record["jobStatus"] == "scheduling" or record[
                    "jobStatus"] == "unapproved":
                    ret["queuedjobs"].append(record)
                else:
                    ret["finishedJobs"].append(record)

                if total is None and record["total"] is not None:
                    total = record["total"]
                else:
                    pass
            conn.commit()
        except Exception as e:
            logger.exception('GetModelConversionJobs Exception: %s', str(e))
        finally:
            if cursor is not None:
                cursor.close()
            if conn is not None:
                conn.close()

        ret["meta"] = {"queuedjobs": len(ret["queuedjobs"]), "runningJobs": len(ret["runningJobs"]),
                       "finishedJobs": len(ret["finishedJobs"]), "visualizationJobs": len(ret["visualizationJobs"])}
        ret["total"] = total
        return ret

    @record
    def GetActiveJobList(self):
        ret = []
        try:
            query = """SELECT "jobId", "userName", "vcName", "jobParams", "jobStatus" FROM %s WHERE "jobStatus" = 'scheduling' OR "jobStatus" = 'running' and "isDeleted"=0""" % (
                self.jobtablename)
            with PostgresqlConn() as conn:
                logger.info(query)
                rets = conn.select_many(query)
            for one in rets:
                ret.append(one)

        except Exception as e:
            logger.exception('GetActiveJobList Exception: %s', str(e))
        return ret

    @record
    def GetGpuTypeActiveJobCount(self):
        ret = {}
        try:
            query = """SELECT "jobId", "userName", "vcName", "jobParams", "jobStatus" FROM %s WHERE "jobStatus" = 'scheduling' OR "jobStatus" = 'running' and "isDeleted"=0""" % (
                self.jobtablename)
            with PostgresqlConn() as conn:
                logger.info(query)
                rets = conn.select_many(query)
            for one in rets:
                jobParam = json.loads(base64.b64decode(one["jobParams"]))
                ret.setdefault(jobParam["gpuType"],0)
                ret[jobParam["gpuType"]]+=1
        except Exception as e:
            logger.exception('GetActiveJobList Exception: %s', str(e))
        return ret

    @record
    def GetJob(self, **kwargs):
        valid_keys = ["jobId", "familyToken", "isParent", "jobName", "userName", "vcName", "jobStatus", "jobType",
                      "jobTime"]
        if len(kwargs) != 1: return []
        key, expected = kwargs.popitem()
        if key not in valid_keys:
            logger.exception("DataHandler_GetJob: key is not in valid keys list...")
            return []

        ret = []
        conn = None
        cursor = None
        try:
            conn = self.pool.get_connection()
            cursor = conn.cursor()

            query = """SELECT "jobId","familyToken","isParent","jobName","userName", "vcName", "jobStatus", "jobStatusDetail", "jobType", "jobDescriptionPath", "jobDescription", "jobTime", endpoints, "jobParams","errorMsg" ,"jobMeta" ,"jobGroup" FROM %s where "%s" = %s """ % (
            self.jobtablename, key, "%s")
            logger.info(query,[expected])
            cursor.execute(query,[expected])
            columns = [column[0] for column in cursor.description]
            ret = [dict(zip(columns, row)) for row in cursor.fetchall()]
            conn.commit()
        except Exception as e:
            logger.exception('GetJob Exception: %s', str(e))
        finally:
            if cursor is not None:
                cursor.close()
            if conn is not None:
                conn.close()
        return ret

    @record
    def GetJobV2(self, jobId):
        ret = []
        conn = None
        cursor = None
        try:
            conn = self.pool.get_connection()
            cursor = conn.cursor()
            query = """SELECT "jobId", "jobName", "userName", "vcName", "jobStatus", "jobStatusDetail", "jobType", "jobTime", "jobParams"  FROM %s where "jobId" = %s """ % (
            self.jobtablename, "%s")
            logger.info(query,[jobId])
            cursor.execute(query,[jobId])

            columns = [column[0] for column in cursor.description]
            data = cursor.fetchall()
            for item in data:
                record = dict(zip(columns, item))
                if record["jobStatusDetail"] is not None:
                    record["jobStatusDetail"] = self.load_json(base64.b64decode(record["jobStatusDetail"]))
                if record["jobParams"] is not None:
                    record["jobParams"] = self.load_json(base64.b64decode(record["jobParams"]))
                ret.append(record)
            conn.commit()
        except Exception as e:
            logger.exception('GetJobV2 Exception: %s', str(e))
        finally:
            if cursor is not None:
                cursor.close()
            if conn is not None:
                conn.close()
        return ret

    @record
    def GetInferenceJob(self, jobId):
        ret = []
        conn = None
        cursor = None
        try:
            conn = self.pool.get_connection()
            cursor = conn.cursor()
            query = """SELECT "jobId", "jobName", "userName", "vcName", "jobStatus", "jobStatusDetail", "jobType", "jobTime", "jobParams",endpoints  FROM %s where "jobId" = %s """ % (
            self.jobtablename, "%s")
            logger.info(query,[jobId])
            cursor.execute(query,[jobId])

            columns = [column[0] for column in cursor.description]
            data = cursor.fetchall()
            for item in data:
                record = dict(zip(columns, item))

                if record["jobStatusDetail"] is not None:
                    record["jobStatusDetail"] = self.load_json(base64.b64decode(record["jobStatusDetail"]))
                if record["jobParams"] is not None:
                    record["jobParams"] = self.load_json(base64.b64decode(record["jobParams"]))

                if record["jobStatus"]=="running":
                    nodeName,domain = EndpointUtils.getNodename()
                    record["inference-url"] = "http://%s.%s/endpoints/v3/v1/models/%s:predict" % (nodeName,domain, "ifs-"+record["jobId"])

                ret.append(record)
            conn.commit()
        except Exception as e:
            logger.exception('GetJobV2 Exception: %s', str(e))
        finally:
            if cursor is not None:
                cursor.close()
            if conn is not None:
                conn.close()
        return ret

    @record
    def DeleteJobByVc(self,vcName):
        try:
            sql = """DELETE FROM %s WHERE "vcName"= %s """ %(self.jobtablename,"%s")
            print(sql)
            with PostgresqlConn() as conn:
                logger.info(sql, [vcName])
                conn.insert_one(sql, [vcName])
                conn.commit()
            ret = True
        except Exception as e:
            logger.exception('AddCommand Exception: %s', str(e))
            ret = False
        return ret

    @record
    def DeleteJobByVcExcludeKilling(self,vcName):
        try:
            sql = """DELETE FROM %s WHERE "vcName"= %s and "jobStatus"<>'killing'""" %(self.jobtablename,"%s")
            print(sql)
            with PostgresqlConn() as conn:
                logger.info(sql, [vcName])
                conn.insert_one(sql, [vcName])
                conn.commit()
            ret = True
        except Exception as e:
            logger.exception('AddCommand Exception: %s', str(e))
            ret = False
        return ret

    @record
    def AddCommand(self, jobId, command):
        ret = False
        try:
            sql = """INSERT INTO %s  ("jobId", command) VALUES (%s,%s)""" %(self.commandtablename,"%s","%s")
            with PostgresqlConn() as conn:
                logger.info(sql, (jobId, command))
                conn.insert_one(sql, (jobId, command))
                conn.commit()
            ret = True
        except Exception as e:
            logger.exception('AddCommand Exception: %s', str(e))
            ret = False
        return ret

    @record
    def GetPendingCommands(self):
        ret = []
        try:
            query = """SELECT id, "jobId", command FROM %s WHERE status = 'pending' order by time""" % (
                self.commandtablename)
            with PostgresqlConn() as conn:
                logger.info(query)
                rets = conn.select_many(query)
            for one in rets:
                ret.append(one)
        except Exception as e:
            logger.exception('GetPendingCommands Exception: %s', str(e))
        return ret

    @record
    def FinishCommand(self, commandId):
        ret = True
        try:
            sql = """update %s set status = 'run' where id = %s """ % (self.commandtablename, "%s")
            with PostgresqlConn() as conn:
                logger.info(sql,[commandId])
                conn.update(sql,[commandId])
                conn.commit()
            ret = True
        except Exception as e:
            logger.exception('FinishCommand Exception: %s', str(e))
        return ret

    @record
    def GetCommands(self, jobId):
        ret = []
        try:
            query = """SELECT time, command, status, output FROM %s WHERE "jobId" = %s order by time""" % (
            self.commandtablename, "%s")
            with PostgresqlConn() as conn:
                logger.info(query,[jobId])
                rets = conn.select_many(query,[jobId])
            for one in rets:
                ret.append(one)
        except Exception as e:
            logger.exception('FinishCommand Exception: %s', str(e))
        return ret

    def load_json(self, raw_str):
        if raw_str is None:
            return {}
        if isinstance(raw_str, unicode):
            raw_str = str(raw_str)
        try:
            return json.loads(raw_str)
        except:
            return {}

    @record
    def GetPendingEndpoints(self):
        ret = {}
        try:
            query = """SELECT endpoints,"jobId" from %s where "jobStatus" = \'%s\' and endpoints is not null""" % (
            self.jobtablename, "running")
            with PostgresqlConn() as conn:
                logger.info(query)
                rets = conn.select_many(query)
            # [ {endpoint1:{},endpoint2:{}}, {endpoint3:{}, ... }, ... ]
            endpoints = map(lambda job: self.load_json(job["endpoints"]), rets)
            # {endpoint1: {}, endpoint2: {}, ... }
            # endpoint["status"] == "pending"
            for one in endpoints:
                for k,v in one.items():
                    if v["status"] == "pending":
                        ret.setdefault(v["jobId"],{})
                        ret[v["jobId"]][k] = v
        except Exception as e:
           logger.exception("Query pending endpoints failed!")
        return ret

    @record
    def GetJobEndpoints(self, job_id):
        ret = {}
        try:
            query = """SELECT endpoints from %s where "jobId" = %s""" % (self.jobtablename, "%s")
            with PostgresqlConn() as conn:
                logger.info(query,[jobId])
                rets = conn.select_many(query,[job_id])
            # [ {endpoint1:{},endpoint2:{}}, {endpoint3:{}, ... }, ... ]
            endpoints = map(lambda job: self.load_json(job["endpoints"]), rets)
            # {endpoint1: {}, endpoint2: {}, ... }
            # endpoint["status"] == "pending"
            ret = {k: v for d in endpoints for k, v in d.items()}
        except Exception as e:
            logger.warning("Query job endpoints failed! Job {}".format(job_id), exc_info=True)
        return ret

    @record
    def GetDeadEndpoints(self):
        ret = {}
        try:
            # TODO we need job["lastUpdated"] for filtering
            query = """SELECT endpoints FROM jobs WHERE "jobStatus" <> 'running' and "jobStatus" <> 'pending' and "jobStatus" <> 'queued' and "jobStatus" <> 'scheduling' order by "jobTime" DESC"""
            with PostgresqlConn() as conn:
                logger.info(query)
                rets = conn.select_many(query)
            for one in rets:
                endpoint_list = {k: v for k, v in self.load_json(one["endpoints"]).items() if v["status"] == "running"}
                ret.update(endpoint_list)
        except Exception as e:
            logger.exception('Query dead endpoints Exception: %s', str(e))
        return ret

    @record
    def UpdateEndpoint(self, endpoint):
        ret = False
        try:
            job_endpoints = self.GetJobEndpoints(endpoint["jobId"])

            # update jobEndpoints
            job_endpoints[endpoint["id"]] = endpoint

            sql = """UPDATE jobs SET endpoints=%s where "jobId"=%s"""
            with PostgresqlConn() as conn:
                conn.update(sql, (json.dumps(job_endpoints), endpoint["jobId"]))
                conn.commit()
            ret = True
        except Exception as e:
           logger.exception("Update endpoints failed! Endpoints: {}".format(endpoint))
        return ret

    @record
    def GetPendingJobs(self):
        ret = []
        try:
            query = """SELECT "jobId","jobName","userName", "vcName", "jobStatus", "jobStatusDetail", "jobType", "jobDescriptionPath", "jobDescription", "jobTime", endpoints, "jobParams","errorMsg" ,"jobMeta" FROM %s where "jobStatus" <> 'error' and "jobStatus" <> 'failed' and "jobStatus" <> 'finished' and "jobStatus" <> 'killed' and "isDeleted"=0 order by "jobTime" DESC""" % (
                self.jobtablename)
            with PostgresqlConn() as conn:
                logger.info(query)
                rets = conn.select_many(query)
            for one in rets:
                ret.append(one)
        except Exception as e:
            logger.exception('GetPendingJobs Exception: %s', str(e))
        return ret

    @record
    def SetJobError(self, jobId, errorMsg):
        ret = False
        try:
            sql = """update %s set "jobStatus" = 'error', "errorMsg" = %s where "jobId" = %s """ % (
            self.jobtablename, "%s", "%s")
            with PostgresqlConn() as conn:
                logger.info(sql,[ errorMsg, jobId])

                conn.update(sql,[ errorMsg, jobId])
                conn.commit()
            ret = True
        except Exception as e:
            logger.exception('SetJobError Exception: %s', str(e))
        return ret

    @record
    def UpdateJobTextField(self, jobId, field, value):
        ret = False
        try:
            sql = """update %s set "%s" = %s where "jobId" = %s """ % (self.jobtablename, field, "%s", "%s")
            with PostgresqlConn() as conn:
                logger.info(sql,[value, jobId])
                conn.update(sql,[value, jobId])
                conn.commit()
            ret = True
        except Exception as e:
            logger.exception('UpdateJobTextField Exception: %s', str(e))
            ret = False
        return ret

    @record
    def GetJobTextField(self, jobId, field):
        query = """SELECT "jobId", "%s" FROM %s where "jobId" = %s """ % (field, self.jobtablename, jobId)
        ret = None
        try:
            with PostgresqlConn() as conn:
                logger.info(query)

                rets = conn.select_many(query)
            for one in rets:
                ret = one[field]
        except Exception as e:
            logger.exception('Exception: %s', str(e))
            pass
        return ret

    @record
    def AddandGetJobRetries(self, jobId):
        sql = """update %s set retries = retries + 1 where "jobId" = %s """ % (self.jobtablename, "%s")
        with PostgresqlConn() as conn:
            conn.update(sql,[jobId])
            conn.commit()

        query = """SELECT "jobId", retries FROM %s where "jobId" = %s """ % (self.jobtablename, "%s")
        with PostgresqlConn() as conn:
            logger.info(query,[jobId])
            rets = conn.select_many(query,[jobId])
        ret = None

        for one in rets:
            ret = one["retries"]
        return ret

    @record
    def UpdateJobTextFields(self, conditionFields, dataFields):
        ret = False
        if not isinstance(conditionFields, dict) or not conditionFields or not isinstance(dataFields,
                                                                                          dict) or not dataFields:
            return ret

        try:
            sql = "update %s set" % (self.jobtablename) + ",".join([""" "%s" = '%s' """ % (field, value) for field, value in dataFields.items()]) + " where" + "and".join([""" "%s" = '%s' """ % (field, value) for field, value in conditionFields.items()])
            with PostgresqlConn() as conn:
                logger.info(sql)
                conn.update(sql)
                conn.commit()
            ret = True
        except Exception as e:
            logger.exception('updateJobTextFields Exception: %s, ex: %s', dataFields, str(e))
        return ret

    @record
    def GetJobTextField(self, jobId, field):
        ret = None
        try:
            query = """SELECT "jobId", "%s" FROM %s where "jobId" = %s """ % (field, self.jobtablename, "%s")
            with PostgresqlConn() as conn:
                logger.info(query,[jobId])
                rets = conn.select_many(query,[jobId])
            for one in rets:
                ret = one[field]
        except Exception as e:
            logger.exception('GetJobTextField Exception: %s', str(e))
        return ret

    @record
    def GetJobTextFields(self, jobId, fields):
        conn = None
        cursor = None
        ret = None
        if not isinstance(fields, list) or not fields:
            return ret

        try:
            fieldsStr = ""
            for field in fields:
                fieldsStr += '"%s", ' %(field)
            fieldsStr = fieldsStr[:-2]
            sql = """select %s from  %s  where "jobId"='%s' """ % (fieldsStr,self.jobtablename,jobId)
            conn = self.pool.get_connection()
            cursor = conn.cursor()
            logger.info(sql)
            cursor.execute(sql)

            columns = [column[0] for column in cursor.description]
            for item in cursor.fetchall():
                ret = dict(zip(columns, item))
            conn.commit()
        except Exception as e:
            logger.exception('GetJobTextFields Exception: %s', str(e))
        finally:
            if cursor is not None:
                cursor.close()
            if conn is not None:
                conn.close()
        return ret

    @record
    def AddandGetJobRetries(self, jobId):
        ret = None
        try:
            sql = """update %s set retries = retries + 1 where "jobId" = %s """ % (self.jobtablename, "%s")
            with PostgresqlConn() as conn:
                conn.update(sql,[jobId])
                conn.commit()

            query = """SELECT "jobId", retries FROM %s where "jobId" = %s """ % (self.jobtablename, "%s")
            with PostgresqlConn() as conn:
                logger.info(query,[jobId])

                rets = conn.select_many(query,[jobId])
            for one in rets:
                ret = one["retries"]
        except Exception as e:
            logger.exception('AddandGetJobRetries Exception: %s', str(e))
        return ret

    @record
    def UpdateClusterStatus(self, clusterStatus):
        try:
            status = base64.b64encode(json.dumps(clusterStatus))
            sql = """INSERT INTO %s (status) VALUES (%s)""" % (self.clusterstatustablename, "%s")
            with PostgresqlConn() as conn:
                logger.info(sql,[status])
                conn.insert_one(sql,[status])
                conn.commit()
            ret = True
        except Exception as e:
            logger.exception('UpdateClusterStatus Exception: %s', str(e))
            ret = False
        return ret

    @record
    def GetClusterStatus(self):
        ret = None
        time = None
        try:
            query = """SELECT time, status FROM %s order by time DESC limit 1""" % (self.clusterstatustablename)
            with PostgresqlConn() as conn:
                logger.info(query)
                rets = conn.select_many(query)
            for one in rets:
                ret = json.loads(base64.b64decode(one["status"]))
                time = one["time"]
        except Exception as e:
            logger.exception('GetClusterStatus Exception: %s', str(e))
        return ret, time

    @record
    def GetAllAccountUser(self):
        query = """SELECT uid,"openId","group","nickName","userName",password,"isAdmin","isAuthorized",email,"phoneNumber" FROM %s""" % (self.accounttablename)
        ret = []
        try:
            with PostgresqlConn() as conn:
                logger.info(query)
                rets = conn.select_many(query)
            for one in rets:
                ret.append(one)

        except Exception as e:
            logger.exception('GetAllAccountUser Exception: %s', str(e))
        return ret

    @record
    def GetUsers(self):
        ret = []
        try:
            # query = "SELECT identityName,uid FROM %s" % (self.identitytablename)
            # with PostgresqlConn() as conn:
            #     rets = conn.select_many(query)
            # for one in rets:
            #     ret.append((one["identityName"],one["uid"]))
            res = requests.get(url=config["usermanagerapi"] + "/open/allUsers",headers={"Authorization": "Bearer " + config["usermanagerapitoken"]})
            if res.status_code == 200:
                response = json.loads(res.content.decode("utf-8"))["list"]
                for one in response:
                    ret.append((one["userName"],one["uid"]))
        except Exception as e:
            logger.exception('GetUsers Exception: %s', str(e))
        return ret

    @record
    def GetActiveJobsCount(self):
        ret = 0
        try:
            query = """SELECT count(ALL id) as c FROM %s where "jobStatus" = 'running' and "isDeleted"=0""" % (self.jobtablename)
            with PostgresqlConn() as conn:
                logger.info(query)
                rets = conn.select_many(query)
            for c in rets:
                ret = c["c"]
        except Exception as e:
            logger.exception('GetActiveJobsCount Exception: %s', str(e))
        return ret

    @record
    def GetActiveJobsCount(self):
        ret = 0
        try:
            query = """SELECT count(ALL id) as c FROM %s where "jobStatus" = 'running' and "isDeleted"=0""" % (self.jobtablename)
            with PostgresqlConn() as conn:
                logger.info(query)
                rets = conn.select_many(query)
            for c in rets:
                ret = c["c"]
        except Exception as e:
            logger.exception('GetActiveJobsCount Exception: %s', str(e))
        return ret

    @record
    def GetALLJobsCount(self):
        ret = 0
        try:
            query = """SELECT count(ALL id) as c FROM %s where "isDeleted"=0""" % (self.jobtablename)
            with PostgresqlConn() as conn:
                logger.info(query)
                rets = conn.select_many(query)
            for c in rets:
                ret = c["c"]
        except Exception as e:
            logger.exception('GetALLJobsCount Exception: %s', str(e))
        return ret

    @record
    def GetTemplates(self, scope):
        ret = []
        try:
            query = """SELECT name, json, "isDefault" FROM %s WHERE scope = %s""" % (self.templatetablename, "%s")
            with PostgresqlConn() as conn:
                logger.info(query,[scope])
                rets = conn.select_many(query,[scope])
            for one in rets:
                one["scope"] = "user" if scope.split(":")[0]=="user" else "team" if scope.split(":")[0]=="vc" else "master"
                ret.append(one)
        except Exception as e:
            logger.exception('GetTemplates Exception: %s', str(e))
        return ret

    @record
    def UpdateTemplate(self, name, scope, json):
        ret = False
        try:
            query = """INSERT INTO %s (name, scope, json) VALUES(%s, %s, %s)  ON CONFLICT (id) DO UPDATE SET json = %s""" %(self.templatetablename,"%s", "%s", "%s","%s")
            with PostgresqlConn() as conn:
                logger.info(query, (name, scope, json, json))
                conn.insert_one(query, (name, scope, json, json))
                conn.commit()
            ret = True
        except Exception as e:
            logger.exception('UpdateTemplate Exception: %s', str(e))
        return ret

    @record
    def HasCurrentActiveJob(self,name):
        ret = False
        try:
            query = """SELECT count(1) FROM  WHERE "jobStatus" in ('queued', 'scheduling', 'running', 'unapproved', 'pausing', 'paused') and "userName" = %s  and "isDeleted"=0"""%(self.jobtablename,"%s")
            with PostgresqlConn() as conn:
                logger.info(query ,(name,))

                cnt = conn.select_one_value(query,(name,))
                if int(cnt)>0:
                    ret = True
        except Exception as e:
            logger.exception('UpdateTemplate Exception: %s', str(e))
        return ret

    @record
    def DeleteTemplate(self, name, scope):
        ret = False
        try:
            query = """DELETE FROM %s WHERE name = %s and scope = %s""" %(self.templatetablename,"%s","%s")
            with PostgresqlConn() as conn:
                logger.info(query,  (name, scope))

                conn.insert_one(query, (name, scope))
                conn.commit()
            ret = True
        except Exception as e:
            logger.exception('DeleteTemplate Exception: %s', str(e))
        return ret

    @record
    def DeleteTemplateByVc(self, scope):
        ret = False
        try:
            query = """DELETE FROM %s  WHERE scope = %s""" %(self.templatetablename,"%s")
            with PostgresqlConn() as conn:
                logger.info(query, (scope,))
                conn.insert_one(query, (scope,))
                conn.commit()
            ret = True
        except Exception as e:
            logger.exception('DeleteTemplate Exception: %s', str(e))
        return ret

    @record
    def get_job_priority(self):
        ret = {}
        try:
            query = """select "jobId", priority from {} where "jobId" in (select "jobId" from {} where "jobStatus" in (\'queued\', \'scheduling\', \'running\', \'unapproved\', \'pausing\', \'paused\'))""".format(
                self.jobprioritytablename, self.jobtablename)
            with PostgresqlConn() as conn:
                logger.info(query)
                rets = conn.select_many(query)
            for one in rets:
                ret[one["jobId"]] = one["priority"]
        except Exception as e:
            logger.exception('get_job_priority Exception: %s', str(e))
        return ret

    @record
    def update_job_priority(self, job_priorites):
        ret = False
        try:
            for job_id, priority in job_priorites.items():
                query = """INSERT INTO {0}("jobId", priority, time) VALUES('{1}', {2}, SYSDATE()) ON CONFLICT ("jobId") DO UPDATE SET  priority='{2}' """.format(
                    self.jobprioritytablename, job_id, priority)
            with PostgresqlConn() as conn:
                logger.info(query)
                conn.insert_one(query)
                conn.commit()
            ret = True
        except Exception as e:
            logger.exception('update_job_priority Exception: %s', str(e))
        return ret

    @record
    def ConvertDataFormat(self,projectId, datasetId,datasetType,targetFormat):
        ret = False
        try:
            query = """INSERT INTO %s ("projectId", "datasetId", type,"targetFormat") VALUES(%s,%s,%s,%s) ON CONFLICT (id) DO UPDATE SET status='queued'""" % (self.dataconvert,"%s","%s","%s","%s")
            with PostgresqlConn() as conn:
                logger.info(query,(projectId,datasetId,datasetType,targetFormat))
                conn.insert_one(query,(projectId,datasetId,datasetType,targetFormat))
                conn.commit()
            ret = True
        except Exception as e:
            logger.exception('add ConvertDataFormat Exception: %s', str(e))
        return ret

    @record
    def getConvertList(self,targetStatus=None):
        ret = []
        try:
            query = """select id,"projectId", "datasetId", type,"targetFormat",status,time,"outPath" FROM %s where true""" % (self.dataconvert,)
            params = []
            if targetStatus:
                if "," in targetStatus:
                    query += " and status in %s"
                    params.extend(targetStatus.split(","))
                else:
                    query += " and status = %s"
                    params.append(targetStatus)
            with PostgresqlConn() as conn:
                logger.info(query,params)
                ret = conn.select_many(query,params)
        except Exception as e:
            logger.exception('add ConvertDataFormat Exception: %s', str(e))
        return ret

    @record
    def GetConvertDetail(self,projectId,datasetId):
        ret = []
        try:
            query = """select id,type,"targetFormat",status,time,"outPath" FROM %s where "projectId"=%s and "datasetId"=%s """ % (self.dataconvert,"%s","%s")
            with PostgresqlConn() as conn:
                logger.info(query,[projectId,datasetId])

                ret = conn.select_many(query,[projectId,datasetId])
        except Exception as e:
            logger.exception('add ConvertDataFormat Exception: %s', str(e))
        return ret

    @record
    def updateConvertStatus(self,targetStatus,id,errMsg=None,outPath=None):
        ret = []
        try:
            if errMsg:
                query = """update %s set status=%s,"errorMsg"=%s where id=%s""" % (self.dataconvert,"%s","%s","%s")
                params = (targetStatus,errMsg, id)
            elif outPath:
                query = "update %s set status=%s,outPath=%s where id=%s" % (self.dataconvert, "%s", "%s", "%s")
                params = (targetStatus, outPath, id)
            else:
                query = "update %s set status=%s where id=%s" % (self.dataconvert, "%s", "%s")
                params = (targetStatus, id)
            with PostgresqlConn() as conn:
                logger.info(query,params)

                conn.insert_one(query,params)
                conn.commit()
        except Exception as e:
            logger.exception('add ConvertDataFormat Exception: %s', str(e))
        return ret
    @record
    def get_job_summary(self, userName, jobType, vcName):
        ret = {}

        try:
            query = """select "jobStatus", count(*) as count from %s where "isDeleted"=0 """ % (self.jobtablename)

            if userName is not None and userName != "":
                query += """ and "userName" = '%s' """% (userName)
            if vcName is not None and vcName != "":
                query += """ and "vcName" = '%s' """% (vcName)
            if len(jobType) > 0:
                query += """ and "jobType"='%s' """% (jobType)

            query += """ group by "jobStatus";""" 

            logger.info(query)

            with PostgresqlConn() as conn:
                logger.info(query)

                records = conn.select_many(query)

            for one in records:
                ret[one["jobStatus"]] = int(one["count"])

        except Exception as e:
            logger.exception('get_job_priority Exception: %s', str(e))

        return ret



    @record
    def DeleteJob(self,jobId):
        ret = False
        try:
            query = """UPDATE %s set "isDeleted"=1 where "jobId"=%s """ % (self.jobtablename, "%s")
            with PostgresqlConn() as conn:
                logger.info(query,[jobId])

                conn.insert_one(query,[jobId])
                conn.commit()
            ret = True
        except Exception as e:
            logger.exception('DeleteJob Exception: %s', str(e))
        return ret


    def __del__(self):
        logger.debug("********************** deleted a DataHandler instance *******************")
        self.Close()

    def Close(self):
        ### !!! This DataHandler uses DB pool, so close connection in each method
        pass


if __name__ == '__main__':
    TEST_INSERT_JOB = False
    TEST_QUERY_JOB_LIST = False
    CREATE_TABLE = False
    CREATE_DB = True
    dataHandler = DataHandler()
    print
    dataHandler.GetJobList("hongzl@microsoft.com", num=1)
    if TEST_INSERT_JOB:
        jobParams = {}
        jobParams["id"] = "dist-tf-00001"
        jobParams["job-name"] = "dist-tf"
        jobParams["user-id"] = "hongzl"
        jobParams["job-meta-path"] = "/dlws/jobfiles/***"
        jobParams["job-meta"] = "ADSCASDcAE!EDASCASDFD"

        dataHandler.AddJob(jobParams)

    if CREATE_TABLE:
        dataHandler.CreateTable()

    if CREATE_DB:
        dataHandler.CreateDatabase()
