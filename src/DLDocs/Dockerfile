FROM node:12

RUN mkdir -p /home/handbook
WORKDIR /home/handbook


ADD package.json .
RUN yarn config set registry 'https://registry.npm.taobao.org'
RUN yarn

COPY . /home/handbook

RUN yarn handbook:build

EXPOSE 3386

CMD ["node", "server.js"]