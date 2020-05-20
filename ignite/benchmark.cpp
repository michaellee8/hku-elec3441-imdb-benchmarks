#include <iostream>
#include <ignite/thin/ignite_client.h>
#include <ignite/thin/ignite_client_configuration.h>
#include <ignite/thin/cache/cache_client.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/time.h>

using namespace ignite::thin;
using namespace std;

const int repeatTimes = 100000;

int main() {
  IgniteClientConfiguration cfg;
  cfg.SetEndPoints("db:10800");
  IgniteClient client = IgniteClient::Start(cfg);

  cache::CacheClient<std::string, std::string> cacheClient =
      client.GetOrCreateCache<std::string, std::string>("benchmark_cache");

  long start, end;
  struct timeval timecheck;

#define START_BENCHMARK gettimeofday(&timecheck, NULL); \
  start = (long) timecheck.tv_sec * 1000 + (long) timecheck.tv_usec / 1000;

#define END_BENCHMARK gettimeofday(&timecheck, NULL); \
  end = (long) timecheck.tv_sec * 1000 + (long) timecheck.tv_usec / 1000;

#define SHOW_BENCHMARK(name) printf("%ld milliseconds elapsed in %s benchmark\n", (end - start), name);

#define FLUSHALL cacheClient.Clear();

  FLUSHALL

  START_BENCHMARK
  for (int i = 0; i < repeatTimes; i++) {
    auto str = to_string(i);
    cacheClient.Put(str, str);
  }
  END_BENCHMARK
  SHOW_BENCHMARK("raw write")

  START_BENCHMARK
  for (int i = 0; i < repeatTimes; i++) {
    auto str = to_string(i);
    cacheClient.Get(str);
  }
  END_BENCHMARK
  SHOW_BENCHMARK("raw read")

  FLUSHALL

  START_BENCHMARK
  for (int i = 0; i < repeatTimes; i++) {
    auto str = to_string(i);
    cacheClient.Put(str, str);
    cacheClient.Get(str);
  }
  END_BENCHMARK
  SHOW_BENCHMARK("write then read")

}