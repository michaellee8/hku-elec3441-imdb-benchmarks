FROM gcc:9

RUN apt-get update && apt-get install -y cmake libssl-dev libsasl2-dev libboost-dev

ADD https://github.com/mongodb/mongo-c-driver/releases/download/1.16.0/mongo-c-driver-1.16.0.tar.gz .

WORKDIR /

RUN tar xzf mongo-c-driver-1.16.0.tar.gz && \
    cd mongo-c-driver-1.16.0 && \
    mkdir cmake-build && \
    cd cmake-build && \
    cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF .. && \
    make install

WORKDIR /

ADD https://github.com/mongodb/mongo-cxx-driver/archive/r3.5.0.tar.gz .

RUN tar -xzf r3.5.0.tar.gz && \
    cd mongo-cxx-driver-r3.5.0/build && \
    cmake ..                                \
        -DCMAKE_BUILD_TYPE=Release          \
        -DBUILD_SHARED_AND_STATIC_LIBS=ON   \
        -DCMAKE_INSTALL_PREFIX=/usr/local   \
        -DBUILD_VERSION=3.5.0               \
        -DBSONCXX_POLY_USE_BOOST=1       && \
    make -j4 && make install

WORKDIR /

COPY benchmark.cpp .

RUN gcc --std=c++17 -g -lc -lstdc++ -o benchmark benchmark.cpp -I/usr/local/include \
    $(pkg-config --cflags --libs libmongocxx) \
    $(pkg-config --cflags --libs libbsoncxx) \
    $(pkg-config --cflags --libs libbson-1.0) \
    $(pkg-config --cflags --libs libmongoc-1.0)

ENV LD_LIBRARY_PATH=/usr/local/lib

CMD ["./benchmark"]