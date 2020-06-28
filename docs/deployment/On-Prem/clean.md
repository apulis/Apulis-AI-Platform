## 清除集群文档


1. 清除所有的worker信息
    ```
    ./deploy.py cleanworker
    ```
    
2. 清除master集群信息
    ```shell script
	./deploy.py kubeadm reset
    ```

3. 卸载共享盘
    ```shell script
    ./deploy.py mount stop
    ```

3. 清除master的配置信息
    ```shell script
    ./deploy.py cleanmasteretcd
    ./deploy.py clean
    ```


