# Build the manager binary
FROM golang:1.10.3 as builder

# Copy in the go src
RUN mkdir -p /go/src/github.com/kubeflow && git clone -b 0.2.2 https://github.com/aurorazl/kfserving.git /go/src/github.com/kubeflow/kfserving
WORKDIR /go/src/github.com/kubeflow/kfserving
# Build
RUN if [ "$(uname -m)" = "ppc64le" ]; then \
        CGO_ENABLED=0 GOOS=linux GOARCH=ppc64le go build -a -o manager ./cmd/manager; \
    elif [ "$(uname -m)" = "aarch64" ]; then \
        CGO_ENABLED=0 GOOS=linux GOARCH=arm64 go build -a -o manager ./cmd/manager; \
    else \
        CGO_ENABLED=0 GOOS=linux GOARCH=amd64 go build -a -o manager ./cmd/manager; \
    fi

# Copy the controller-manager into a thin image
FROM ubuntu:latest
WORKDIR /
COPY --from=builder /go/src/github.com/kubeflow/kfserving/manager .
ENTRYPOINT ["/manager"]