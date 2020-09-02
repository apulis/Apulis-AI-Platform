FROM golang:1.15.0 AS build
RUN mkdir -p /go/src/github.com/knative && git clone -b release-0.15 https://github.com/knative/serving.git /go/src/github.com/knative/serving
WORKDIR /go/src/github.com/knative/serving/cmd
RUN cd activator && go build && cp ./activator /usr/local/bin

FROM debian:stable-slim
COPY --from=build /usr/local/bin/activator .
ENTRYPOINT ["./activator"]