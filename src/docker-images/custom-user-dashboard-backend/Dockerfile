FROM node:12

RUN mkdir -p /home/addon_custom_user_dasboard_backend
WORKDIR /home/addon_custom_user_dasboard_backend
RUN git clone --branch release-0.0.3-beta https://5ad9420031b357cdd1f9f9cea5022c19c0ea2da1@github.com/apulis/addon_custom_user_dashboard_backend.git /home/addon_custom_user_dasboard_backend
RUN yarn config set registry 'https://registry.npm.taobao.org'
RUN yarn
RUN yarn build

EXPOSE 5001

CMD ["yarn", "start:prod"]
