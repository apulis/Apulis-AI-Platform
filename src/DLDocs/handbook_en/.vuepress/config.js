module.exports = {
  base : '/docs/en/handbook/',
  title:'Apulis AI Development Platform',
  dest:'./dist/en/handbook',
  themeConfig: {
      displayAllHeaders: true,
      lastUpdated: 'Update ',
      sidebarDepth:2,
      navbar:false,
      sidebar: [
          {
              title: '1 Product Descriptions', 
              path: '/', 
              sidebarDepth: 1
          },
          {
              title: '2 Operation Instructions',
              path: '/Login and Registration',
              collapsable: false,
              children: [
                  {
                      title: '2.1 Login and Registration',
                      path: '/Login and Registration',
                      sidebarDepth:2,
                  },
                  {
                      title: '2.2 Home Page',
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
                  // {
                  //     title: '2.8 报警配置',
                  //     path: '/报警配置',
                  //     sidebarDepth:2,
                  // },
              ]
          },
      ]
      ,
  }
}