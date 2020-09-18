FROM node:12

RUN mkdir -p /home/docs
WORKDIR /home/docs


ADD DLDocs/package.json .
# RUN yarn config set registry 'https://registry.npm.taobao.org'
RUN yarn

COPY DLDocs/. /home/docs

RUN yarn docs:build

EXPOSE 3386

CMD ["node", "server.js"]