FROM ubuntu:22.04 AS builder

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libncurses5-dev \
    libncursesw5-dev \
    nlohmann-json3-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

RUN mkdir -p build && cd build && \
    cmake .. && \
    cmake --build . -j$(nproc)

RUN cd build && ctest --output-on-failure

FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    libncurses5 \
    libncursesw5 \
    && rm -rf /var/lib/apt/lists/*

COPY --from=builder /app/build/bin/ascii_paint /usr/local/bin/
COPY --from=builder /app/build/bin/test_* /usr/local/bin/
COPY --from=builder /app/build/bin/scenario_* /usr/local/bin/

WORKDIR /workspace

CMD ["/usr/local/bin/ascii_paint", "--help"]