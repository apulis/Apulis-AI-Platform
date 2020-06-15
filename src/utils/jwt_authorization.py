import time
import jwt
from config import config

def create_jwt_token_with_message(ret):
    return create_jwt_token_for_claims({"userId":ret["uid"],"userName":ret["userName"]},**{
        "openId":ret["openId"],
        "uid":ret["uid"],
        "userName":ret["userName"],
        "gid":ret["uid"],
        "familyName":"",
        "givenName":"",
    })

def create_jwt_token_for_claims(claims,**kwargs):
    payload = {
        'iat': time.time(),
        'claims': claims,
        "iss":  config["webportal_node"],
        "aud ": config["webportal_node"],
        "exp": time.time() + int(config["jwt"]["token_ttl"]),
        "nbf": time.time()-1,
        "identity": claims
    }
    payload.update(kwargs)
    headers = {
        'alg': config["jwt"]["algorithm"],
    }
    return create_jwt_token(payload,config["jwt"]["secret_key"],headers=headers)

def create_jwt_token(payload,secret,algorithm=config["jwt"]["algorithm"],headers=None):
    jwt_token = jwt.encode(payload,
                           secret,
                           algorithm=algorithm,
                           headers=headers
                           ).decode('utf-8')
    return jwt_token

if __name__ == '__main__':
    token = create_jwt_token_with_message({"uid":30000,"userName":"administrator","openId":1})
    print token