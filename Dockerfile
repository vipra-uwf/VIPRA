FROM ubuntu:24.04

# Update apt and clean up unnecessary files.
RUN rm -rf /var/lib/apt/lists/*
RUN apt clean
RUN apt update

# Install required packages.
RUN apt install -y build-essential
RUN apt install -y cmake
RUN apt install -y git
RUN apt install -y libjsoncpp-dev

RUN apt install -y ffmpeg 

RUN apt install -y python3-ezdxf
RUN apt install -y python3-matplotlib
RUN apt install -y python3-numpy

WORKDIR /VIPRA
COPY . .
