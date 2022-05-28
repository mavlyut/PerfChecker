#include <benchmark/benchmark.h>

#include <system_error>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFSIZE 1024

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
    size_t cnt = msg.size() / BUFSIZE;
    for (size_t i = 1; i < cnt; i++) {
      std::string tmp = msg.substr((i - 1) * BUFSIZE, i * BUFSIZE);
      write_buf(tmp);
    }
    write_buf(msg.substr(cnt * BUFSIZE, msg.size()));
  }

private:
  int fd_ = -1;

  void write_buf(const std::string& buf) {
    if (::write(fd_, buf.data(), buf.size()) !=
        static_cast<ssize_t>(buf.size())) {
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
