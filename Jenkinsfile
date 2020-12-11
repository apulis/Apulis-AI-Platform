@Library('apulis-build@master') _

buildPlugin ( {
    repoName = "DLWorkspace"
    dockerImages = [
        [
            "compileContainer": "cicd/dlworkspace-comile",
            "preBuild":[
                "src/ClusterBootstrap": "mkdir -p build",
                "src/ClusterBootstrap": "pip install -r scripts/requirements.txt",
                "src/ClusterManager": "pip install -r requirements.txt",
                "src/ClusterBootstrap/build": "cp -r ../../docker-images/restfulapi2 .",
                "src/ClusterBootstrap": "./deploy.py rendertemplate ../docker-images/restfulapi2/ports.conf build/restfulapi2/ports.conf",
                "src/ClusterBootstrap": "./deploy.py rendertemplate ../docker-images/restfulapi2/000-default.conf build/restfulapi2/000-default.conf",
                "src/ClusterBootstrap/build/restfulapi2": "cp -r ../../../Jobs_Templete Jobs_Templete",
                "src/ClusterBootstrap/build/restfulapi2": "cp -r ../../../utils utils",
                "src/ClusterBootstrap/build/restfulapi2": "cp -r ../../../RestAPI RestAPI",
                "src/ClusterBootstrap/build/restfulapi2": "cp -r ../../../ClusterManager ClusterManager",
                "src/ClusterBootstrap/build/restfulapi2": "cp ../../../../version-info version-info"
            ]
            "imageName": "apulistech/restfulapi2",
            "directory": "src/ClusterBootstrap/build/restfulapi2"
        ]
    ]
})