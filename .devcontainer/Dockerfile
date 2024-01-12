FROM mcr.microsoft.com/devcontainers/base:jammy
ARG ALIRE_URL="https://github.com/alire-project/alire/releases/download/v1.2.2/alr-1.2.2-bin-x86_64-linux.zip"
RUN apt update \
    && apt install -y \
        curl \
        gnat \
    && curl -Lo alire.zip $ALIRE_URL \
    && unzip -p alire.zip bin/alr > /usr/bin/alr \
    && chmod 0755 /usr/bin/alr \
    && rm alire.zip
