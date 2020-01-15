FROM node:erbium AS builder

RUN mkdir /usr/src/app
WORKDIR /usr/src/app

ENV NODE_OPTIONS --max-http-header-size=65536

COPY package.json yarn.lock ./
RUN yarn --frozen-lockfile

COPY . .
RUN npm run build

#ENV HOST 0.0.0.0
#ENV PORT 3000
#
#VOLUME /usr/src/app/config
#EXPOSE 3000
#
#CMD npm start --production

FROM nginx:alpine
# configure ports and listeners with custom nginx.conf
RUN rm /etc/nginx/conf.d/default.conf
ADD default.conf /etc/nginx/conf.d/

# copy from dist to nginx root dir
COPY --from=builder /usr/src/app/ /usr/share/nginx/html/

ADD start.sh /

RUN chmod +x /start.sh

EXPOSE 6601

# run nginx in foreground
ENTRYPOINT ./start.sh
