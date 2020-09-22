module.exports = {
  base : '/docs/zh/handbook',
  title:'依瞳人工智能开发平台',
  dest:'./dist/zh/handbook',
  themeConfig: {
      displayAllHeaders: true,
      lastUpdated: '最后更新于',
      sidebarDepth:2,
      navbar:false,
      sidebar: [
          {
              title: '1 产品简介', 
              path: '/', 
              sidebarDepth: 1
          },
          {
              title: '2 操作说明',
              path: '/注册和登录',
              collapsable: false,
              children: [
                  {
                      title: '2.1 注册和登录',
                      path: '/注册和登录',
                      sidebarDepth:2,
                  },
                  {
                      title: '2.2 平台主页',
                      path: '/平台主页',
                      sidebarDepth:2,
                  },
                  {
                      title: '2.3 Submit Training Job',
                      path: '/提交训练任务',
                      sidebarDepth:2,
                  },
                  {
                      title: '2.4 View and Manage Jobs',
                      path: '/查看管理任务',
                      sidebarDepth:2,
                  },
                  {
                      title: '2.5 Cluster Status',
                      path: '/集群状态',
                      sidebarDepth:2,
                  },
                  {
                      title: '2.6 Visual Cluster',
                      path: '/虚拟集群',
                      sidebarDepth:2,
                  },
                  {
                      title: '2.7 user Dashboard',
                      path: '/用户仪表盘',
                      sidebarDepth:2,
                  },
                  {
                      title: '2.8 报警配置',
                      path: '/报警配置',
                      sidebarDepth:2,
                  },
              ]
          },
      ]
      ,
  }
}