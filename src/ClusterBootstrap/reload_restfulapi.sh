./deploy.py docker push restfulapi2
./deploy.py kubernetes stop  restfulapi2
./deploy.py kubernetes start  restfulapi2
kubectl get pod -A -w
