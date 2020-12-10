FROM golang:1.15.0 AS build
RUN go get -u github.com/google/ko/cmd/ko
RUN mkdir -p /go/src/github.com/knative && git clone -b v0.15.0 https://github.com/knative/serving.git /go/src/github.com/knative/serving && git clone -b v0.15.0  https://github.com/knative-sandbox/net-istio.git /go/src/github.com/knative/net-istio
WORKDIR /go/src/github.com/knative/serving
RUN ko publish ./cmd/activator/ --push=false --local --base-import-paths=true && docker tag ko.local/activator apulistech/knative-serving-activator
RUN ko publish ./cmd/controller/ --push=false --local --base-import-paths=true && docker tag ko.local/controller apulistech/knative-serving-controller
RUN ko publish ./cmd/autoscaler/ --push=false --local --base-import-paths=true && docker tag ko.local/autoscaler apulistech/knative-serving-autoscaler
RUN ko publish ./cmd/queue/ --push=false --local --base-import-paths=true && docker tag ko.local/queue apulistech/knative-serving-queue
RUN ko publish ./cmd/webhook/ --push=false --local --base-import-paths=true && docker tag ko.local/queue apulistech/knative-serving-webhook
WORKDIR /go/src/github.com/knative/net-istio
RUN ko publish ./cmd/webhook/ --push=false --local --base-import-paths=true && docker tag ko.local/queue apulistech/knative-net-istio-webhook
RUN ko publish ./cmd/controller/ --push=false --local --base-import-paths=true && docker tag ko.local/queue apulistech/knative-net-istio-controller