module.exports = {
  base : '/docs/zh/handbook/',
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
              path: '/Login and Registration',
              collapsable: false,
              children: [
                  {
                      title: '2.1 注册和登录',
                      path: '/Login and Registration',
                      sidebarDepth:2,
                  },
                  {
                      title: '2.2 平台主页',
                      path: '/Home Page',
                      sidebarDepth:2,
                  },
                  {
                      title: '2.3 Submit Training Job',
                      path: '/Submit Training Job',
                      sidebarDepth:2,
                  },
                  {
                      title: '2.4 View and Manage Jobs',
                      path: '/View and Manage Jobs',
                      sidebarDepth:2,
                  },
                  {
                      title: '2.5 Cluster Status',
                      path: '/Cluster Status',
                      sidebarDepth:2,
                  },
                  {
                      title: '2.6 Visual Cluster',
                      path: '/Virtual Cluster',
                      sidebarDepth:2,
                  },
                  {
                      title: '2.7 user Dashboard',
                      path: '/User Dashboard',
                      sidebarDepth:2,
                  },
                  {
                      title: '2.8 报警配置',
                      path: '/Alarm configuration',
                      sidebarDepth:2,
                  },
              ]
          },
      ]
      ,
  }
}