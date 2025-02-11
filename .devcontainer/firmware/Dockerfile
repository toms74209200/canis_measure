FROM mcr.microsoft.com/devcontainers/base:bookworm

USER root

RUN apt-get update \
    && apt-get install -y ninja-build \
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
    && rustup toolchain install nightly --component rust-src \
    && rustup target add riscv32imac-unknown-none-elf
ARG TARGET=all

RUN /home/${CONTAINER_USER}/.cargo/bin/espup install \
    --targets "${TARGET}" \
    --log-level debug \
    --export-file /home/${CONTAINER_USER}/export-esp.sh

RUN echo "source /home/${CONTAINER_USER}/export-esp.sh" >> /home/${CONTAINER_USER}/.bashrc
