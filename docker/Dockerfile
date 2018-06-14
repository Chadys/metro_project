FROM ubuntu:16.04
RUN apt-get update
RUN apt-get install -y git python-dev build-essential
RUN apt-get update
RUN apt-get install -y cmake

#Install Emscripten
RUN mkdir -p /opt/code/
RUN cd /opt/ && git clone https://github.com/juj/emsdk.git
RUN cd /opt/emsdk && \
    ./emsdk install latest && \
    ./emsdk activate latest

WORKDIR /opt/code/
EXPOSE 8084