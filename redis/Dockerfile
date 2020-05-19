FROM gcc:9

RUN apt-get -y install git && \
    git clone https://github.com/redis/hiredis.git

WORKDIR ./hiredis

RUN make && \
    make install

COPY benchmark.c .

RUN gcc -o benchmark benchmark.c libhiredis.a

CMD ["./benchmark"]