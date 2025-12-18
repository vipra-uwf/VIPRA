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

# TODO(tylerk): Should this even be an argument? Or should this just be done always or make it an option to not compile?
ARG compile

RUN if [ "${compile}" -eq 1 ] ; then \
        rm -rf build ; \
        mkdir build ; \
        make release ; \
        echo "Compilation complete." ; \
        find build -name "libvipra*.so*" -type f -exec cp {} /usr/local/lib/ \; ; \
        find build -name "libantlr4-runtime*.so*" -type f -exec cp {} /usr/local/lib/ \; ; \
        find build -name "libdxfrw*.so*" -type f -exec cp {} /usr/local/lib/ \; ; \
        find . -name "vipra" -type f -executable -exec cp {} /usr/local/bin/ \; ; \
        ldconfig ; \
    fi
