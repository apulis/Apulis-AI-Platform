FROM golang:1.15.0 AS build
RUN mkdir -p /go/src/github.com/knative && git clone -b v0.15.0 https://github.com/knative-sandbox/net-istio.git /go/src/github.com/knative/net-istio
WORKDIR /go/src/github.com/knative/net-istio/cmd
RUN cd webhook && CGO_ENABLED=0 go build \
    mkdir -p /ko-app; \
    mkdir -p ./kodata; \
    cp -RL ./kodata /var/run/ko; \
    cp activator /ko-app/;

FROM debian:stable-slim
COPY --from=build /ko-app /ko-app
COPY --from=build /var/run/ko /var/run/ko
ENV PATH="/ko-app:${PATH}" KO_DATA_PATH=/var/run/ko
ENTRYPOINT ["/ko-app/webhook"]