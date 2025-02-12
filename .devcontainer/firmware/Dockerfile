FROM mcr.microsoft.com/devcontainers/cpp:1-debian-12 AS install-esp-idf

USER root

RUN apt-get update && apt-get -y install --no-install-recommends \
    git \
    wget \
    flex \
    bison \
    gperf \
    python3 \
    python3-venv \
    cmake \
    ninja-build \
    ccache \
    libffi-dev \
    libssl-dev \
    dfu-util \
    libusb-1.0-0

ARG ESP_IDF_VERSTION=v5.3.2
ARG TARGET=esp32c6

RUN mkdir -p /opt && cd /opt \
    && git clone --filter=blob:none --no-checkout --depth 1 --single-branch -b ${ESP_IDF_VERSTION} --recursive https://github.com/espressif/esp-idf.git \
    && cd /opt/esp-idf \
    && git sparse-checkout init --cone \
    && git sparse-checkout set components tools \
    && git checkout \
    &&  git submodule update --init --recursive --recommend-shallow --depth 1

USER vscode

RUN /opt/esp-idf/install.sh ${TARGET}

FROM mcr.microsoft.com/devcontainers/base:bookworm

COPY --from=install-esp-idf --chown=vscode /opt/esp-idf /opt/esp-idf
COPY --from=install-esp-idf --chown=vscode /home/vscode/.espressif /home/vscode/.espressif

USER root

RUN apt-get update \
    && apt-get install --no-install-recommends -y \
    ninja-build \
    cmake \
    llvm-dev \
    libclang-dev \
    libuv1-dev \
    libudev-dev \
    libusb-1.0-0 \
    libssl-dev \
    pkg-config \
    && apt-get clean -y \
    && rm -rf /var/lib/apt/lists/* /tmp/library-scripts

ARG CONTAINER_USER=vscode
USER ${CONTAINER_USER}

RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- \
    --default-toolchain none -y --profile minimal

ENV PATH=${PATH}:/home/${CONTAINER_USER}/.cargo/bin

RUN rustup default nightly \
    && cargo install espup ldproxy cargo-espflash espflash \
    && rustup toolchain install nightly --component rust-src
ARG TARGET=esp32c6

RUN /home/${CONTAINER_USER}/.cargo/bin/espup install \
    --targets "${TARGET}" \
    --log-level debug \
    --export-file /home/${CONTAINER_USER}/export-esp.sh

RUN echo "source /opt/esp-idf/export.sh" >> /home/${CONTAINER_USER}/.bashrc \
    && echo "source /home/${CONTAINER_USER}/export-esp.sh" >> /home/${CONTAINER_USER}/.bashrc
