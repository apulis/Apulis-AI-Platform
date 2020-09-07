if [ "$1" = "clean" ] && [ "$2" = "knative" ];
then
    kubectl delete --filename https://github.com/knative/serving/releases/download/v0.15.0/serving-crds.yaml
    kubectl delete --filename https://github.com/knative/serving/releases/download/v0.15.0/serving-core.yaml
    kubectl delete --filename https://github.com/knative/net-istio/releases/download/v0.15.0/release.yaml
    kubectl delete -f https://raw.githubusercontent.com/kubeflow/kfserving/master/install/0.2.2/kfserving.yaml
    kubectl patch crd/routes.serving.knative.dev -p '{"metadata":{"finalizers":[]}}' --type=merge
fi

if [ "$1" = "install" ];
then
  if [ "$2" = "knative" ];
  then
    kubectl apply --filename https://github.com/knative/serving/releases/download/v0.15.0/serving-crds.yaml
    kubectl apply --filename https://github.com/knative/serving/releases/download/v0.15.0/serving-core.yaml
    kubectl apply --filename https://github.com/knative/net-istio/releases/download/v0.15.0/release.yaml
  elif [ "$2" = "kfserving" ];
  then
      kubectl apply -f https://raw.githubusercontent.com/kubeflow/kfserving/master/install/0.2.2/kfserving.yaml
  elif [ "$2" = "istio" ];
  then
      istioctl install -f istio-default.yaml --set values.global.jwtPolicy=first-party-jwt --force
  elif [ "$2" = "k8s" ];
  then
        sudo kubeadm init
        sudo chmod 777 /etc/kubernetes/admin.conf
        sudo cp /etc/kubernetes/admin.conf deploy/sshkey/
        sudo cp /etc/kubernetes/admin.conf ~/.kube/config
        kubectl apply -f "https://cloud.weave.works/k8s/net?k8s-version=v1.18.0"
  fi
fi


if [ "$1" = "clean" ] && [ "$2" = "ns" ];
then
  kubectl delete ns istio-system knative-serving cert-manager kfserving-system kubeflow auth
  kubectl delete validatingwebhookconfigurations --all
  kubectl delete mutatingwebhookconfigurations --all
  kubectl delete crds --all
elif [ "$1" = "clean" ] && [ "$2" = "ns2" ];
then
  for ns in istio-system knative-serving cert-manager kfserving-system kubeflow auth;do
    kubectl get namespace $ns -o json | tr -d "\n" | sed "s/\"finalizers\": \[[^]]\+\]/\"finalizers\": []/" | kubectl replace --raw /api/v1/namespaces/$ns/finalize -f -;done
elif [ "$1" = "clean" ] && [ "$2" = "istio" ];
then
    istioctl manifest generate --set profile=demo | kubectl delete -f -
fi

if [ "$1" = "run" ] && [ "$2" = "knative" ];
then
    kubectl apply --filename knative-docs/docs/serving/samples/hello-world/helloworld-python/service.yaml
fi

if [ "$1" = "run" ] && [ "$2" = "istio" ];
then
    kubectl apply -f istio/samples/bookinfo/platform/kube/bookinfo.yaml
    kubectl exec -it $(kubectl get pod -l app=ratings -o jsonpath='{.items[0].metadata.name}') -c ratings -- curl productpage:9080/productpage | grep -o "<title>.*</title>"
fi

if [ "$1" = "stop" ] && [ "$2" = "knative" ];
then
    kubectl delete --filename knative-docs/docs/serving/samples/hello-world/helloworld-python/service.yaml
fi

if [ "$1" = "stop" ] && [ "$2" = "istio" ];
then
    kubectl delete -f istio/samples/bookinfo/platform/kube/bookinfo.yaml
fi


archtype=`uname -m`
if [ $archtype = "aarch64" ];then
  tag=latest-arm64
else
  tag=latest
fi

if [ "$1" = "build" ];
then
    for file in ../docker-images/$2/*;do
      arr=(${file//./ })
      docker build -t apulistech/$2-`basename ${arr[0]}`:$tag -f $file "../docker-images/$2"
    done
fi

if [ "$1" = "push" ];
then
    for file in ../docker-images/$2/*;do
      arr=(${file//./ })
      docker build -t apulistech/$2-`basename ${arr[0]}`:$tag -f $file "../docker-images/$2"
      docker push apulistech/$2-`basename ${arr[0]}`:$tag
    done
fi

if [ "$1" = "pull" ];
then
  for file in ../docker-images/$2/*;do
      arr=(${file//./ })
      docker pull apulistech/$2-`basename ${arr[0]}`:$tag
  done
fi

if [ "$1" = "push2" ];
then
    for file in ../docker-images/$2/*;do
      arr=(${file//./ })
      docker tag apulistech/$2-`basename ${arr[0]}`:$tag harbor.sigsus.cn:8443/sz_gongdianju/apulistech/$2-`basename ${arr[0]}`:$tag
      docker push apulistech/$2-`basename ${arr[0]}`:$tag
    done
fi
