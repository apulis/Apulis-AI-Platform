FROM node:erbium

RUN mkdir /usr/src/app
WORKDIR /usr/src/app

ENV NODE_OPTIONS --max-http-header-size=65536

COPY package.json yarn.lock ./
RUN yarn --frozen-lockfile

COPY . .
RUN npm run build

ENV HOST 0.0.0.0
ENV PORT 3000

VOLUME /usr/src/app/config
EXPOSE 3000

CMD npm start --production
