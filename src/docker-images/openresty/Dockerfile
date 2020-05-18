FROM ubuntu:18.04
RUN apt update && apt-get install -y libpcre3-dev libssl-dev perl make build-essential curl wget
RUN wget https://openresty.org/download/openresty-1.15.8.3.tar.gz && tar zxf openresty-1.15.8.3.tar.gz && cd openresty-1.15.8.3 && ./configure --without-http_gzip_module && make && make install

ADD config/nginx/nginx.conf /usr/local/openresty/nginx/conf/nginx.conf
ADD lib/resty /usr/local/openresty/nginx/jwt-lua/resty
ADD start.sh /
RUN chmod +x start.sh
#RUN git clone https://github.com/aurorazl/lua-resty-jwt.git && mkdir /usr/local/openresty/nginx/jwt-lua/ &&  mv lua-resty-jwt/lib/resty /usr/local/openresty/nginx/jwt-lua/ &&\
#    mv lua-resty-jwt/config/nginx/nginx.conf /usr/local/openresty/nginx/conf/nginx.conf

