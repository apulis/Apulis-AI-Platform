FROM apulistech/openresty:base

ADD config/nginx/nginx.conf /usr/local/openresty/nginx/conf/nginx.conf
ADD lib/resty /usr/local/openresty/nginx/jwt-lua/resty
ADD start.sh /
RUN chmod +x start.sh
#RUN git clone https://github.com/aurorazl/lua-resty-jwt.git && mkdir /usr/local/openresty/nginx/jwt-lua/ &&  mv lua-resty-jwt/lib/resty /usr/local/openresty/nginx/jwt-lua/ &&\
#    mv lua-resty-jwt/config/nginx/nginx.conf /usr/local/openresty/nginx/conf/nginx.conf

