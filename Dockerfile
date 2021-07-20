FROM ubuntu:hirsute

WORKDIR /workspace

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -yq \
    git wget build-essential python3 clang \
    && rm -rf /var/lib/apt/lists/*

ADD https://github.com/bazelbuild/bazelisk/releases/download/v1.10.1/bazelisk-linux-amd64 /usr/local/bin/bazelisk
RUN chmod +x /usr/local/bin/bazelisk

ADD . /workspace

RUN bazelisk build //...
