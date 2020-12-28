@Library('apulis-build@master') _

buildPlugin ( {
    repoName = 'DLWorkspace'
    dockerImages = [
        [
            'imageName': 'cicd/dlworkspace-compile',
            'directory': 'dlworkspace-compile/dlworkspace-compile-amd64 ',
            'arch': ['amd64']
        ],
        [
            'imageName': 'cicd/dlworkspace-compile',
            'directory': 'dlworkspace-compile/dlworkspace-compile-arm64 ',
            'arch': ['arm64']
        ],
        [
            'compileContainer': 'cicd/dlworkspace-compile',
            'preBuild':[
                ['src/ClusterBootstrap', 'mkdir -p build'],
                ['src/ClusterBootstrap/build', 'rm -rf restfulapi2'],
                ['src/ClusterBootstrap', 'pip install -r scripts/requirements.txt'],
                ['src/ClusterManager', 'pip install -r requirements.txt'],
                ['src/ClusterBootstrap/build', 'cp -r ../../docker-images/restfulapi2 .'],
                ['src/ClusterBootstrap', 'cp config.yaml.template config.yaml'],
                ['src/ClusterBootstrap', 'python ./deploy.py rendertemplate ../docker-images/restfulapi2/ports.conf build/restfulapi2/ports.conf'],
                ['src/ClusterBootstrap', 'python ./deploy.py rendertemplate ../docker-images/restfulapi2/000-default.conf build/restfulapi2/000-default.conf'],
                ['src/ClusterBootstrap/build/restfulapi2', 'cp -r ../../../Jobs_Templete Jobs_Templete'],
                ['src/ClusterBootstrap/build/restfulapi2', 'cp -r ../../../utils utils'],
                ['src/ClusterBootstrap/build/restfulapi2', 'cp -r ../../../RestAPI RestAPI'],
                ['src/ClusterBootstrap/build/restfulapi2', 'cp -r ../../../ClusterManager ClusterManager'],
                ['src/ClusterBootstrap/build/restfulapi2', 'cp ../../../../version-info version-info']
            ],
            'imageName': 'apulistech/restfulapi2',
            'directory': 'src/ClusterBootstrap/build/restfulapi2',
            'arch': ['amd64','arm64']
        ],
        [
            'compileContainer': 'cicd/dlworkspace-compile',
            'preBuild':[
                ['src/ClusterBootstrap', 'mkdir -p build/webui3'],
                ['src/ClusterBootstrap', 'pip install -r scripts/requirements.txt'],
                ['src/ClusterManager', 'pip install -r requirements.txt'],
                ['src/ClusterBootstrap/build', 'cp -r ../../docker-images/webui3 .'],
                ['src/ClusterBootstrap', 'cp config.yaml.template config.yaml'],
                ['src/ClusterBootstrap/build/webui3', 'cp -rf ../../../dashboard/* .']
            ],
            'imageName': 'apulistech/webui3',
            'directory': 'src/ClusterBootstrap/build/webui3',
            'arch': ['amd64','arm64']
        ]
    ]
})
