#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/time.h>
#include <sqlite3.h>
#include <assert.h>
#include <SQLiteCpp/Database.h>
#include <string>

const int repeatTimes = 100000;

std::string db_filename = ":memory:";

int main() {

  long start, end;
  struct timeval timecheck;

#define START_BENCHMARK gettimeofday(&timecheck, NULL); \
  start = (long) timecheck.tv_sec * 1000 + (long) timecheck.tv_usec / 1000;

#define END_BENCHMARK gettimeofday(&timecheck, NULL); \
  end = (long) timecheck.tv_sec * 1000 + (long) timecheck.tv_usec / 1000;

#define SHOW_BENCHMARK(name) printf("%ld milliseconds elapsed in %s benchmark\n", (end - start), name);

  SQLite::Database db(db_filename, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);

  db.exec(
      "CREATE TABLE IF NOT EXISTS benchmark (id INTEGER NOT NULL PRIMARY KEY, key TEXT NOT NULL UNIQUE, value TEXT NOT NULL UNIQUE);");

#define FLUSHALL   db.exec("DROP TABLE IF EXISTS benchmark;"); \
  db.exec("CREATE TABLE IF NOT EXISTS benchmark (id INTEGER NOT NULL PRIMARY KEY, key TEXT NOT NULL UNIQUE, value TEXT NOT NULL UNIQUE);"); \
  db.exec("CREATE UNIQUE INDEX key_idx ON benchmark (key);");

  FLUSHALL

  {
    SQLite::Statement writeQuery(db, "INSERT INTO benchmark (key,value) VALUES (?,?);");
    SQLite::Statement readQuery(db, "SELECT value FROM benchmark WHERE key = ?;");
    START_BENCHMARK
    for (int i = 0; i < repeatTimes; i++) {
      auto str = std::to_string(i);
      writeQuery.bind(1, str);
      writeQuery.bind(2, str);
      writeQuery.exec();

      writeQuery.reset();
    }
    END_BENCHMARK
    SHOW_BENCHMARK("raw write")
  };
  {
    SQLite::Statement writeQuery(db, "INSERT INTO benchmark (key,value) VALUES (?,?);");
    SQLite::Statement readQuery(db, "SELECT value FROM benchmark WHERE key = ?;");
    START_BENCHMARK
    for (int i = 0; i < repeatTimes; i++) {
      auto str = std::to_string(i);
      readQuery.bind(1, str);
      readQuery.executeStep();
      readQuery.reset();
    }
    END_BENCHMARK
    SHOW_BENCHMARK("raw read")
  };

  FLUSHALL
  {
    SQLite::Statement writeQuery(db, "INSERT INTO benchmark (key,value) VALUES (?,?);");
    SQLite::Statement readQuery(db, "SELECT value FROM benchmark WHERE key = ?;");
    START_BENCHMARK
    for (int i = 0; i < repeatTimes; i++) {
      auto str = std::to_string(i);
      writeQuery.bind(1, str);
      writeQuery.bind(2, str);
      writeQuery.exec();
      writeQuery.reset();
      readQuery.bind(1, str);
      readQuery.executeStep();
      readQuery.reset();
    }
    END_BENCHMARK
    SHOW_BENCHMARK("write then read");
  };

}