FROM golang:1.15.0 AS build
RUN mkdir -p /go/src/github.com/knative && git clone -b v0.15.1 https://github.com/knative/serving.git /go/src/github.com/knative/serving
WORKDIR /go/src/github.com/knative/serving/cmd
RUN cd autoscaler && go build && cp ./autoscaler /usr/local/bin

FROM debian:stable-slim
COPY --from=build /usr/local/bin/autoscaler .
ENTRYPOINT ["./autoscaler"]