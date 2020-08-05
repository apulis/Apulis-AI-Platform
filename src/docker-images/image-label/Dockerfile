FROM node:12

RUN mkdir -p /home/AIArts
WORKDIR /home/AIArts
RUN git clone https://5ad9420031b357cdd1f9f9cea5022c19c0ea2da1@github.com/apulis/NewObjectLabel.git /home/AIArts

RUN yarn config set registry 'https://registry.npm.taobao.org'
RUN yarn
RUN yarn build

EXPOSE 3085

CMD ["node", "server.js"]