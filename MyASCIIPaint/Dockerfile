FROM ubuntu:22.04 AS builder
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    nlohmann-json3-dev \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /app
COPY . .
RUN mkdir -p build && cd build && \
    cmake .. && \
    cmake --build . -j$(nproc)

FROM ubuntu:22.04
COPY --from=builder /app/build/bin/ascii_paint /usr/local/bin/
WORKDIR /workspace
CMD ["/usr/local/bin/ascii_paint"]