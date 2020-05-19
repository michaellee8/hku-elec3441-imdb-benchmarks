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

  memcached_st *memc = memcached(configString, strlen(configString);



  memcached_free(memc);

}