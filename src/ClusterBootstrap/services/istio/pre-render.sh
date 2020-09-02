curl -L https://istio.io/downloadIstio | ISTIO_VERSION=1.6.8 sh -
istio-1.6.8/bin/istioctl install -f istio.yaml --set values.global.jwtPolicy=first-party-jwt --force