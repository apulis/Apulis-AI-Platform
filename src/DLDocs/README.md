## 手册文档更新后，如何修改更新项目中的md文件以达到手册页面更新？
### 1.使用Typora修改md
###### 为什么不直接在VScode中修改？
- 对编辑md而言，VScode没有Typora方便，比如在VScode中，图片不能直接复制粘贴进来，而在Typora中可以。
###### 使用Typora编辑项目中md需要注意些什么？
- 首先明确，在项目中，md文件引用的图片静态资源统一都放在/docs/assets文件夹下。
- 为了保证我们在使用Typora编辑时插入图片资源后的路径能和项目中的引用路径一致，进而减少麻烦，我们在用Typora打开md时，需以/docs作为文件根路径打开，并对Typora做如下配置。

![image-20200827175754323](https://github.com/apulis/DLDocs/raw/master/docs/assets/image-20200827175754323.png)

### 2.在VScode中调整图片路径
- 在经过第一步使用Typora编辑完成后，图片资源的路径依然是错误的，需要把docs路径下的所有图片的路径，统一搜索替换从绝对路径方式(/assets)改成相对路径方式(./assets)。

## 部署更新
### 1.同步dockerfile
更新docker-images/dldocs下的dockerfile文件
### 2.拉取代码
```shell script
ssh root@10.31.3.108
1
cd /root/DLWorkspace/src/ClusterBootstrap
git checkout develop
git pull
```
### 3.编译镜像

```shell script
./deploy.py docker push dldocs
```

### 4.更新服务
编译完镜像后，执行
```shell script
./deploy.py kubernetes stop dldocs
./deploy.py kubernetes start dldocs
```