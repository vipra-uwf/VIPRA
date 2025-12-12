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

ARG map_file
ARG pedestrian_file
ARG modules_file
ARG params_file
ARG compile

# RUN if [ "${compile}" -eq 1 ] ; then \
#         rm -rf build ; \
#         # Configure build into 'build' and set executable install location to /VIPRA
#         cmake -B build -S . -DVIPRA_MAIN_FILE=main.cpp -DVIPRA_EXECUTABLE_PATH=/VIPRA -DVIPRA_BUILD_EXECUTABLE=ON ; \
#         # Build
#         cmake --build build -- -j$(nproc) ; \
#         # Install artifacts into /usr/local (library -> /usr/local/lib, binaries -> /usr/local/bin)
#         cmake --install build --prefix /usr/local ; \
#         ldconfig ; \
#     fi

RUN if [ "${compile}" -eq 1 ] ; then \
        rm -rf build ; \
        make ; \
        echo "Compilation complete." ; \
    fi

RUN echo "map: ${map_file}"
RUN echo "peds: ${pedestrian_file}"
RUN echo "modules: ${modules_file}"
RUN echo "params: ${params_file}"

# Run the executable. The shared library is installed into /usr/local/lib above
RUN ./vipra -map=${map_file} -peds=${pedestrian_file} -params=${params_file} -modules=${modules_file}

# TODO: UI or Interactive mode?