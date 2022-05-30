#include <benchmark/benchmark.h>

#include <system_error>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "string.h"

#define BUFSIZE 4096

// try to optimize without removing syscalls!
class Logger {
public:
  Logger(const std::string& path) {
    fd_ = ::open(path.c_str(), O_CREAT | O_APPEND | O_WRONLY | O_TRUNC, 0644);
    if (fd_ == -1) {
      throw std::system_error(errno, std::system_category(), "open");
    }
  }

  ~Logger() {
    if (fd_ != -1) {
      ::close(fd_);
      fd_ = -1;
    }
  }


  void Write(const std::string& msg) {
    if (msg.size() + pos_ > BUFSIZE) {
      write_buf(buf, pos_);
      pos_ = 0;
    }
    if (msg.size() > BUFSIZE) {
      write_buf(msg.data(), msg.size());
    } else {
      msg.copy(buf + pos_, msg.size());
      pos_ += msg.size();
    }
  }

private:
  int fd_ = -1, pos_ = 0;
  char* buf;

  void write_buf(char const* data, size_t len) const {
    if (::write(fd_, data, len) !=
        static_cast<ssize_t>(len)) {
      throw std::system_error(errno, std::system_category(), "write");
    }
  }
};

static Logger TestLogger{"/tmp/benchmark_logger"};

void BM_Logger(benchmark::State& state) {
  for (auto _ : state) {
    TestLogger.Write("Test Message\n");
  }
}

BENCHMARK(BM_Logger)->Threads(1);

BENCHMARK_MAIN();
