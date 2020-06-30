FROM node:12

RUN mkdir -p /home/custom-user-dashboard
WORKDIR /home/custom-user-dashboard
#RUN git clone --branch release-0.0.2-beta https://{% if "user" in cnf["dockers"] %}{{cnf["dockers"]["user"]}}:{{cnf["dockers"]["password"]}}@{% endif %}github.com/apulis/addon_custom_user_group_dashboard.git /home/custom-user-dashboard
RUN git clone --branch release-0.0.3-beta https://5ad9420031b357cdd1f9f9cea5022c19c0ea2da1@github.com/apulis/addon_custom_user_group_dashboard.git /home/custom-user-dashboard
RUN yarn config set registry 'https://registry.npm.taobao.org'
RUN yarn
RUN yarn build

EXPOSE 3083

CMD ["yarn", "run", "static"]
