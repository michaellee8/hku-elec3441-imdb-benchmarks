FROM gcc:9

RUN apt-get update && apt-get install -y cmake

WORKDIR /app

COPY . .

WORKDIR /app/build

RUN cmake ..

RUN make

CMD ["./benchmark"]