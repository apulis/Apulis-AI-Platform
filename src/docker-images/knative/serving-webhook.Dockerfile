FROM golang:1.15.0 AS build
RUN mkdir -p /go/src/github.com/knative && git clone -b master https://github.com/knative/serving.git /go/src/github.com/knative/serving
WORKDIR /go/src/github.com/knative/serving/cmd
RUN cd webhook && go build && cp ./webhook /usr/local/bin

FROM debian:stable-slim
COPY --from=build /usr/local/bin/webhook .
ENTRYPOINT ["./webhook"]