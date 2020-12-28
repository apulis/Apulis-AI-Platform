
import base64
import time
import datetime

def base64encode(str_val):
    return base64.b64encode(str_val.encode("utf-8")).decode("utf-8")


def base64decode(str_val):
    return base64.b64decode(str_val.encode("utf-8")).decode("utf-8")


def override(func):
    return func


def walk_json(obj, *fields):
    """ for example a=[{"a": {"b": 2}}]
    walk_json_field_safe(a, 0, "a", "b") will get 2
    walk_json_field_safe(a, 0, "not_exist") will get None
    """
    try:
        for f in fields:
            obj = obj[f]
        return obj
    except:
        return None

def to_seconds_from_date(date):
    return time.mktime(date.timetuple())

def to_seconds_from_isodate_str(date_str):
    return to_seconds_from_date(datetime.datetime.strptime(date_str, "%Y-%m-%dT%H:%M:%S.%f"))