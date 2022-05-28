#include <benchmark/benchmark.h>

#include <sstream>

struct User {
  int64_t id;
  std::string username;
};

std::ostream& operator<<(std::ostream& out, const User& user) {
    return out << "{\"id\":" << user.id << ",\"username\":\"" << user.username << "\"}";
}

std::string ToJson(const User& user) {
  std::stringstream ss;
  ss << user;
  return ss.str();
}

// don't change structs!
struct Message {
  int64_t id;
  std::string subject;
  std::string body;

  User from, to;
};

// DO NOT change signature of this function.
// You may change signature of other functions.
std::string ToJson(const Message& msg) {
  std::stringstream ss;
  ss << "{"
     << "\"id\":" << msg.id << ","
     << "\"subject\":\"" << msg.subject << "\","
     << "\"body\":\"" << msg.body << "\","
     << "\"from\":" << msg.from << ","
     << "\"to\":" << msg.to << "}";
  return ss.str();
}

void BM_MessageToJson(benchmark::State& state) {
  Message msg{1000, "About modules",
              "So, when is that 'modules' proposal coming?",
              User{12345, "Herb Sutter"}, User{1, "Biern Stroustrup"}};

  for (auto _ : state) {
    benchmark::DoNotOptimize(ToJson(msg));
  }

  auto json = ToJson(msg);
  std::string expected =
      R"({"id":1000,"subject":"About modules","body":"So, when is that 'modules' proposal coming?","from":{"id":12345,"username":"Herb Sutter"},"to":{"id":1,"username":"Biern Stroustrup"}})";
  if (json != expected) {
    state.SkipWithError("Wrong output");
  }
}

BENCHMARK(BM_MessageToJson);

BENCHMARK_MAIN();
