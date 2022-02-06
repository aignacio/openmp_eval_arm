FROM ubuntu:latest
LABEL author="Anderson Ignacio da Silva"
LABEL maintainer="anderson@aignacio.com"
RUN apt-get update && apt-get upgrade -y
RUN apt install file gcc make time -y
RUN apt install gcc-arm-linux-gnueabi binutils-arm-linux-gnueabi -y
RUN apt install gcc-aarch64-linux-gnu binutils-aarch64-linux-gnu -y
RUN apt install device-tree-compiler -y
