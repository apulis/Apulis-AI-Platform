FROM node:12

RUN mkdir -p /home/AIArts
WORKDIR /home/AIArts
RUN git clone https://0d7d3ce231a0f47c97b62c0d7e3ed1211f8218c3@github.com/apulis/NewObjectLabel.git /home/AIArts

RUN yarn config set registry 'https://registry.npm.taobao.org'
RUN yarn
RUN yarn build

EXPOSE 3085

CMD ["node", "server.js"]