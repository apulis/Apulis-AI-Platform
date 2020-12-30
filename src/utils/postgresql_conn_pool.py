# coding=utf8
from DBUtils.PooledDB import PooledDB
import threading
import logging
import traceback
from config import config
import psycopg2


from decimal import Decimal
from prometheus_client import Histogram
db_connect_histogram = Histogram("db_connect_latency_seconds",
                                 "latency for connecting to db (seconds)",
                                 buckets=(.05, .075, .1, .25, .5, .75, 1.0, 2.5, 5.0, 7.5, float("inf")))

def dict_decimal_to_float(d):
    if not isinstance(d, dict):
        raise TypeError("d must be dict")
    for key, value in d.items():
        if isinstance(value, Decimal):
            d[key] = float(value)
    return d

class PostgresqlConn(object):
    _db_pools = {}
    _risk_min_cached = 0
    _risk_max_cached = 1
    _risk_max_connections = 1
    _risk_block = False

    _master_min_cached = 0
    _master_max_cached = 1
    _master_max_connections = 1
    _master_block = False

    _reptile_min_cached = 0
    _reptile_max_cached = 1
    _reptile_max_connections = 1
    _reptile_block = False

    _tidb_min_cached = 0
    _tidb_max_cached = 1
    _tidb_max_connections = 1
    _tidb_block = False

    _risk_loan_read_min_cached = 0
    _risk_loan_read_max_cached = 1
    _risk_loan_read_connections = 1
    _risk_loan_read_block = False

    _lock = threading.Lock()

    @classmethod
    def config_pool(cls, risk_config=None, master_config=None, reptile_config=None, tidb_config=None, risk_loan_read_config=None):
        PostgresqlConn._lock.acquire()
        try:
            if risk_config is not None and PostgresqlConn._db_pools.has_key("risk"):
                raise Exception("risk db pool has been inited")
            if master_config is not None and PostgresqlConn._db_pools.has_key("master"):
                raise Exception("master db pool has been inited")
            if reptile_config is not None and PostgresqlConn._db_pools.has_key("reptile"):
                raise Exception("reptile_config db pool has been inited")
            if tidb_config is not None and PostgresqlConn._db_pools.has_key("tidb"):
                raise Exception("tidb_config db pool has been inited")
            if risk_loan_read_config is not None and PostgresqlConn._db_pools.has_key("riskloanread"):
                raise Exception("riskloanread db pool has been inited")

            if risk_config is not None:
                cls._risk_min_cached = risk_config.get("min_cached", 0)
                cls._risk_max_cached = risk_config.get("max_cached", 1)
                cls._risk_max_connections = risk_config.get("max_connections", 5)
                cls._risk_block = risk_config.get("block", False)
            if master_config is not None:
                cls._master_min_cached = master_config.get("min_cached", 0)
                cls._master_max_cached = master_config.get("max_cached", 1)
                cls._master_max_connections = master_config.get("max_connections", 5)
                cls._master_block = master_config.get("block", False)
            if reptile_config is not None:
                cls._reptile_min_cached = reptile_config.get("min_cached", 0)
                cls._reptile_max_cached = reptile_config.get("max_cached", 1)
                cls._reptile_max_connections = reptile_config.get("max_connections", 5)
                cls._reptile_block = reptile_config.get("block", False)

            if tidb_config is not None:
                cls._tidb_min_cached = tidb_config.get("min_cached", 0)
                cls._tidb_max_cached = tidb_config.get("max_cached", 1)
                cls._tidb_max_connections = tidb_config.get("max_connections", 5)
                cls._tidb_block = tidb_config.get("block", False)

            if risk_loan_read_config is not None:
                cls._risk_loan_read_min_cached = risk_loan_read_config.get("min_cached", 0)
                cls._risk_loan_read_max_cached = risk_loan_read_config.get("max_cached", 1)
                cls._risk_loan_read_connections = risk_loan_read_config.get("max_connections", 5)
                cls._risk_loan_read_block = risk_loan_read_config.get("block", False)
        finally:
            PostgresqlConn._lock.release()

    @staticmethod
    def connection_statics():
        ret = {}
        max_ret = {}
        for db_type, db_pool in PostgresqlConn._db_pools.items():
            ret[db_type] = db_pool._connections
            max_ret[db_type] = db_pool._maxconnections
        return ret, max_ret


    def __init__(self, db_type="risk",database=None,server=None,username=None,password=None):
        if db_type not in ["risk", "master", "db4", "reptile", "tidb", "riskloanread","model"]:
            raise Exception("db_type must be risk or master or reptile")
        if not PostgresqlConn._db_pools.has_key(db_type):
            PostgresqlConn._lock.acquire()
            try:
                if not PostgresqlConn._db_pools.has_key(db_type):
                    database = database if database else "DLWSCluster-%s" % config["clusterId"]
                    server =server if server else  config["postgresql"]["hostname"]
                    username =username if username else  config["postgresql"]["username"]
                    password =password if password else  config["postgresql"]["password"]
                    if db_type == "risk":
                        PostgresqlConn._db_pools[db_type] = PooledDB(creator=psycopg2, mincached=PostgresqlConn._risk_min_cached,
                                                                maxcached=PostgresqlConn._risk_max_cached,
                                                                maxconnections=PostgresqlConn._risk_max_connections,
                                                                host=server,
                                                                user=username,
                                                                password=password,
                                                                database=database,
                                                                blocking=PostgresqlConn._risk_block)
                    elif db_type == "master":
                        PostgresqlConn._db_pools[db_type] = PooledDB(creator=psycopg2, mincached=PostgresqlConn._risk_min_cached,
                                                                maxcached=PostgresqlConn._risk_max_cached,
                                                                maxconnections=PostgresqlConn._risk_max_connections,
                                                                host=server,
                                                                user=username,
                                                                password=password,
                                                                database=database,
                                                                blocking=PostgresqlConn._risk_block)

            finally:
                PostgresqlConn._lock.release()

        with db_connect_histogram.time():
            self._conn = PostgresqlConn._db_pools[db_type].connection()
            self._cur = self._conn.cursor()


    def select_one(self, sql, params=None):
        self._cur.execute(sql, params)
        result = [[item for item in row] for row in self._cur.fetchone()]
        coloumns = [row[0] for row in self._cur.description]
        result = [dict(zip(coloumns, row)) for row in result][0]
        return result


    def select_one_value(self, sql, params=None):
        self._cur.execute(sql, params)
        count = self._cur.fetchone()
        if count:
            return count[0]
        else:
            return None

    def select_many(self, sql, params=None):
        self._cur.execute(sql, params)
        result = [[item for item in row] for row in self._cur.fetchall()]
        coloumns = [row[0] for row in self._cur.description]
        result = [dict(zip(coloumns, row)) for row in result]
        return result


    def insert_one(self, sql, params=None, return_auto_increament_id=False):
        self._cur.execute(sql, params)
        if return_auto_increament_id:
            return self._cur.lastrowid

    def insert_many(self, sql, params):
        count = self._cur.executemany(sql, params)
        return count

    def update(self, sql, param=None):
        return self._cur.execute(sql, param)

    def delete(self,sql,param=None):
        return self._cur.execute(sql,param)

    def commit(self):
        self._conn.commit()

    def rollback(self):
        self._conn.rollback()

    def close(self):
        self._cur.close()
        self._conn.close()

    def executed(self):
        """获取刚刚执行的SQL语句
        _cur._executed 是私有属性，可能会在没有警告的情况下改变，所以本方法仅适用于调试时使用"""
        return self._cur._executed

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        if exc_type is not None:
            logging.error(exc_type)
            logging.error(exc_val)
            logging.error(traceback.format_exc())
        self.close()
        return False



if __name__ == '__main__':

    with PostgresqlConn() as connect:
        a = connect.select_one("select * from midatadb.r_test")
        connect.commit()
        print(a)
