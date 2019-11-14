FROM node:dubnium
ENV NODE_ENV production
WORKDIR /usr/src/app

ARG registry=https://registry.npm.taobao.org
ARG disturl=https://npm.taobao.org/dist
RUN yarn config set disturl $disturl
RUN yarn config set registry $registry

COPY package.json ./
RUN yarn --frozen-lockfile
# RUN yarn build

COPY . .
EXPOSE 3081
CMD node server