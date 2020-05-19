#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <libmemcached/memcached.h>

const int repeatTimes = 1000000;

int main() {

  long start, end;
  struct timeval timecheck;

#define START_BENCHMARK gettimeofday(&timecheck, NULL); \
  start = (long) timecheck.tv_sec * 1000 + (long) timecheck.tv_usec / 1000;

#define END_BENCHMARK gettimeofday(&timecheck, NULL); \
  end = (long) timecheck.tv_sec * 1000 + (long) timecheck.tv_usec / 1000;

#define SHOW_BENCHMARK(name) printf("%ld milliseconds elapsed in %s benchmark\n", (end - start), name);

  const char *configString = "--SERVER=db";

  memcached_st *memc = memcached(configString, strlen(configString));

#define FLUSHALL memcached_flush(memc, (time_t) 0);

  FLUSHALL

  START_BENCHMARK
  for (int i = 0; i < repeatTimes; i++) {
    char str[20];
    sprintf(str, "%d", i);
    size_t vlen = strlen(str);
    memcached_return_t rc = memcached_set(memc, str, vlen, str, vlen, (time_t) 0, (uint32_t) 0);
    if (rc != MEMCACHED_SUCCESS) {
      printf("memcached raw write failed at %d", i);
      return 1;
    }
  }
  END_BENCHMARK
  SHOW_BENCHMARK("raw write")

  START_BENCHMARK
  for (int i = 0; i < repeatTimes; i++) {
    char str[20];
    sprintf(str, "%d", i);
    size_t vlen = strlen(str);
    memcached_return_t rc;
    size_t value_len;
    memcached_get(memc, str, vlen, &value_len, NULL, &rc);
    if (rc != MEMCACHED_SUCCESS) {
      printf("memcached raw read failed at %d", i);
      return 1;
    }
  }
  END_BENCHMARK
  SHOW_BENCHMARK("raw read")

  FLUSHALL
  START_BENCHMARK
  for (int i = 0; i < repeatTimes; i++) {
    char str[20];
    sprintf(str, "%d", i);
    size_t vlen = strlen(str);
    memcached_return_t rc = memcached_set(memc, str, vlen, str, vlen, (time_t) 0, (uint32_t) 0);
    if (rc != MEMCACHED_SUCCESS) {
      printf("memcached write then read write failed at %d", i);
      return 1;
    }
    size_t value_len;
    memcached_get(memc, str, vlen, &value_len, NULL, &rc);
    if (rc != MEMCACHED_SUCCESS) {
      printf("memcached write then read read failed at %d", i);
      return 1;
    }
  }
  END_BENCHMARK
  SHOW_BENCHMARK("write then read");

  memcached_free(memc);

}