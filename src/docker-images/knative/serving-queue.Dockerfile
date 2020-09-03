FROM golang:1.15.0 AS build
RUN mkdir -p /go/src/github.com/knative && git clone -b v0.15.0 https://github.com/knative/serving.git /go/src/github.com/knative/serving
WORKDIR /go/src/github.com/knative/serving/cmd
RUN cd queue && go build && cp ./queue /usr/local/bin

FROM debian:stable-slim
COPY --from=build /usr/local/bin/queue .
ENV KO_DATA_PATH=/var/run/ko
ENTRYPOINT ["./queue"]