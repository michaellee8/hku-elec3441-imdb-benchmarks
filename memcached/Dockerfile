FROM gcc:9

RUN apt-get update && apt-get install -y libmemcached-dev

COPY benchmark.c .

RUN gcc -o benchmark -lmemcached benchmark.c

CMD ["./benchmark"]