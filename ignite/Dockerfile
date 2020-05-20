FROM buildpack-deps:stretch

RUN apt-get update && apt-get upgrade && apt-get -y install git libssl1.0-dev gcc g++-multilib && \
    git clone https://github.com/apache/ignite.git --branch 2.7.6 --depth 1

WORKDIR ignite/modules/platforms/cpp

RUN libtoolize && aclocal && autoheader && automake --add-missing && autoreconf && \
    ./configure --disable-odbc --disable-node --enable-thin-client --disable-core --disable-tests && \
    make -j4 && make install

COPY benchmark.cpp .

RUN g++ -g -o benchmark -lc -lstdc++ -L /usr/local/lib benchmark.cpp /usr/local/lib/libignite-binary.a /usr/local/lib/libignite-common.a /usr/local/lib/libignite-thin-client.a

CMD ["./benchmark"]