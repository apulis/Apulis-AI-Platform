# Deployment of QianJiangYuan in China

## app-get install fails due to 

*** Error in `appstreamcli': double free or corruption (fasttop): 0x000000000210f4b0 ***

Solution:

```
sudo apt-get purge libappstream3
```
or
```
sudo apt install --reinstall libappstream3
```


