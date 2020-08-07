FROM node:dubnium

RUN mkdir /usr/src/app
WORKDIR /usr/src/app

ARG registry=https://registry.npm.taobao.org
ARG disturl=https://npm.taobao.org/dist
RUN yarn config set disturl $disturl
RUN yarn config set registry $registry

COPY package.json yarn.lock ./
RUN yarn --frozen-lockfile

COPY . .
RUN yarn build

EXPOSE 3081
CMD node server