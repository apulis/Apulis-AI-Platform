FROM golang:1.11-alpine AS build
RUN apk add --update make git
RUN mkdir -p /go/src/github.com/brancz && git clone -b release-0.2 https://github.com/brancz/kube-rbac-proxy.git /go/src/github.com/brancz/kube-rbac-proxy
WORKDIR /go/src/github.com/brancz/kube-rbac-proxy
RUN make build && \
 if [ "$(go env GOARCH)" = "arm64" ]; then \
    cp /go/src/github.com/brancz/kube-rbac-proxy/_output/linux/aarch64/kube-rbac-proxy /usr/local/bin;\
 else \
    cp /go/src/github.com/brancz/kube-rbac-proxy/_output/linux/amd64/kube-rbac-proxy /usr/local/bin; \
 fi

FROM alpine:3.8
RUN apk add -U --no-cache ca-certificates && rm -rf /var/cache/apk/*
COPY --from=build /usr/local/bin/kube-rbac-proxy .
ENTRYPOINT ["./kube-rbac-proxy"]
EXPOSE 8080