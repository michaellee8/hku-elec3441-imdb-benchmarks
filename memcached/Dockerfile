FROM gcc:9

RUN apt-get install libmemcached-dev

COPY benchmark.c .

RUN gcc -o benchmark -lmemcached benchmark.c

CMD ["./benchmark"]