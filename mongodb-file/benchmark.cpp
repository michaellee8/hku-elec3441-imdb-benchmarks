#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/time.h>
#include <bsoncxx/v_noabi/bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/v_noabi/bsoncxx/json.hpp>
#include <bsoncxx/v_noabi/bsoncxx/document/value.hpp>
#include <mongocxx/v_noabi/mongocxx/instance.hpp>
#include <mongocxx/v_noabi/mongocxx/uri.hpp>
#include <mongocxx/v_noabi/mongocxx/collection.hpp>
#include <mongocxx/v_noabi/mongocxx/client.hpp>
#include <assert.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>

const int repeatTimes = 100000;

using namespace std;

int main() {

  long start, end;
  struct timeval timecheck;

#define START_BENCHMARK gettimeofday(&timecheck, NULL); \
  start = (long) timecheck.tv_sec * 1000 + (long) timecheck.tv_usec / 1000;

#define END_BENCHMARK gettimeofday(&timecheck, NULL); \
  end = (long) timecheck.tv_sec * 1000 + (long) timecheck.tv_usec / 1000;

#define SHOW_BENCHMARK(name) printf("%ld milliseconds elapsed in %s benchmark\n", (end - start), name); fflush(stdout);

  mongocxx::instance inst{}; // This should be done only once.
  mongocxx::uri mongo_uri("mongodb://db:27017");
  mongocxx::client client(mongo_uri);

  mongocxx::collection coll = client["benchmark_db"]["benchmark"];

#define FLUSHALL coll.drop();

  FLUSHALL

  auto builder = bsoncxx::builder::stream::document{};
  builder << "key" << 1;
  coll.create_index(builder.view());

  START_BENCHMARK
  for (int i = 0; i < repeatTimes; i++) {
    auto str = to_string(i);
    auto builder = bsoncxx::builder::stream::document{};
    builder
        << "key" << str
        << "value" << str;
    coll.insert_one(builder.view());
  }
  END_BENCHMARK
  SHOW_BENCHMARK("raw write")

  START_BENCHMARK
  for (int i = 0; i < repeatTimes; i++) {
    auto builder = bsoncxx::builder::stream::document{};
    builder << "key" << to_string(i);
    coll.find_one(builder.view());
  }
  END_BENCHMARK
  SHOW_BENCHMARK("raw read")

  FLUSHALL
  coll.create_index(builder.view());
  START_BENCHMARK
  for (int i = 0; i < repeatTimes; i++) {
    auto str = to_string(i);
    auto builder = bsoncxx::builder::stream::document{};
    builder
        << "key" << str
        << "value" << str;
    coll.insert_one(builder.view());
    auto builder2 = bsoncxx::builder::stream::document{};
    builder2 << "key" << to_string(i);
    coll.find_one(builder2.view());
  }
  END_BENCHMARK
  SHOW_BENCHMARK("write then read");

}