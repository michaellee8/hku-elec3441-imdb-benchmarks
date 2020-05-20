#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/time.h>
#include <libmemcached/memcached.h>
#include <assert.h>

const int repeatTimes = 100000;

char *get_ip_str(const struct sockaddr *sa, char *s, size_t maxlen) {
  switch (sa->sa_family) {
    case AF_INET:
      inet_ntop(AF_INET, &(((struct sockaddr_in *) sa)->sin_addr),
                s, maxlen);
      break;

    case AF_INET6:
      inet_ntop(AF_INET6, &(((struct sockaddr_in6 *) sa)->sin6_addr),
                s, maxlen);
      break;

    default:strncpy(s, "Unknown AF", maxlen);
      return NULL;
  }

  return s;
}

int main() {

  long start, end;
  struct timeval timecheck;

#define START_BENCHMARK gettimeofday(&timecheck, NULL); \
  start = (long) timecheck.tv_sec * 1000 + (long) timecheck.tv_usec / 1000;

#define END_BENCHMARK gettimeofday(&timecheck, NULL); \
  end = (long) timecheck.tv_sec * 1000 + (long) timecheck.tv_usec / 1000;

#define SHOW_BENCHMARK(name) printf("%ld milliseconds elapsed in %s benchmark\n", (end - start), name);

  struct addrinfo *dbinfo;
  int error = getaddrinfo("db", NULL, NULL, &dbinfo);
  if (error != 0) {
    if (error == EAI_SYSTEM) {
      perror("getaddrinfo");
    } else {
      fprintf(stderr, "error in getaddrinfo: %s\n", gai_strerror(error));
    }
    exit(EXIT_FAILURE);
  }
  char dbIpString[200];
  get_ip_str(dbinfo->ai_addr, dbIpString, 200);

  char configString[200];
  sprintf(configString, "--SERVER=%s:11211", dbIpString);
  printf("configString: %s\n", configString);

  memcached_st *memc = memcached(configString, strlen(configString));

//  assert(memc != NULL);

#define FLUSHALL memcached_flush(memc, (time_t) 0);

  FLUSHALL

  START_BENCHMARK
  for (int i = 0; i < repeatTimes; i++) {
    char str[20];
    sprintf(str, "%d", i);
    size_t vlen = strlen(str);
    memcached_return_t rc = memcached_set(memc, str, vlen, str, vlen, (time_t) 0, (uint32_t) 0);
    if (rc != MEMCACHED_SUCCESS) {
      printf("memcached raw write failed at %d\n", i);
      fprintf(stderr, "Error: %s\n", memcached_strerror(memc, rc));
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
    uint32_t flags;
    memcached_get(memc, str, vlen, &value_len, &flags, &rc);
    if (rc != MEMCACHED_SUCCESS && rc != MEMCACHED_NOTFOUND) {
      printf("memcached raw read failed at %d\n", i);
      fprintf(stderr, "Error: %s\n", memcached_strerror(memc, rc));
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
      printf("memcached write then read write failed at %d\n", i);
      fprintf(stderr, "Error: %s\n", memcached_strerror(memc, rc));
      return 1;
    }
    size_t value_len;
    memcached_get(memc, str, vlen, &value_len, NULL, &rc);
    if (rc != MEMCACHED_SUCCESS) {
      printf("memcached write then read read failed at %d\n", i);
      fprintf(stderr, "Error: %s\n", memcached_strerror(memc, rc));
      return 1;
    }
  }
  END_BENCHMARK
  SHOW_BENCHMARK("write then read");

  memcached_free(memc);

}