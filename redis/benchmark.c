#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#include "hiredis.h"

const int repeatTimes = 1000000;

int main() {

  long start, end;
  struct timeval timecheck;
  redisReply *reply;

#define START_BENCHMARK gettimeofday(&timecheck, NULL); \
  start = (long) timecheck.tv_sec * 1000 + (long) timecheck.tv_usec / 1000;

#define END_BENCHMARK gettimeofday(&timecheck, NULL); \
  end = (long) timecheck.tv_sec * 1000 + (long) timecheck.tv_usec / 1000;

#define SHOW_BENCHMARK(name) printf("%ld milliseconds elapsed in %s benchmark\n", (end - start), name);



  // Setup connection
  redisContext *c = redisConnect("db", 6379);
  if (c != NULL && c->err) {
    printf("Error: %s\n", c->errstr);
    return 1;
  } else {
    printf("Connected to Redis\n");
  }

#define FLUSHALL \
  reply = redisCommand(c, "FLUSHALL"); \
  freeReplyObject(reply);

  // Raw Write benchmark
  FLUSHALL
  START_BENCHMARK
  for (int i = 0; i < repeatTimes; i++) {
    redisReply *reply;
    char str[20];
    sprintf(str, "%d", i);
    reply = redisCommand(c, "SET %s %s", str, str);
    freeReplyObject(reply);
  }
  END_BENCHMARK
  SHOW_BENCHMARK("raw write")

  // Raw read benchmark
  START_BENCHMARK
  for (int i = 0; i < repeatTimes; i++) {
    redisReply *reply;
    char str[20];
    sprintf(str, "%d", i);
    reply = redisCommand(c, "GET %s", str);
    freeReplyObject(reply);
  }
  END_BENCHMARK
  SHOW_BENCHMARK("raw read")

  // Write then read benchmark
  FLUSHALL
  START_BENCHMARK
  for (int i = 0; i < repeatTimes; i++) {
    redisReply *reply;
    char str[20];
    sprintf(str, "%d", i);
    reply = redisCommand(c, "SET %s %s", str, str);
    freeReplyObject(reply);
    reply = redisCommand(c, "GET %s", str);
    freeReplyObject(reply);
  }
  END_BENCHMARK
  SHOW_BENCHMARK("write then read")

}